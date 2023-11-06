#include <memory>

#include "display_texture.h"


bool load_frame(const char *path,int *width,int *height,const char * data);


class RawFrame{
public:
    ~RawFrame();
    RawFrame(int width,int height,int countOfColorComponent,unsigned char *buffer);
    friend class VideoSource;
    friend class DisplayTexture;
private:
    int width,height,countOfColorComponent;
    uint8_t * buffer;
};


class VideoSource{
private:
    const char *path;
public:
    VideoSource(const char *path);
    std::shared_ptr<RawFrame>next();
};

