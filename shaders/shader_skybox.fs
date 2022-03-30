#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skyboxTexture1;
uniform samplerCube skyboxTexture2;

void main()
{   
    // mix two texture
    FragColor = mix(texture(skyboxTexture1, TexCoords), texture(skyboxTexture2,TexCoords),0.5);
}
