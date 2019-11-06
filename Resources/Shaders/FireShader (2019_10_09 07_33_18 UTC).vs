attribute vec3 a_posL;
uniform mat4 u_mvpMatrix;
attribute vec2 a_uv;
varying vec2 v_uv;

varying vec4 v_position;
uniform mat4 u_modelMatrix;
void main()
{
vec4 posL = vec4(a_posL, 1.0);
gl_Position = u_mvpMatrix * posL;
v_uv=a_uv;
v_position = u_modelMatrix * posL;
}
 