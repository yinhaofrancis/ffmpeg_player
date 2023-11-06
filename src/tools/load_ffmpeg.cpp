#include "load_ffmpeg.h"


bool load_frame(const char *path,int *width,int *height,const char * data){
    return false;
}

RawFrame::RawFrame(int width,int height,int countOfColorComponent,uint8_t* buffer):buffer(buffer),width(width),height(height),countOfColorComponent(countOfColorComponent){}
RawFrame::~RawFrame(){
    delete[] this->buffer;
}

VideoSource::VideoSource(const char *path):path(path){}

std::shared_ptr<RawFrame> VideoSource::next(){

    uint8_t  *pix = new uint8_t[100 * 100 * 3];
    for (size_t i = 0; i < 100; i++)
    {
        for (size_t j = 0; j < 100; j++)
        {
            if(i < 75 && i > 25 && j > 25 && j < 75){
                pix[i * 300 + j * 3] = 0x0;
                pix[i * 300 + j * 3 + 1] = 0xff;
                pix[i * 300 + j * 3 + 2] = 0;
            }else{
                pix[i * 300 + j * 3] = 0xff;
                pix[i * 300 + j * 3 + 1] = 0;
                pix[i * 300 + j * 3 + 2] = 0;
            }
            
        }
    }
    std::shared_ptr result = std::make_shared<RawFrame>(100,100,3,pix);
    return result;
}