precision mediump float;
varying vec2 v_uv;
uniform sampler2D u_texture;
uniform vec4 u_color;

void main()
{
	gl_FragColor = vec4(u_color.r, u_color.g, u_color.b, texture2D(u_texture, v_uv).a * u_color.a);
}
