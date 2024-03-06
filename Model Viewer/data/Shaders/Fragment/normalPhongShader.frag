// Normal Phong fragment shader
#version 460

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vTangent;
in vec3 vBiTangent;

uniform vec3 CameraPosition;

uniform vec3 LightColour;
uniform vec3 AmbientColour;
uniform vec3 LightDirection;

const int MAX_LIGHTS = 4;
uniform int numLights;
uniform vec3 PointLightColour[MAX_LIGHTS];
uniform vec3 PointLightPosition[MAX_LIGHTS];

uniform vec3 Ka;             // Ambient material colour 
uniform vec3 Kd;             // Diffuse material colour 
uniform vec3 Ks;             // Specular material colour 
uniform float SpecularPower; // Tightness of specular highlights

uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform sampler2D normalTex; 

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

void main()
{
	// Ensure normal and light direction are normalised
	vec3 N = normalize(vNormal);
	vec3 T = normalize(vTangent);
	vec3 B = normalize(vBiTangent);
	vec3 L = normalize(LightDirection);

	vec3 textureNormal = texture(normalTex, vTexCoord).rgb;
	mat3 TBN = mat3(T, B, N);
	N = TBN * (textureNormal * 2 - 1);

	// Calculate view vector
	vec3 V = normalize(CameraPosition - vPosition.xyz);

	// Calculate lambert term (negate light direction)
	vec3 diffuseTotal = GetDiffuse(L, LightColour, N);

	// Calclate specular term from directional lights
	vec3 specularTotal = GetSpecular(L, LightColour, N, V);

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
	
	vec3 textureDiffuse  = texture(diffuseTex, vTexCoord).rgb;  // Only interested in RGB, don't need to worry about Alpha
	vec3 textureSpecular = texture(specularTex, vTexCoord).rgb;
	
	vec3 ambient  = AmbientColour * Ka;
	vec3 diffuse = diffuseTotal * Kd * textureDiffuse;
	vec3 specular = specularTotal * Ks * textureSpecular;

	FragColour = vec4( ambient + diffuse + specular, 1 );
}