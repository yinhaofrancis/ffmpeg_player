#include <iostream>
#include <string>

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
    
    int frame_w,frame_h;
    auto source = VideoSource("/Users/haoyin/Desktop/aaaa-1.mov");
    
    while (!glfwWindowShouldClose(window))
    {
        int width,height;
        glfwGetFramebufferSize(window, &width, &height);
        auto frame = source.next(glfwGetTime());
        if(frame != nullptr){
            display.display(frame,{0,0,frame->getWidth() * 2 ,frame->getHeight() * 2},{width,height});
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}





    