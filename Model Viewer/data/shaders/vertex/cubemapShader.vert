// Cubemap Shader
#version 460

layout( location = 0) in vec3 Position;

out vec3 TexCoord;

uniform mat4 Projection;
uniform mat4 View;

void main(){
	TexCoord = Position;
	vec4 pos = Projection * View * vec4(Position, 1.0f);
	gl_Position = pos.xyww;
}