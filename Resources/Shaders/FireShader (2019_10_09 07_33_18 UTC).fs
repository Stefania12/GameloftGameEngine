precision mediump float;
varying vec2 v_uv;
uniform sampler2D u_texture0;
uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform float u_time;
uniform float u_dispMax;

varying vec4 v_position;
uniform vec3 u_camPos;
uniform float u_minRadius;
uniform float u_maxRadius;
uniform vec3 u_fogColor;

void main()
{
	vec2 disp = texture2D(u_texture0, vec2(v_uv.x, v_uv.y + u_time)).rg;
	vec2 offset;
	offset.x = disp.r * u_dispMax * 2.0 - u_dispMax;
	offset.y = disp.g * u_dispMax * 2.0 - u_dispMax;
	vec2 v_uv_displaced = v_uv + offset;
	vec4 c_fire = texture2D(u_texture1, v_uv_displaced);
	vec4 c_alpha = texture2D(u_texture2, v_uv);
	c_fire.a = c_fire.a * c_alpha.r;

	float d = distance(vec3(v_position.x, v_position.y, v_position.z), u_camPos);
	float alpha = (clamp(d, u_minRadius, u_maxRadius) - u_minRadius)/(u_maxRadius - u_minRadius);
	vec3 modified_color = vec3((1.0 - alpha) * c_fire.x, (1.0 - alpha) * c_fire.y, (1.0 - alpha) * c_fire.z);
	vec4 color = vec4(alpha * u_fogColor + modified_color, c_fire.a);

	if (c_fire.a < 0.4)
		discard;
	gl_FragColor = color;
}
