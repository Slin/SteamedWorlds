#version 150
precision highp float;

#include <shader/rn_Matrices.vsh>
#include <shader/rn_Animation.vsh>
#include <shader/rn_Shadow.vsh>

in vec3 attPosition;
in vec3 attNormal;
in vec2 attTexcoord0;
in vec4 attTangent;

out vec2 vertTexcoord;
out vec3 vertNormal;
out vec3 vertPosition;

void main()
{
	vertTexcoord = attTexcoord0;
	
	vertNormal = (matModel * vec4(attNormal, 0.0)).xyz;
	vertPosition = (matModel * vec4(attPosition, 1.0)).xyz;
	
	#if defined(RN_DIRECTIONAL_SHADOWS) && defined(RN_LIGHTING)
		rn_ShadowDirectional0(vec4(vertPosition, 1.0));
	#endif
	
	gl_Position = matProjViewModel * vec4(attPosition, 1.0);
}
