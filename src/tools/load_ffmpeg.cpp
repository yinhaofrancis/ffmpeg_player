#include "load_ffmpeg.h"
#include <iostream>




RawFrame::RawFrame(int width,
                    int height,
                    int countOfColorComponent,
                    uint8_t* buffer):buffer(buffer),width(width),height(height),countOfColorComponent(countOfColorComponent){}
RawFrame::~RawFrame(){
    delete[] this->buffer;
}
int RawFrame::getWidth(){
    return this->width;
}
int RawFrame::getHeight(){
    return this->height;
}
std::shared_ptr<RawFrame> VideoSource::next(double current_time,int width_dest){

    if(this->pendingFrame == nullptr){
        auto packet = av_packet_alloc(); 
        auto frame = av_frame_alloc();
        if (av_read_frame(this->format_ctx,packet) == 0){
            if(packet->stream_index == video_stream_index){
                auto response = avcodec_send_packet(this->video_codec_ctx,packet);
                if(response != 0){
                    av_packet_free(&packet);
                    av_frame_free(&frame);
                    throw VideoSourceError("send packet to codec fail " + std::string(av_err2str(response)));
                }
                auto reponse = avcodec_receive_frame(
                    this->video_codec_ctx, frame
                );
                if(response != 0){
                    av_packet_free(&packet);
                    av_frame_free(&frame);
                    throw VideoSourceError("decode fail " + std::string(av_err2str(response)));
                }
                this->pendingFrame = frame;
            }
        }else{
            av_packet_free(&packet);
            av_frame_free(&frame);
            return nullptr;
        }
        av_packet_free(&packet);
    }
    double cts = current_time  / this->time_base.num * (double)this->time_base.den;
    if(cts > this->pendingFrame->pts){
        auto out = this->genRawBuffer(this->pendingFrame,width_dest);
        this->pendingFrame = nullptr;
        return out;
    }
    return nullptr;
    
}
std::shared_ptr<RawFrame> VideoSource::genRawBuffer(AVFrame * frame,int width_dest){
    int buffer_size = frame->width * frame->height * 4;
    if(buffer_size == 0){
        av_frame_free(&frame);
        return nullptr;
    }
    int width = frame->width;
    int height = frame->height;
    int height_dest = height;
    if(width_dest <= 0){
        width_dest = width;
    }else{
        double ratio = (double)height / width;
        height_dest = width_dest * ratio;
    }
    uint8_t  *pix = new uint8_t[buffer_size];
    if(this->sws_context == nullptr || this->last_image_out_width != width_dest){
        SwsContext* sws_context = sws_getContext(
                width,
                height,
                this->video_codec_ctx->pix_fmt,
                width_dest,
                height_dest,
                AV_PIX_FMT_RGB0,
                SWS_BILINEAR,NULL,NULL,NULL);
        this->sws_context = sws_context;
    }
    uint8_t* dest[4] = {pix,0,0,0};
    int dest_linezize[4] = {width * 4,0,0,0};
    sws_scale(this->sws_context,frame->data,frame->linesize,0,frame->height,dest,dest_linezize);
    std::shared_ptr result = std::make_shared<RawFrame>(width,height,4,pix);
    av_frame_free(&frame);
    sws_freeContext(sws_context);
    return result;
}
VideoSource::VideoSource(const char *path,int outWidth):path(path){
    this->format_ctx = avformat_alloc_context();
    if(this->format_ctx == nullptr){
        throw VideoSourceError("create avformatcontext");
    }
    if(avformat_open_input(&this->format_ctx,path,NULL,NULL) != 0){
        throw VideoSourceError("open file fail");
    }
    
    for (size_t i = 0; i < this->format_ctx->nb_streams; i++)
    {
        auto stream = this->format_ctx->streams[i];
        auto codec_para = stream->codecpar;
        if(codec_para->codec_type == AVMEDIA_TYPE_VIDEO){
            this->video_codec_ctx = this->createCodecContext(stream);
            this->video_stream_index = i;
            this->video_codec_params = stream->codecpar;
            this->time_base = stream->time_base;
        }
        if(codec_para->codec_type == AVMEDIA_TYPE_AUDIO){
            this->audio_codec_ctx = this->createCodecContext(stream);
            this->audio_stream_index = i;
            this->audio_codec_params = stream->codecpar;
        }
    }   
    this->pendingFrame = nullptr; 
}
AVCodecContext * VideoSource::createCodecContext(AVStream *stream){
   
    AVCodecParameters *codec_params = stream->codecpar;
    const AVCodec *codec = avcodec_find_decoder(codec_params->codec_id);
    if(codec == nullptr){
        throw VideoSourceError("find codec fail");
    }

    auto codec_ctx = avcodec_alloc_context3(codec);
    if(codec_ctx == nullptr){
        throw VideoSourceError("create codec context fail");
    }

    if (0 != avcodec_parameters_to_context(codec_ctx,codec_params)){
        throw VideoSourceError("initialize codec context fail");
    }
    if (avcodec_open2(codec_ctx,codec,NULL) < 0){
        throw VideoSourceError("open codec fail");
    }
    return codec_ctx;
}
VideoSource ::~VideoSource(){
    avformat_free_context(this->format_ctx);
    avformat_close_input(&this->format_ctx);
    avcodec_free_context(&this->video_codec_ctx);
    avcodec_free_context(&this->audio_codec_ctx);
}
int VideoSource::width(){
    return this->video_codec_params->width;
}
int VideoSource::height(){
    return this->video_codec_params->height;
}