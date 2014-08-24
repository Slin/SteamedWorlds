//
//  rn_Texture1.fsh
//  Rayne
//
//  Copyright 2013 by Überpixel. All rights reserved.
//  Unauthorized use is punishable by torture, mutilation, and vivisection.
//

#version 150
precision highp float;

#if !defined(TILE_FACTOR)
	#define TILE_FACTOR 1.0
#endif

#include <shader/rn_Lighting.fsh>

uniform sampler2D mTexture0;
uniform sampler2D mTexture1;

#if defined(RN_SPECULARITY)
	uniform vec4 specular;
#endif

in vec2 vertTexcoord;

#if defined(SECOND_UV)
in vec2 vertTexcoord2;
#endif

in vec3 vertNormal;
in vec3 vertPosition;

out vec4 fragColor0;

void main()
{
#if !defined(SECOND_UV)
	vec3 colorAO = texture(mTexture0, vertTexcoord).rgb;
	vec3 color = texture(mTexture1, vertTexcoord*TILE_FACTOR).rgb;
#else
	vec3 colorAO = texture(mTexture0, vertTexcoord).rgb;
	vec3 color = texture(mTexture1, vertTexcoord2*TILE_FACTOR).rgb;
#endif

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
