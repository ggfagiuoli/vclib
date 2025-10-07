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

#include <vclib/bgfx/shaders_common.sh>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>

BUFFER_RO(positions, vec4, VCL_MRB_VERTEX_POSITION_STREAM); // coordinates (3 floats)
BUFFER_RO(indices, uint, 5);
BUFFER_RW(face_selected, uint, 6); // is face selected? 1 bit per face...

uniform vec4 u_selectionBox; // screen space
uniform vec4 u_workgroupSizeAndVertexCount; // despite the name, w should contain the face count (i.e. numTris)

// THE SELECTION IS CHECKED IN NDC SPACE. I decided for this because this way i only need the viewRect and the modelViewProj uniforms.
// Possibility: uniform containing selection box passed already in NDC space? It's probably doable

NUM_THREADS(1, 1, 1) // 1 'thread' per face, or 1 'thread' per 3 indices
void main()
{
    uint faceCount = floatBitsToUint(u_workgroupSizeAndVertexCount.w);
    uvec3 workGroupSize = uvec3(floatBitsToUint(u_workgroupSizeAndVertexCount.x), floatBitsToUint(u_workgroupSizeAndVertexCount.y), floatBitsToUint(u_workgroupSizeAndVertexCount.z));
    uint faceIndex = (gl_WorkGroupID.x + workGroupSize.x * gl_WorkGroupID.y + workGroupSize.x * workGroupSize.y * gl_WorkGroupID.z);
    uint indicesBaseIndex = 3 * faceIndex;
    uvec3 idcs = uvec3(indices[indicesBaseIndex], indices[indicesBaseIndex + 1], indices[indicesBaseIndex + 2]);

    if(faceIndex >= faceCount) {
        return;
    }

    float minX = u_selectionBox[0];
    float minY = u_selectionBox[1];
    float maxX = u_selectionBox[2];
    float maxY = u_selectionBox[3];

    vec3 maxNDC = vec3(
        (maxX - u_viewRect.x) / max(1, u_viewRect.z) * 2 - 1,
        1 - ((minY - u_viewRect.y) / max(1, u_viewRect.w)) * 2,
        1
    );
    vec3 minNDC = vec3(
        (minX - u_viewRect.x) / max(1, u_viewRect.z) * 2 - 1,
        1 - ((maxY - u_viewRect.y) / max(1, u_viewRect.w) * 2),
        0
    );

    mat3 poss = mat3(
        positions[(idcs[0] * 3) / 4][(idcs[0] * 3) % 4], positions[(idcs[0] * 3 + 1) / 4][(idcs[0] * 3 + 1) % 4], positions[(idcs[0] * 3 + 2) / 4][(idcs[0] * 3 + 2) % 4],
        positions[(idcs[1] * 3) / 4][(idcs[1] * 3) % 4], positions[(idcs[1] * 3 + 1) / 4][(idcs[1] * 3 + 1) % 4], positions[(idcs[1] * 3 + 2) / 4][(idcs[1] * 3 + 2) % 4],
        positions[(idcs[2] * 3) / 4][(idcs[2] * 3) % 4], positions[(idcs[2] * 3 + 1) / 4][(idcs[2] * 3 + 1) % 4], positions[(idcs[2] * 3 + 2) / 4][(idcs[2] * 3 + 2) % 4]
    );

    uint slctd = 1;
    for (uint i = 0; i < 3; i++) {
        vec4 pNDC = mul(u_modelViewProj, vec4(poss[i].xyz, 1));
        pNDC = pNDC / (pNDC.w == 0? 1 : pNDC.w);
        if (!(pNDC.x >= minNDC.x && pNDC.x <= maxNDC.x && pNDC.y >= minNDC.y && pNDC.y <= maxNDC.y && pNDC.z >= minNDC.z && pNDC.z <= maxNDC.z)) {
            slctd = 0;
            break;
        }
    }

    uint fBufferIndex = faceIndex/32;
    uint fBitOffset = 31-(faceIndex%32);
    uint fBitMask = 0x1 << fBitOffset;
    uint _useless;
    if (slctd == 1) {
        atomicFetchAndOr(face_selected[fBufferIndex], fBitMask, _useless);
    } else {
        atomicFetchAndAnd(face_selected[fBufferIndex], ~fBitMask, _useless);
    }
}