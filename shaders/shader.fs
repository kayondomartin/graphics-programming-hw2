#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D cubeTexture;
uniform sampler2D texture2;

void main()
{
    // fill in
    // Hint) you can ignore transparent texture pixel by 
    // if(color.a < 0.5){discard;}
    FragColor = texture(cubeTexture,TexCoord);
}