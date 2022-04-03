#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{
    // fill in
    // Hint) you can ignore transparent texture pixel by 
    // if(color.a < 0.5){discard;}
    vec4 texColor = texture(texture1,TexCoord);
    if(texColor.a < 0.5){
        discard;
    }
    FragColor = texColor;
}