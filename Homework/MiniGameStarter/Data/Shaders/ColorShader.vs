attribute vec3 a_posL;
attribute vec2 a_uv;
varying vec2 v_uv;
uniform mat4 u_wvpMatrix;
uniform float u_alpha;

void main()
{
	vec4 posL = vec4(a_posL, u_alpha);
	gl_Position = u_wvpMatrix * posL;
	v_uv = a_uv;
}   