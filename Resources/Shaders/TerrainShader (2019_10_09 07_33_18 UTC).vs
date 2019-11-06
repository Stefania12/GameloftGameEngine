attribute vec3 a_posL;
uniform mat4 u_mvpMatrix;
uniform mat4 u_modelMatrix;
uniform vec3 u_height;
uniform sampler2D u_texture3;
uniform vec2 u_translation;

attribute vec2 a_uv;
varying vec2 v_uv;

attribute vec2 a_mini_uv;
varying vec2 v_mini_uv;

varying vec4 v_position;

attribute vec3 a_norm;
varying vec3 v_norm;

void main()
{
vec3 pos = a_posL;
vec3 heights = texture2D(u_texture3, a_uv + u_translation).rgb;
pos.y = heights.r * u_height.x + heights.g * u_height.y + heights.b * u_height.z;
vec4 posL = vec4(pos, 1.0);

gl_Position = u_mvpMatrix * posL;
v_uv = a_uv;
v_mini_uv = a_mini_uv;
v_position = u_modelMatrix * posL;

vec4 N = u_modelMatrix * vec4(a_norm, 0.0);
v_norm = vec3(N.x, N.y, N.z);
}
 