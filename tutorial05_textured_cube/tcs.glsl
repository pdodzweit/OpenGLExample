#version 150
#extension GL_ARB_tessellation_shader : enable

layout(vertices = 3) out;

in vec3 WorldPos_CS_in[];
out vec3 WorldPos_ES_in[];

void main()
{
    WorldPos_ES_in[gl_InvocationID] = WorldPos_CS_in[gl_InvocationID];
	
	gl_TessLevelOuter[0] = 5.0;
	gl_TessLevelOuter[1] = 5.0;
	gl_TessLevelOuter[2] = 5.0;
	gl_TessLevelInner[0] = 5.0;
	// gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}


