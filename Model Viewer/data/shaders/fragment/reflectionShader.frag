// Normal Phong fragment shader  (No Texture, Reflection Only)
#version 460

out vec4 FragColour;

in vec4 vPosition;
in vec3 vNormal;

uniform vec3 CameraPosition;
uniform samplerCube skyboxTex;

// The below parameters (line 16 to 28 and related functions) are 
// only used so the shader messages are happy that variables are 
// passed in, even if they are zero'd in the Mesh constructor

uniform vec3 LightColour;
uniform vec3 AmbientColour;
uniform vec3 LightDirection;

const int MAX_LIGHTS = 4;
uniform int numLights;
uniform vec3 PointLightColour[MAX_LIGHTS];
uniform vec3 PointLightPosition[MAX_LIGHTS];

uniform vec3 Ka; // Ambient material colour 
uniform vec3 Kd; // Diffuse material colour 
uniform vec3 Ks; // Specular material colour 
uniform float SpecularPower; // Tightness of specular highlights

vec3 GetDiffuse(vec3 direction, vec3 colour, vec3 normal)
{
	return colour * max(0, min(1, dot(normal, -direction)));
}

vec3 GetSpecular(vec3 direction, vec3 colour, vec3 normal, vec3 view)
{
	vec3 R = reflect(direction, normal);
	float specularTerm = pow( max( 0, dot( R, view ) ), SpecularPower );
	return specularTerm * colour;
}

float SchlickApprox(vec3 I, vec3 normal){
	return pow(1.f - (dot(I, normal)), 5.f);
}

void main(){
	// Ensure normal and light direction are normalised
	vec3 N = normalize(vNormal);
	vec3 L = normalize(LightDirection);
	
	// Calculate view vector and reflection vector
	vec3 V = normalize(CameraPosition - vPosition.xyz);

	// Calculate lambert term (negate light direction)
	vec3 diffuseTotal = GetDiffuse(L, LightColour, N);

	// Calclate specular term from directional lights
	vec3 specularTotal = GetSpecular(L, LightColour, N, V);

	vec3 reflectionDirection = reflect(V, N);

	// Create mirrored surface and not using Schlick so there is no edges to the cube
	vec3 reflectionColour = texture(skyboxTex, reflectionDirection).rgb;

	// Calculate light from point lights
	for(int i = 0; i < numLights; i++)
	{
		vec3 direction = vPosition.xyz - PointLightPosition[i];
		float distance = length(direction);
		direction = direction / distance;

		vec3 colour = PointLightColour[i] / (distance * distance); // Closer it is, the more intense it is

		diffuseTotal += GetDiffuse(direction, colour, N);
		specularTotal += GetSpecular(direction, colour, N, V);
	}

	vec3 ambient  = AmbientColour * Ka;
	vec3 diffuse  = diffuseTotal * Kd;	
	vec3 specular = specularTotal * Ks;

	// Show no colour and have surfaces act as a mirror
	FragColour = vec4(ambient + diffuse + specular + reflectionColour, 1 );
}