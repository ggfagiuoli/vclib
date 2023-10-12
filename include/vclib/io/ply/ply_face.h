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

#ifndef VCL_IO_PLY_FACE_H
#define VCL_IO_PLY_FACE_H

#include <fstream>

#include <vclib/algorithms/polygon.h>
#include <vclib/exceptions/io_exceptions.h>
#include <vclib/mesh/requirements.h>
#include <vclib/misc/tokenizer.h>

#include "ply_header.h"
#include "../detail/io_read.h"
#include "../detail/io_write.h"

namespace vcl::io::ply {

namespace detail {

template<FaceMeshConcept MeshType, FaceConcept FaceType>
void saveFaceIndices(
    std::ofstream&           file,
    Property                 p,
    const MeshType&          m,
    const std::vector<uint>& vIndices,
    const FaceType&          f,
    bool                     bin)
{
    using VertexType = MeshType::VertexType;

    uint fsize = f.vertexNumber();
    io::detail::writeProperty(file, fsize, p.listSizeType, bin);
    for (const VertexType* v : f.vertices()) {
        io::detail::writeProperty(file, vIndices[m.index(v)], p.type, bin);
    }
}

template<FaceMeshConcept MeshType, FaceConcept FaceType>
void setFaceIndices(FaceType& f, MeshType& m, const std::vector<uint>& vids)
{
    bool splitFace = false;
    // we have a polygonal mesh
    if constexpr (FaceType::VERTEX_NUMBER < 0) {
        // need to resize the face to the right number of verts
        f.resizeVertices(vids.size());
    }
    else if (FaceType::VERTEX_NUMBER != vids.size()) {
        // we have faces with static sizes (triangles), but we are loading faces
        // with number of verts > 3. Need to split the face we are loading in n
        // faces!
        splitFace = true;
    }

    if (!splitFace) { // classic load, no split needed
        uint i = 0;
        for (auto& v : f.vertices()) {
            if (vids[i] >= m.vertexNumber()) {
                throw vcl::MalformedFileException(
                    "Bad vertex index for face " + std::to_string(i));
            }
            v = &m.vertex(vids[i]);
            i++;
        }
    }
    else { // split needed
        addTriangleFacesFromPolygon(m, f, vids);
    }
}

template<FaceMeshConcept MeshType, FaceConcept FaceType, typename Scalar>
void setFaceWedgeTexCoords(
    FaceType&                                     f,
    MeshType&                                     m,
    const std::vector<uint>&                      vids,
    const std::vector<std::pair<Scalar, Scalar>>& wedges)
{
    bool splitFace = false;
    if (FaceType::VERTEX_NUMBER > 0 && FaceType::VERTEX_NUMBER != wedges.size())
        splitFace = true;
    if (!splitFace) {
        for (uint i = 0; i < wedges.size(); ++i) {
            f.wedgeTexCoord(i).u() = wedges[i].first;
            f.wedgeTexCoord(i).v() = wedges[i].second;
        }
    }
    else { // the face has been splitted
        // for each triangle generated by the polygon
        for (uint ff = m.index(f); ff < m.faceNumber(); ++ff) {
            // for each vertex/wedge of the triangle
            for (uint i = 0; i < m.face(ff).vertexNumber(); ++i) {
                // get the position of this vertex in the original polygon
                uint vid = m.index(m.face(ff).vertex(i));
                auto it  = std::find(vids.begin(), vids.end(), vid);
                if (it == vids.end()) { // should never happen
                    // if this happens, it means that this triangle was not
                    // generated by the polygon
                    throw vcl::MalformedFileException(
                        "Bad vertex index for face " + std::to_string(ff));
                }
                // p is the position of the vertex in the polygon
                uint p = it - vids.begin();
                m.face(ff).wedgeTexCoord(i).u() = wedges[p].first;
                m.face(ff).wedgeTexCoord(i).v() = wedges[p].second;
            }
        }
    }
}

template<FaceMeshConcept MeshType, FaceConcept FaceType, typename Stream>
void loadFaceProperty(Stream& file, MeshType& mesh, FaceType& f, ply::Property p)
{
    bool              hasBeenRead = false;
    std::vector<uint> vids; // contains the vertex ids of the actual face
    if (p.name == ply::vertex_indices) { // loading vertex indices
        uint fSize = io::detail::readProperty<uint>(file, p.listSizeType);
        vids.resize(fSize);
        for (uint i = 0; i < fSize; ++i) {
            vids[i] = io::detail::readProperty<size_t>(file, p.type);
        }
        hasBeenRead = true;
        // will manage the case of loading a polygon in a triangle mesh
        setFaceIndices(f, mesh, vids);
    }
    if (p.name == ply::texcoord) { // loading wedge texcoords
        if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
            if (vcl::isPerFaceWedgeTexCoordsAvailable(mesh)) {
                using Scalar = FaceType::WedgeTexCoordType::ScalarType;
                uint uvSize =
                    io::detail::readProperty<uint>(file, p.listSizeType);
                uint fSize   = uvSize / 2;
                std::vector<std::pair<Scalar, Scalar>> wedges(fSize);
                for (uint i = 0; i < fSize; ++i) {
                    Scalar u = io::detail::readProperty<Scalar>(file, p.type);
                    Scalar v = io::detail::readProperty<Scalar>(file, p.type);
                    wedges[i].first  = u;
                    wedges[i].second = v;
                }
                hasBeenRead = true;
                setFaceWedgeTexCoords(f, mesh, vids, wedges);
            }
        }
    }
    // loading texture id associated to ALL the wedges
    if (p.name == ply::texnumber) {
        if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
            if (vcl::isPerFaceWedgeTexCoordsAvailable(mesh)) {
                uint n      = io::detail::readProperty<uint>(file, p.type);
                hasBeenRead = true;
                // in case the loaded polygon has been triangulated in the last
                // n triangles of mesh
                for (uint ff = mesh.index(f); ff < mesh.faceNumber(); ++ff) {
                    mesh.face(ff).textureIndex() = n;
                }
            }
        }
    }
    // loading one of the normal components
    if (p.name >= ply::nx && p.name <= ply::nz) {
        if constexpr (vcl::HasPerFaceNormal<MeshType>) {
            if (vcl::isPerFaceNormalAvailable(mesh)) {
                using Scalar = FaceType::NormalType::ScalarType;
                int    a     = p.name - ply::nx;
                Scalar n     = io::detail::readProperty<Scalar>(file, p.type);
                hasBeenRead  = true;
                // in case the loaded polygon has been triangulated in the last
                // n triangles of mesh
                for (uint ff = mesh.index(f); ff < mesh.faceNumber(); ++ff) {
                    mesh.face(ff).normal()[a] = n;
                }
            }
        }
    }
    // loading one of the color components
    if (p.name >= ply::red && p.name <= ply::alpha) {
        if constexpr (vcl::HasPerFaceColor<MeshType>) {
            if (vcl::isPerFaceColorAvailable(mesh)) {
                int           a = p.name - ply::red;
                unsigned char c =
                    io::detail::readProperty<unsigned char>(file, p.type);
                hasBeenRead = true;
                // in case the loaded polygon has been triangulated in the last
                // n triangles of mesh
                for (uint ff = mesh.index(f); ff < mesh.faceNumber(); ++ff) {
                    mesh.face(ff).color()[a] = c;
                }
            }
        }
    }
    if (p.name == ply::quality) { // loading the quality component
        if constexpr (vcl::HasPerFaceQuality<MeshType>) {
            using QualityType = FaceType::QualityType;
            if (vcl::isPerFaceQualityAvailable(mesh)) {
                QualityType s =
                    io::detail::readProperty<QualityType>(file, p.type);
                hasBeenRead   = true;
                // in case the loaded polygon has been triangulated in the last
                // n triangles of mesh
                for (uint ff = mesh.index(f); ff < mesh.faceNumber(); ++ff) {
                    mesh.face(ff).quality() = s;
                }
            }
        }
    }
    if (p.name == ply::unknown) {
        if constexpr (vcl::HasPerFaceCustomComponents<MeshType>) {
            if (mesh.hasPerFaceCustomComponent(p.unknownPropertyName)){
                io::detail::readCustomComponent(
                    file, f, p.unknownPropertyName, p.type);
                hasBeenRead = true;
            }
        }
    }
    // if nothing has been read, it means that there is some data we don't know
    // we still need to read and discard what we read
    if (!hasBeenRead) {
        if (p.list) {
            uint s = io::detail::readProperty<int>(file, p.listSizeType);
            for (uint i = 0; i < s; ++i)
                io::detail::readProperty<int>(file, p.type);
        }
        else {
            io::detail::readProperty<int>(file, p.type);
        }
    }
}

