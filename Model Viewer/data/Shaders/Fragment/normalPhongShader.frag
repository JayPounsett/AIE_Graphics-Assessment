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

uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform sampler2D normalTex; 

out vec4 FragColour;

void main(){
	// Ensure normal and light direction are normalised
	vec3 N = normalize(vNormal);
	vec3 T = normalize(vTangent);
	vec3 B = normalize(vBiTangent);
	vec3 L = normalize(LightDirection);

	vec3 textureNormal = texture(normalTex, vTexCoord).rgb;
	mat3 TBN = mat3(T, B, N);
	N = TBN * (textureNormal * 2 - 1);

	// Calculate lambert term (negate light direction)
	float lambertTerm = max(0, min( 1, dot(N, -L)));

	// Calculate view vector and reflection vector
	vec3 V = normalize(CameraPosition - vPosition.xyz);
	vec3 R = reflect( L, N );

	float specularTerm = pow( max( 0, dot( R, V ) ), SpecularPower );
	
	vec3 textureDiffuse  = texture(diffuseTex, vTexCoord).rgb;  // Only interested in RGB, don't need to worry about Alpha
	vec3 textureSpecular = texture(specularTex, vTexCoord).rgb;
	
	vec3 ambient  = AmbientColour * Ka;
	vec3 diffuse  = LightColour * Kd * lambertTerm * textureDiffuse;	
	vec3 specular = LightColour * Ks * specularTerm * textureSpecular;

	FragColour = vec4( ambient + diffuse + specular, 1 );
}