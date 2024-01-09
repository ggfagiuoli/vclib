$input v_pos
$input v_normal
$input v_color

#include <bgfx_shader.sh>

uniform vec4 cameraEyePos;
uniform vec4 lightDir;
uniform vec4 lightColor;

uniform vec4 meshColor;
uniform vec4 drawMode;

void main()
{
    int drawModeInt = floatBitsToInt(drawMode.x);

    // compute light
    float ambientStrength = 0.2;
    float specularStrength = 0.3;
    // ambient
    vec3 ambient = (ambientStrength * lightColor).xyz;
    // diffuse
    float diff = max(dot(v_normal, lightDir.xyz), 0);
    vec3 diffuse = diff * lightColor.xyz;
    // specular
    vec3 viewDir = normalize(cameraEyePos.xyz - v_pos);
    vec3 reflectDir = reflect(-lightDir.xyz, v_normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor.xyz;

    vec4 light = vec4(ambient + diffuse, 1.0);

    if (bool(drawModeInt & (1 << 14))) {
        gl_FragColor = light * v_color + vec4(specular, 0);
        return;
    }
    if (bool(drawModeInt & (1 << 15))) {
        gl_FragColor = light * meshColor + vec4(specular, 0);
        return;
    }

    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
