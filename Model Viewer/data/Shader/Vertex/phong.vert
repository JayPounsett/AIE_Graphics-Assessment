// classic Phong vertex shader
#version 460

layout( location = 0) in vec4 Position;
layout( location = 1) in vec4 Normal;
//layout( location = 2) in vec2 TexCoord;

out vec4 vPosition;
out vec3 vNormal;
//out vec2 vTexCoord;

uniform mat4 ProjectionViewModel;

//we need this matrix to transform the normal
uniform mat4 ModelMatrix;

void main(){
	vPosition = ModelMatrix * Position;
	vNormal = (ModelMatrix * Normal).xyz;
	//vTexCoord = TexCoord;
	gl_Position = ProjectionViewModel * Position;
}