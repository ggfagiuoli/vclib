#ifndef VCL_EXT_BGFX_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_SH
#define VCL_EXT_BGFX_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_SH

uniform vec4 u_mrsPack;

#define u_primitiveFloat u_mrsPack.x
#define u_drawModeFloat u_mrsPack.y

#endif // VCL_EXT_BGFX_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_SH
