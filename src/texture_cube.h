#ifndef TEXTURE_CUBE_H
#define TEXTURE_CUBE_H
#include "texture.h"

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::vector<std::string> skyboxTextureDay = {
        "../resources/Sky Textures/right.jpg","../resources/Sky Textures/left.jpg","../resources/Sky Textures/bottom.jpg",
        "../resources/Sky Textures/top.jpg","../resources/Sky Textures/front.jpg","../resources/Sky Textures/back.jpg"};

std::vector<std::string> skyboxTextureNight = {
        "../resources/Night Sky Textures/nightLeft.png","../resources/Night Sky Textures/nightRight.png","../resources/Night Sky Textures/nightBottom.png",
        "../resources/Night Sky Textures/nightTop.png","../resources/Night Sky Textures/nightFront.png","../resources/Night Sky Textures/nightBack.png"};

class CubemapTexture{
public:
    unsigned int textureID;
    int width;
    int height;
    int channels;

    CubemapTexture(std::vector<std::string> faces)
    {
        if(faces.size() < 6){
            std::cout<<"Not enought faces for Cube Maps"<<std::endl;
            abort();
        }
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
        for(unsigned int i=0; i < faces.size(); i++){
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels,0);
            if(data){
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            }else{
                std::cout << "Failed to load texture: "+faces[i] << std::endl;
            }
            stbi_image_free(data);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 
    }
};
#endif