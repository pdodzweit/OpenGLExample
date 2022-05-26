#version 150

in vec4 s_attribute_0;
out vec3 WorldPos_CS_in;
	
void main() 
{
	WorldPos_CS_in = s_attribute_0.xyz;
}
	


