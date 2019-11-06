precision mediump float;
uniform samplerCube u_texture0;
varying vec3 v_coord;
void main()
{
	vec4 color = textureCube(u_texture0, v_coord);
	if (color.a < 0.2)
		discard;
	gl_FragColor = color;
}
