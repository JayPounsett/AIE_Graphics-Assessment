#version 330 core

out vec4 FragColour;

in vec3 TexCoord;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoord);
}