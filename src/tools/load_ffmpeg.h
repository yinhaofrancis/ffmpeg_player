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
    int video_stream_index;
    int audio_stream_index;
    AVCodecContext * createCodecContext(AVStream *stream);
public:
    VideoSource(const char *path);
    ~VideoSource();
    std::shared_ptr<RawFrame>next();
};

#endif
