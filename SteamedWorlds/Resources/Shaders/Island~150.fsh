//
//  rn_Texture1.fsh
//  Rayne
//
//  Copyright 2013 by Überpixel. All rights reserved.
//  Unauthorized use is punishable by torture, mutilation, and vivisection.
//

#version 150
precision highp float;

#include <shader/rn_Lighting.fsh>

uniform sampler2D mTexture0;
uniform sampler2D mTexture1;
uniform sampler2D mTexture2;
uniform sampler2D mTexture3;
uniform sampler2D mTexture4;
uniform sampler2D mTexture5;
//uniform sampler2D mTexture6;

#if defined(RN_SPECULARITY)
	uniform vec4 specular;
#endif

in vec2 vertTexcoord;
in vec3 vertNormal;
in vec3 vertPosition;

out vec4 fragColor0;

void main()
{
	vec2 tileCoords = vertTexcoord*30.0f;
	
	vec3 colorAO = texture(mTexture0, vertTexcoord).rgb;
	vec4 colorSplatmap = texture(mTexture1, vertTexcoord);
	vec3 colorBase = texture(mTexture2, tileCoords).rgb;
	vec3 colorRed = texture(mTexture3, tileCoords).rgb;
	vec3 colorGreen = texture(mTexture4, tileCoords).rgb;
	vec3 colorBlue = texture(mTexture5, tileCoords).rgb;
//	vec3 colorAlpha = texture(mTexture6, tileCoords).rgb;
	
	vec3 color = mix(colorBase, colorRed, colorSplatmap.r);
	color = mix(color, colorGreen, colorSplatmap.g);
	color = mix(color, colorBlue, colorSplatmap.b);
//	color = mix(color, colorAlpha, colorSplatmap.a);

	color *= colorAO;

	vec4 color0 = vec4(color, 1.0);
	#if defined(RN_LIGHTING)
		#if defined(RN_SPECULARITY)
			vec4 spec;
			spec.rgb = specular.rgb*bottomBlend;
			spec.a = specular.a;
		#else
			vec4 spec = vec4(0.0);
		#endif
		vec3 normal = normalize(vertNormal);
		rn_Lighting(color0, spec, normal, vertPosition);
	#endif
	
	fragColor0 = color0;
}
