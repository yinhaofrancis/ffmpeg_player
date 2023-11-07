#include <memory>
#include <string>

#include "display_texture.h"
extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavdevice/avdevice.h>
    #include <libavformat/avformat.h>
    #include <libavutil/avutil.h>
    #include <libavfilter/avfilter.h>
    #include <GLFW/glfw3.h> 
    #include <libswscale/swscale.h>
}


#ifndef __LOAD_FFMPEG__
#define  __LOAD_FFMPEG__

struct VideoSourceError {
    std::string message;
    VideoSourceError(std::string errorMessage):message(errorMessage){}
};


class RawFrame {
public:
    ~RawFrame();
    RawFrame(int width,int height,int countOfColorComponent,unsigned char *buffer);
    friend class VideoSource;
    friend class DisplayTexture;
    int getWidth();
    int getHeight();
private:
    int width,height,countOfColorComponent;
    uint8_t * buffer;
};


class VideoSource {
private:
    const char *path;
    AVFormatContext *format_ctx;
    AVCodecContext *video_codec_ctx;
    AVCodecContext *audio_codec_ctx;
    AVCodecParameters *video_codec_params;
    AVCodecParameters *audio_codec_params;
    int video_stream_index;
    int audio_stream_index;
    SwsContext* sws_context;
    int last_image_out_width;
    AVRational time_base;
    AVCodecContext * createCodecContext(AVStream *stream);
    std::shared_ptr<RawFrame> genRawBuffer(AVFrame * frame,int width_dest);
    AVFrame* pendingFrame;
public:
    VideoSource(const char *path,int outWidth = 0);
    ~VideoSource();
    void beginRelateTimeBase();
    std::shared_ptr<RawFrame>next(double current_time,int width = 0);
    int width();
    int height();
};

#endif
