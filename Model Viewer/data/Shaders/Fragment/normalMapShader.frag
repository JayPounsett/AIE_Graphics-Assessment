// Normal Phong fragment shader
#version 460

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vTangent;
in vec3 vBiTangent;

uniform vec3 CameraPosition;

uniform vec3 LightColour;
uniform vec3 LightDirection;
uniform vec3 AmbientColour;

uniform vec3 Ka;             // Ambient material colour 
uniform vec3 Kd;             // Diffuse material colour 
uniform vec3 Ks;             // Specular material colour 
uniform float SpecularPower; // Tightness of specular highlights

uniform sampler2D DiffuseTexture;
uniform sampler2D SpecularTexture;
uniform sampler2D NormalTexture; 

out vec4 FragColour;

void main(){
	// Ensure normal and light direction are normalised
	vec3 N = normalize(vNormal);
	vec3 T = normalize(vTangent);
	vec3 B = normalize(vBiTangent);
	vec3 L = normalize(LightDirection);

	mat3 TBN = mat3(T, B, N);

	// Calculate lambert term (negate light direction)
	float lambertTerm = max(0, min( 1, dot(N, -L)));

	// Calculate view vector and reflection vector
	vec3 V = normalize(CameraPosition - vPosition.xyz);
	vec3 R = reflect( L, N );

	float specularTerm = pow( max( 0, dot( R, V ) ), SpecularPower );
	
	vec3 textureDiffuse = texture(DiffuseTexture, vTexCoord).rgb;  // Only interested in RGB, don't need to worry about Alpha
	vec3 textureSpecular = texture(SpecularTexture, vTexCoord).rgb;
	vec3 textureNormal = texture(NormalTexture, vTexCoord).rgb;

	N = TBN * (textureNormal * 2 - 1);
	
	vec3 ambient = AmbientColour * Ka * textureDiffuse;
	vec3 diffuse = LightColour * Kd * lambertTerm * textureDiffuse;	
	vec3 specular = LightColour * Ks * specularTerm;

	FragColour = vec4( N, 1 );
}