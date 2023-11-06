#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <memory>

GLuint loadPixelToTexture(int frame_w,int frame_h,const void * pix);
void displayTexture(int frame_x,int frame_y,int frame_w,int frame_h,int window_w,int window_h, GLuint display_texture);

#ifndef __Frame__
class RawFrame;
struct FrameRect{
    int x,y,w,h;
};

struct FrameSize{
    int w,h;
};

class DisplayTexture{
public:
    void display(std::shared_ptr<RawFrame> &frame,struct FrameRect rect,struct FrameSize size);
};

#define __Frame__
#endif
