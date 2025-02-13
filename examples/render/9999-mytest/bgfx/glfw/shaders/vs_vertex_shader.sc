$input a_position
$input a_color0

$output v_color

#include <bgfx_shader.sh>

void main()
{
    gl_Position = u_proj * u_view * u_model * vec4(a_position.x, a_position.y, a_position.z, 1.0);
    v_color = a_color0;
}
