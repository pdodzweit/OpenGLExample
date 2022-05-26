#version 150
#extension GL_ARB_tessellation_shader : enable

layout(triangles,fractional_odd_spacing,ccw) in;

in vec3 WorldPos_ES_in[];

void main() 
{		
	gl_Position.xyz = gl_TessCoord.x*WorldPos_ES_in[0] + gl_TessCoord.y*WorldPos_ES_in[1] + gl_TessCoord.z*WorldPos_ES_in[2];
	gl_Position.w = 1.0;
}


