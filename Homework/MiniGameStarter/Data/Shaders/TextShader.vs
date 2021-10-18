attribute vec4 a_posL;
varying vec2 v_uv;
void main() 
{
	gl_Position = vec4(a_posL.xy, 0.0, 1.0);
	v_uv = a_posL.zw;
}