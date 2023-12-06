/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_EXT_VCG_IMPORT_H
#define VCL_EXT_VCG_IMPORT_H

#include <vclib/mesh/requirements.h>

#include <vcg/complex/complex.h>

namespace vcl::vc {

namespace detail {

using SupportedCustomComponentTypes =
    TypeWrapper<int, float, double>;

template<uint ELEM_ID, typename T, MeshConcept MeshType>
void addCustomComponentsIfTypeMatches(MeshType& mesh, auto& p)
{
    if (p._type == std::type_index(typeid(T))) {
        mesh.template addPerElementCustomComponent<ELEM_ID, T>(p._name);
    }
}

template<uint ELEM_ID, typename T, MeshConcept MeshType, typename VCGMeshType>
void addCustomComponentsOfTypeFromVCGMesh(MeshType& mesh, const VCGMeshType& vcgMesh)
{
    using CustomAttrSet = std::set<typename VCGMeshType::PointerToAttribute>;

    const CustomAttrSet* ps = nullptr;

    switch (ELEM_ID) {
        case VERTEX:
            ps = &vcgMesh.vert_attr;
            break;
        case FACE:
            ps = &vcgMesh.face_attr;
            break;
        default:
            break;
    }

    if (ps) {
        for (auto& p : *ps) {
            addCustomComponentsIfTypeMatches<ELEM_ID, T>(mesh, p);
        }
    }
}

template<
    uint ELEM_ID,
    typename T,
    ElementConcept ElementType,
    typename VCGMeshType>
void importCustomComponentsOfTypeFromVCGMesh(
    ElementType&       el,
    const VCGMeshType& vcgMesh,
    uint               elemIndex)
{
    if constexpr (ELEM_ID == VERTEX) {
        for (auto& p : vcgMesh.vert_attr) {
            if (p._type == std::type_index(typeid(T))) {
                const auto& h = vcg::tri::Allocator<VCGMeshType>::
                    template FindPerVertexAttribute<T>(vcgMesh, p._name);
                el.template customComponent<T>(p._name) = h[elemIndex];
            }
        }
    }

    if constexpr (ELEM_ID == FACE) {
        for (auto& p : vcgMesh.face_attr) {
            if (p._type == std::type_index(typeid(T))) {
                const auto& h = vcg::tri::Allocator<VCGMeshType>::
                    template FindPerFaceAttribute<T>(vcgMesh, p._name);
                el.template customComponent<T>(p._name) = h[elemIndex];
            }
        }
    }
}

} // namespace detail

template<MeshConcept MeshType, typename VCGMeshType>
void importMeshFromVCGMesh(MeshType& mesh, const VCGMeshType& vcgMesh)
{
    using CoordType = MeshType::VertexType::CoordType;

    mesh.reserveVertices(vcgMesh.VN());

    // add custom components that can be imported
    if (HasPerVertexCustomComponents<MeshType>) {
        // for each supported type, apply the lampda function that adds the
        // custom components of the type T that are in the vcgMesh
        vcl::ForEachType<detail::SupportedCustomComponentTypes>::apply(
            [&mesh, &vcgMesh]<typename T>() {
                detail::addCustomComponentsOfTypeFromVCGMesh<VERTEX, T>(
                    mesh, vcgMesh);
            });
    }

    // vertices
    for (uint i = 0; i < vcgMesh.vert.size(); i++) {
        if (!vcgMesh.vert[i].IsD()) {
            uint vi = mesh.addVertex(CoordType(
                vcgMesh.vert[i].P()[0],
                vcgMesh.vert[i].P()[1],
                vcgMesh.vert[i].P()[2]));

            auto& vertex = mesh.vertex(vi);

            // flags
            vertex.importFlagsFromVCGFormat(vcgMesh.vert[i].Flags());

            // normal
            if constexpr (HasPerVertexNormal<MeshType>) {
                using NormalType = MeshType::VertexType::NormalType;
                if (isPerVertexNormalAvailable(mesh)) {
                    if (vcg::tri::HasPerVertexNormal(vcgMesh)) {
                        vertex.normal() = NormalType(
                            vcgMesh.vert[i].N()[0],
                            vcgMesh.vert[i].N()[1],
                            vcgMesh.vert[i].N()[2]);
                    }
                }
            }

            // color
            if constexpr (HasPerVertexColor<MeshType>) {
                if (isPerVertexColorAvailable(mesh)) {
                    if (vcg::tri::HasPerVertexColor(vcgMesh)) {
                        vertex.color() = Color(
                            vcgMesh.vert[i].C()[0],
                            vcgMesh.vert[i].C()[1],
                            vcgMesh.vert[i].C()[2],
                            vcgMesh.vert[i].C()[3]);
                    }
                }
            }

            // quality
            if constexpr (HasPerVertexQuality<MeshType>) {
                if (isPerVertexQualityAvailable(mesh)) {
                    if (vcg::tri::HasPerVertexQuality(vcgMesh)) {
                        vertex.quality() = vcgMesh.vert[i].Q();
                    }
                }
            }

            // texcoord
            if constexpr (HasPerVertexTexCoord<MeshType>) {
                using TexCoordType = MeshType::VertexType::TexCoordType;
                if (isPerVertexTexCoordAvailable(mesh)) {
                    if (vcg::tri::HasPerVertexTexCoord(vcgMesh)) {
                        vertex.texCoord() = TexCoordType(
                            vcgMesh.vert[i].T().U(), vcgMesh.vert[i].T().V());
                    }
                }
            }

            // custom components
            if constexpr (HasPerVertexCustomComponents<MeshType>) {
                // for each supported type, apply the lampda function that adds
                // the custom components of the type T that are in the vcgMesh
                vcl::ForEachType<detail::SupportedCustomComponentTypes>::apply(
                    [&vertex, &vcgMesh, vi]<typename T>() {
                        detail::importCustomComponentsOfTypeFromVCGMesh<
                            VERTEX, T>(vertex, vcgMesh, vi);
                    });
            }
        }
    }

    // faces
    if constexpr (HasFaces<MeshType>) {
        using FaceType = MeshType::FaceType;

        // add custom components that can be imported
        if (HasPerFaceCustomComponents<MeshType>) {
            // for each supported type, apply the lampda function that adds the
            // custom components of the type T that are in the vcgMesh
            vcl::ForEachType<detail::SupportedCustomComponentTypes>::apply(
                [&mesh, &vcgMesh]<typename T>() {
                    detail::addCustomComponentsOfTypeFromVCGMesh<FACE, T>(
                        mesh, vcgMesh);
                });
        }

        for (uint i = 0; i < vcgMesh.face.size(); ++i) {
            if (!vcgMesh.face[i].IsD()) {
                uint fi = mesh.addFace();

                auto& face = mesh.face(fi);

                if constexpr (FaceType::VERTEX_NUMBER < 0) {
                    face.resizeVertices(3);
                }
                for (uint j = 0; j < 3; ++j) {
                    uint vi = vcg::tri::Index(vcgMesh, vcgMesh.face[i].V(j));
                    face.vertex(j) = &mesh.vertex(vi);
                }

                // flags
                face.importFlagsFromVCGFormat(vcgMesh.face[i].Flags());

                // normal
                if constexpr (HasPerFaceNormal<MeshType>) {
                    using NormalType = MeshType::FaceType::NormalType;
                    if (isPerFaceNormalAvailable(mesh)) {
                        if (vcg::tri::HasPerFaceNormal(vcgMesh)) {
                            face.normal() = NormalType(
                                vcgMesh.face[i].N()[0],
                                vcgMesh.face[i].N()[1],
                                vcgMesh.face[i].N()[2]);
                        }
                    }
                }

                // color
                if constexpr (HasPerFaceColor<MeshType>) {
                    if (isPerFaceColorAvailable(mesh)) {
                        if (vcg::tri::HasPerFaceColor(vcgMesh)) {
                            face.color() = Color(
                                vcgMesh.face[i].C()[0],
                                vcgMesh.face[i].C()[1],
                                vcgMesh.face[i].C()[2],
                                vcgMesh.face[i].C()[3]);
                        }
                    }
                }

                // quality
                if constexpr (HasPerFaceQuality<MeshType>) {
                    if (isPerFaceQualityAvailable(mesh)) {
                        if (vcg::tri::HasPerFaceQuality(vcgMesh)) {
                            face.quality() = vcgMesh.face[i].Q();
                        }
                    }
                }

                // wedge texcoords
                if constexpr (HasPerFaceWedgeTexCoords<MeshType>) {
                    using WTType = MeshType::FaceType::WedgeTexCoordType;
                    if (isPerFaceWedgeTexCoordsAvailable(mesh)) {
                        if (vcg::tri::HasPerWedgeTexCoord(vcgMesh)) {
                            face.textureIndex() =
                                vcgMesh.face[i].WT(0).N();
                            for (uint j = 0; j < 3; ++j) {
                                face.wedgeTexCoord(j) = WTType(
                                    vcgMesh.face[i].WT(j).U(),
                                    vcgMesh.face[i].WT(j).V());
                            }
                        }
                    }
                }

                // custom components
                if constexpr (HasPerVertexCustomComponents<MeshType>) {
                    // for each supported type, apply the lampda function that adds
                    // the custom components of the type T that are in the vcgMesh
                    vcl::ForEachType<detail::SupportedCustomComponentTypes>::apply(
                        [&face, &vcgMesh, fi]<typename T>() {
                            detail::importCustomComponentsOfTypeFromVCGMesh<
                                FACE, T>(face, vcgMesh, fi);
                        });
                }
            }
        }
    }

    if constexpr (HasTexturePaths<MeshType>) {
        for (const auto& s : vcgMesh.textures) {
            mesh.pushTexturePath(s);
        }
    }
}

template<MeshConcept MeshType, typename VCGMeshType>
MeshType meshFromVCGMesh(const VCGMeshType& vcgMesh)
{
    MeshType mesh;

    importMeshFromVCGMesh(mesh, vcgMesh);

    return mesh;
}

} // namespace vcl::vc

#endif // VCL_EXT_VCG_IMPORT_H
