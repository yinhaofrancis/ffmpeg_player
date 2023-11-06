#include <iostream>
#include <string>
extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavdevice/avdevice.h>
    #include <libavformat/avformat.h>
    #include <libavutil/avutil.h>
    #include <libavfilter/avfilter.h>
    #include <GLFW/glfw3.h> 
}
#include "tools/display_texture.h"
#include "tools/load_ffmpeg.h"

class MediaPlayer{
    MediaPlayer(std::string url){
        
    }
    virtual ~MediaPlayer(){

    }
};

int main(int, char**){
    if(!glfwInit()){
        std::cout<< "glfw init fail"<<std::endl;
        return 1;
    }

    auto window = glfwCreateWindow(640,360,"title",NULL,NULL);
    if(!window){
        return 1;
    }
    glfwMakeContextCurrent(window);
    
    auto display = DisplayTexture();
    
    int frame_x = 100,frame_y = 100;
    int frame_w,frame_h;
    auto source = VideoSource("");
    
    while (!glfwWindowShouldClose(window))
    {
        int width,height;
        glfwGetFramebufferSize(window, &width, &height);
        auto frame = source.next();
        display.display(frame,{frame_x,frame_y,500,500},{width,height});
        glfwSwapBuffers(window);
        glfwWaitEvents();
    }
    
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    return 0;
}





    