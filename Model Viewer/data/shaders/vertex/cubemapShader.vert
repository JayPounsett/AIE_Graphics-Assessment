// Phong vertex shader (no texture)
#version 460

layout( location = 0) in vec4 Position;

out vec3 TexCoord;

uniform mat4 Projection;
uniform mat4 View;

void main(){
	TexCoord = Position;
	gl_Position = Projection * View * Position;
}