//classic Phong fragment shader
#version 460

in vec4 vPosition;
in vec3 vNormal;

uniform vec3 CameraPosition;

uniform vec3 Ka; // ambient material colour 
uniform vec3 Kd; // diffuse material colour 
uniform vec3 Ks; // specular material colour 
uniform float SpecularPower; // material specular power

uniform vec3 LightColour;
uniform vec3 LightDirection;
uniform vec3 AmbientColour;

out vec4 FragColour;

void main(){
	//ensure normal and light direction are normalised
	vec3 N = normalize(vNormal);
	vec3 L = normalize(LightDirection);

	//calculate lambert term (negate light direction)
	float lambertTerm = max(0, min( 1, dot(N, -L)));

	//calculate view vector and reflection vector
	vec3 V = normalize(CameraPosition - vPosition.xyz);
	vec3 R = reflect( L, N );

	//calculate specular term
	float specularTerm = pow( max( 0, dot( R, V ) ), SpecularPower );

	//Set ambient up
	vec3 ambient = AmbientColour * Ka;
	
	//Calculate diffuse
	vec3 diffuse = LightColour * Kd * lambertTerm;
	
	//Calculate specular
	vec3 specular = LightColour * Ks * specularTerm;

	//output final colour
	FragColour = vec4( ambient + diffuse + specular, 1 );
}