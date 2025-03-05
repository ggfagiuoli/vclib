/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#include <vclib/bgfx/programs/embedded_vf_programs/drawable_mesh_surface_smooth_color_mesh.h>

#include <shaders/drawable/drawable_mesh/surface/vs_surface.sc.glsl.bin.h>
#include <shaders/drawable/drawable_mesh/surface/fs_surface_smooth_color_mesh.sc.glsl.bin.h>

#include <shaders/drawable/drawable_mesh/surface/fs_surface_smooth_color_mesh.sc.essl.bin.h>
#include <shaders/drawable/drawable_mesh/surface/vs_surface.sc.essl.bin.h>

#include <shaders/drawable/drawable_mesh/surface/fs_surface_smooth_color_mesh.sc.spv.bin.h>
#include <shaders/drawable/drawable_mesh/surface/vs_surface.sc.spv.bin.h>
#ifdef _WIN32
#include <shaders/drawable/drawable_mesh/surface/fs_surface_smooth_color_mesh.sc.dx11.bin.h>
#include <shaders/drawable/drawable_mesh/surface/vs_surface.sc.dx11.bin.h>
#endif //  defined(_WIN32)
#ifdef __APPLE__
#include <shaders/drawable/drawable_mesh/surface/fs_surface._smooth_color_mesh.sc.mtl.bin.h>
#include <shaders/drawable/drawable_mesh/surface/vs_surface.sc.mtl.bin.h>
#endif // __APPLE__

namespace vcl {

bgfx::EmbeddedShader::Data VertFragLoader<
    VertFragProgram::DRAWABLE_MESH_SURFACE_SMOOTH_COLOR_MESH>::
    vertexShader(bgfx::RendererType::Enum type)
{
    switch (type) {
    case bgfx::RendererType::OpenGLES:
        return {type, vs_surface_essl, sizeof(vs_surface_essl)};
    case bgfx::RendererType::OpenGL:
        return {type, vs_surface_glsl, sizeof(vs_surface_glsl)};
    case bgfx::RendererType::Vulkan:
        return {type, vs_surface_spv, sizeof(vs_surface_spv)};
#ifdef _WIN32
    case bgfx::RendererType::Direct3D11:
        return {type, vs_surface_dx11, sizeof(vs_surface_dx11)};
    case bgfx::RendererType::Direct3D12:
#endif
#ifdef __APPLE__
    case bgfx::RendererType::Metal:
        return {type, vs_surface_mtl, sizeof(vs_surface_mtl)};
#endif
    default: return {type, nullptr, 0};
    }
}

bgfx::EmbeddedShader::Data VertFragLoader<
    VertFragProgram::DRAWABLE_MESH_SURFACE_SMOOTH_COLOR_MESH>::
    fragmentShader(bgfx::RendererType::Enum type)
{
    switch (type) {
    case bgfx::RendererType::OpenGLES:
        return {type, fs_surface_smooth_color_mesh_essl, sizeof(fs_surface_smooth_color_mesh_essl)};
    case bgfx::RendererType::OpenGL:
        return {type, fs_surface_smooth_color_mesh_glsl, sizeof(fs_surface_smooth_color_mesh_glsl)};
    case bgfx::RendererType::Vulkan:
        return {type, fs_surface_smooth_color_mesh_spv, sizeof(fs_surface_smooth_color_mesh_spv)};
#ifdef _WIN32
    case bgfx::RendererType::Direct3D11:
        return {type, fs_surface_smooth_color_mesh_dx11, sizeof(fs_surface_smooth_color_mesh_dx11)};
    case bgfx::RendererType::Direct3D12:
#endif
#ifdef __APPLE__
    case bgfx::RendererType::Metal:
        return {type, fs_surface_smooth_color_mesh_mtl, sizeof(fs_surface_smooth_color_mesh_mtl)};
#endif
    default: return {type, nullptr, 0};
    }
}

} // namespace vcl

