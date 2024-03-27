// Cubemap Shader
#version 460

layout( location = 0) in vec4 Position;

out vec3 TexCoord;

uniform mat4 Projection;
uniform mat4 View;

void main(){
	TexCoord = Position.xyz;
	gl_Position = Projection * mat4(mat3(View)) * Position;
}