attribute vec3 a_posL;
uniform vec3 u_color;
uniform mat4 u_rot;
uniform mat4 u_vpMatrix;
void main()
{
vec4 posL = vec4(a_posL, 1.0);
gl_Position = u_vpMatrix * u_rot *  posL;
v_color = a_color;
}
 