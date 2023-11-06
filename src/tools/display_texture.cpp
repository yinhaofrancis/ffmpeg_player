#include "display_texture.h"
#include "load_ffmpeg.h"

GLuint loadPixelToTexture(int frame_w,int frame_h,const void * pix){
    GLuint display_texture;
    glGenTextures(1,&display_texture);
    glBindTexture(GL_TEXTURE_2D,display_texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,frame_w,frame_h,0,GL_RGB,GL_UNSIGNED_BYTE,pix);
    return display_texture;
}

void displayTexture(int frame_x,int frame_y,int frame_w,int frame_h,int window_w,int window_h, GLuint display_texture){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0,0,0,0);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, window_w, window_h, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,display_texture);
        glBegin(GL_QUADS);
            glTexCoord2d(0,0);glVertex2d(frame_x,frame_y);
            glTexCoord2d(0,1);glVertex2d(frame_x,frame_y + frame_h);
            glTexCoord2d(1,1);glVertex2d(frame_x + frame_w,frame_y + frame_h);
            glTexCoord2d(1,0);glVertex2d(frame_x + frame_w,frame_y);
        glEnd();
        glDisable(GL_TEXTURE_2D);
}

void DisplayTexture::display(std::shared_ptr<RawFrame> &frame,FrameRect rect,FrameSize size){
    auto texture = loadPixelToTexture(frame->width,frame->height,frame->buffer);
    displayTexture(rect.x,rect.y,rect.w,rect.h,size.w,size.h,texture);
}