// Phong fragment shader
#version 460

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

uniform vec3 CameraPosition;

uniform vec3 LightColour;
uniform vec3 LightDirection;
uniform vec3 AmbientColour;

const int MAX_LIGHTS = 4;
uniform int numLights;
uniform vec3 PointLightColour[MAX_LIGHTS];
uniform vec3 PointLightPosition[MAX_LIGHTS];

uniform vec3 Ka; // Ambient material colour 
uniform vec3 Kd; // Diffuse material colour 
uniform vec3 Ks; // Specular material colour 
uniform float SpecularPower; // Tightness of specular highlights

uniform sampler2D diffuseTex;

out vec4 FragColour;

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

void main(){
	// Ensure normal and light direction are normalised
	vec3 N = normalize(vNormal);
	vec3 L = normalize(LightDirection);

	// Calculate view vector and reflection vector
	vec3 V = normalize(CameraPosition - vPosition.xyz);

	// Calculate lambert term (negate light direction)
	vec3 diffuseTotal = GetDiffuse(L, LightColour, N);
	//float lambertTerm = max(0,min(1, dot(N, L)));

	// Calclate specular term from directional lights
	vec3 specularTotal = GetSpecular(L, LightColour, N, V);
	//float specularTerm = pow( max( 0, dot( L, V ) ), SpecularPower );

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

	vec3 textureDiffuse = texture(diffuseTex, vTexCoord).rgb;  // Only interested in RGB, don't need to worry about Alpha

	vec3 ambient  = AmbientColour * Ka;
	vec3 diffuse  = diffuseTotal * Kd * textureDiffuse;	
	//vec3 diffuse  = lambertTerm * LightColour * Kd * textureDiffuse;	
	vec3 specular = specularTotal * Ks;
	//vec3 specular = specularTerm * LightColour * Ks;

	FragColour = vec4( ambient + diffuse + specular, 1 );
}