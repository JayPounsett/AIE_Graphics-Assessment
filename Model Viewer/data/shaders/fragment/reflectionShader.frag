// Normal Phong fragment shader  (No Texture, Reflection Only)
#version 460

in vec4 vPosition;
in vec3 vNormal;

uniform vec3 CameraPosition;

uniform samplerCube skyboxTex;

out vec4 FragColour;

float SchlickApprox(vec3 I, vec3 normal){
	return pow(1.f - (dot(I, normal)), 5.f);
}

void main(){
	// Ensure normal and light direction are normalised
	vec3 N = normalize(vNormal);
	
	// Calculate view vector and reflection vector
	vec3 V = normalize(CameraPosition - vPosition.xyz);

	vec3 reflectionDirection = reflect(V, N);

	vec3 reflectionColour = texture(skyboxTex, reflectionDirection).rgb * SchlickApprox(V, N);

	// Show no colour and have surfaces act as a mirror
	FragColour = vec4(reflectionColour, 1 );
}