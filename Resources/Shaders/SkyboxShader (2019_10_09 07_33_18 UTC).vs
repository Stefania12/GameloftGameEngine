attribute vec3 a_posL;
uniform mat4 u_mvpMatrix;
varying vec3 v_coord;
void main()
{
vec4 posL = vec4(a_posL, 1.0);
gl_Position = u_mvpMatrix * posL;
v_coord = a_posL;
}
 