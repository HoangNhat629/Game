attribute 	vec3 	a_posL;
attribute 	vec2 	a_uv;
varying 	vec2 	v_uv;
uniform 	mat4 	u_wvpMatrix;
uniform		ivec2	u_atlasSize;
uniform		ivec2	u_atlasCoord;

void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	gl_Position = u_wvpMatrix * posL;

	vec2 subTexturePosition = vec2(float(u_atlasCoord.x) / float(u_atlasSize.x), float(u_atlasCoord.y) / float(u_atlasSize.y));
	vec2 subTextureUV = vec2(a_uv.x / float(u_atlasSize.x), a_uv.y / float(u_atlasSize.y));
	v_uv = subTexturePosition + subTextureUV;
}
   