template<FaceConcept FaceType, MeshConcept MeshType>
void loadFaceTxt(
    std::ifstream& file,
    FaceType& f,
    MeshType& mesh,
    const std::list<ply::Property>& faceProperties)
{
    vcl::Tokenizer spaceTokenizer =
        vcl::io::detail::nextNonEmptyTokenizedLine(file);
    vcl::Tokenizer::iterator token = spaceTokenizer.begin();
    for (const ply::Property& p : faceProperties) {
        if (token == spaceTokenizer.end()) {
            throw vcl::MalformedFileException("Unexpected end of line.");
        }
        loadFaceProperty(token, mesh, f, p);
    }
}

template<FaceConcept FaceType, MeshConcept MeshType>
void loadFaceBin(
    std::ifstream& file,
    FaceType& f,
    MeshType& mesh,
    const std::list<ply::Property>& faceProperties)
{
    for (const ply::Property& p : faceProperties) {
        loadFaceProperty(file, mesh, f, p);
    }
}

} // namespace vcl::io::ply::detail

template<FaceMeshConcept MeshType>
void saveFaces(
    std::ofstream&   file,
    const PlyHeader& header,
    const MeshType&  mesh)
{
    using FaceType = MeshType::FaceType;

    bool bin = header.format() == ply::BINARY;

    // indices of vertices that do not consider deleted vertices
    std::vector<uint> vIndices = mesh.vertexCompactIndices();

    for (const FaceType& f : mesh.faces()) {
        for (const ply::Property& p : header.faceProperties()) {
            bool hasBeenWritten = false;
            if (p.name == ply::vertex_indices) {
                detail::saveFaceIndices(file, p, mesh, vIndices, f, bin);
                hasBeenWritten = true;
            }
            if (p.name >= ply::nx && p.name <= ply::nz) {
                if constexpr (vcl::HasPerFaceNormal<MeshType>) {
                    io::detail::writeProperty(
                        file, f.normal()[p.name - ply::nx], p.type, bin);
                    hasBeenWritten = true;
                }
            }
            if (p.name >= ply::red && p.name <= ply::alpha) {
                if constexpr (vcl::HasPerFaceColor<MeshType>) {
                    io::detail::writeProperty(
                        file, f.color()[p.name - ply::red], p.type, bin);
                    hasBeenWritten = true;
                }
            }
            if (p.name == ply::quality) {
                if constexpr (vcl::HasPerFaceQuality<MeshType>) {
                    io::detail::writeProperty(file, f.quality(), p.type, bin);
                    hasBeenWritten = true;
                }
            }
            if (p.name == ply::texcoord) {
                if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
                    io::detail::writeProperty(
                        file, f.vertexNumber() * 2, p.listSizeType, bin);
                    for (const auto& tc : f.wedgeTexCoords()) {
                        io::detail::writeProperty(file, tc.u(), p.type, bin);
                        io::detail::writeProperty(file, tc.v(), p.type, bin);
                    }
                    hasBeenWritten = true;
                }
            }
            if (p.name == ply::texnumber) {
                if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
                    io::detail::writeProperty(
                        file, f.textureIndex(), p.type, bin);
                    hasBeenWritten = true;
                }
            }
            if (p.name == ply::unknown) {
                if constexpr (vcl::HasPerFaceCustomComponents<MeshType>) {
                    if (mesh.hasPerFaceCustomComponent(p.unknownPropertyName)) {
                        io::detail::writeCustomComponent(
                            file, f, p.unknownPropertyName, p.type, bin);
                        hasBeenWritten = true;
                    }
                }
            }
            if (!hasBeenWritten) {
                // be sure to write something if the header declares some
                // property that is not in the mesh
                io::detail::writeProperty(file, 0, p.type, bin);
            }
        }
        if (!bin)
            file << std::endl;
    }
}

template<FaceMeshConcept MeshType>
void loadFaces(std::ifstream& file, const PlyHeader& header, MeshType& mesh)
{
    using FaceType = MeshType::FaceType;
    mesh.reserveFaces(header.numberFaces());
    for (uint fid = 0; fid < header.numberFaces(); ++fid) {
        uint      ffid = mesh.addFace();
        FaceType& f    = mesh.face(ffid);
        if (header.format() == ply::ASCII) {
            detail::loadFaceTxt(file, f, mesh, header.faceProperties());
        }
        else {
            detail::loadFaceBin(file, f, mesh, header.faceProperties());
        }
    }

}

} // namespace vcl::io::ply

#endif // VCL_IO_PLY_FACE_H
