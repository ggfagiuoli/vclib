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

#ifndef VCL_LOAD_SAVE_PLY_DETAIL_FACE_H
#define VCL_LOAD_SAVE_PLY_DETAIL_FACE_H

#include "header.h"

#include <vclib/algorithms/mesh/face_topology.h>
#include <vclib/exceptions/io.h>
#include <vclib/io/file_type.h>
#include <vclib/io/read.h>
#include <vclib/io/write.h>
#include <vclib/mesh/requirements.h>
#include <vclib/misc/tokenizer.h>

namespace vcl::detail {

template<FaceMeshConcept MeshType, FaceConcept FaceType>
void writePlyFaceIndices(
    std::ostream&            file,
    PlyProperty              p,
    const MeshType&          m,
    const std::vector<uint>& vIndices,
    const FaceType&          f,
    FileType                 format)
{
    using VertexType = MeshType::VertexType;

    uint fsize = f.vertexNumber();
    io::writeProperty(file, fsize, p.listSizeType, format);
    for (const VertexType* v : f.vertices()) {
        io::writeProperty(file, vIndices[m.index(v)], p.type, format);
    }
}

template<FaceMeshConcept MeshType, FaceConcept FaceType>
void setPlyFaceIndices(FaceType& f, MeshType& m, const std::vector<uint>& vids)
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
        for (uint i = 0; i < f.vertexNumber(); ++i) {
            if (vids[i] >= m.vertexNumber()) {
                throw MalformedFileException(
                    "Bad vertex index for face " + std::to_string(i));
            }
            f.setVertex(i, vids[i]);
        }
    }
    else { // split needed
        addTriangleFacesFromPolygon(m, f, vids);
    }
}

template<FaceMeshConcept MeshType, FaceConcept FaceType, typename Scalar>
void setPlyFaceWedgeTexCoords(
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
                    throw MalformedFileException(
                        "Bad vertex index for face " + std::to_string(ff));
                }
                // p is the position of the vertex in the polygon
                uint p                          = it - vids.begin();
                m.face(ff).wedgeTexCoord(i).u() = wedges[p].first;
                m.face(ff).wedgeTexCoord(i).v() = wedges[p].second;
            }
        }
    }
}

template<FaceMeshConcept MeshType, FaceConcept FaceType, typename Stream>
void readPlyFaceProperty(
    Stream&     file,
    MeshType&   mesh,
    FaceType&   f,
    PlyProperty p,
    MeshInfo&   loadedInfo,
    std::endian end = std::endian::little)
{
    bool              hasBeenRead = false;
    std::vector<uint> vids; // contains the vertex ids of the actual face
    if (p.name == ply::vertex_indices) { // loading vertex indices
        uint fSize = io::readPrimitiveType<uint>(file, p.listSizeType, end);
        loadedInfo.updateMeshType(fSize);
        vids.resize(fSize);
        for (uint i = 0; i < fSize; ++i) {
            vids[i] = io::readPrimitiveType<size_t>(file, p.type, end);
        }
        hasBeenRead = true;
        // will manage the case of loading a polygon in a triangle mesh
        setPlyFaceIndices(f, mesh, vids);
    }
    if (p.name == ply::texcoord) { // loading wedge texcoords
        if constexpr (HasPerFaceWedgeTexCoords<MeshType>) {
            if (isPerFaceWedgeTexCoordsAvailable(mesh)) {
                using Scalar = FaceType::WedgeTexCoordType::ScalarType;
                uint uvSize =
                    io::readPrimitiveType<uint>(file, p.listSizeType, end);
                uint                                   fSize = uvSize / 2;
                std::vector<std::pair<Scalar, Scalar>> wedges(fSize);
                for (uint i = 0; i < fSize; ++i) {
                    Scalar u = io::readPrimitiveType<Scalar>(file, p.type, end);
                    Scalar v = io::readPrimitiveType<Scalar>(file, p.type, end);
                    wedges[i].first  = u;
                    wedges[i].second = v;
                }
                hasBeenRead = true;
                setPlyFaceWedgeTexCoords(f, mesh, vids, wedges);
            }
        }
    }
    // loading texture id associated to ALL the wedges
    if (p.name == ply::texnumber) {
        if constexpr (HasPerFaceWedgeTexCoords<MeshType>) {
            if (isPerFaceWedgeTexCoordsAvailable(mesh)) {
                uint n      = io::readPrimitiveType<uint>(file, p.type, end);
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
        if constexpr (HasPerFaceNormal<MeshType>) {
            if (isPerFaceNormalAvailable(mesh)) {
                using Scalar = FaceType::NormalType::ScalarType;
                int    a     = p.name - ply::nx;
                Scalar n     = io::readPrimitiveType<Scalar>(file, p.type, end);
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
        if constexpr (HasPerFaceColor<MeshType>) {
            if (isPerFaceColorAvailable(mesh)) {
                int           a = p.name - ply::red;
                unsigned char c =
                    io::readPrimitiveType<unsigned char>(file, p.type, end);
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
        if constexpr (HasPerFaceQuality<MeshType>) {
            using QualityType = FaceType::QualityType;
            if (isPerFaceQualityAvailable(mesh)) {
                QualityType s =
                    io::readPrimitiveType<QualityType>(file, p.type, end);
                hasBeenRead = true;
                // in case the loaded polygon has been triangulated in the last
                // n triangles of mesh
                for (uint ff = mesh.index(f); ff < mesh.faceNumber(); ++ff) {
                    mesh.face(ff).quality() = s;
                }
            }
        }
    }
    if (p.name == ply::unknown) {
        if constexpr (HasPerFaceCustomComponents<MeshType>) {
            if (mesh.hasPerFaceCustomComponent(p.unknownPropertyName)) {
                io::readCustomComponent(
                    file, f, p.unknownPropertyName, p.type, end);
                hasBeenRead = true;
            }
        }
    }
    // if nothing has been read, it means that there is some data we don't know
    // we still need to read and discard what we read
    if (!hasBeenRead) {
        if (p.list) {
            uint s = io::readPrimitiveType<int>(file, p.listSizeType, end);
            for (uint i = 0; i < s; ++i)
                io::readPrimitiveType<int>(file, p.type, end);
        }
        else {
            io::readPrimitiveType<int>(file, p.type, end);
        }
    }
}

template<FaceConcept FaceType, MeshConcept MeshType>
void readPlyFaceTxt(
    std::istream&                 file,
    FaceType&                     f,
    MeshType&                     mesh,
    MeshInfo&                     loadedInfo,
    const std::list<PlyProperty>& faceProperties)
{
    Tokenizer           spaceTokenizer = readAndTokenizeNextNonEmptyLine(file);
    Tokenizer::iterator token          = spaceTokenizer.begin();
    for (const PlyProperty& p : faceProperties) {
        if (token == spaceTokenizer.end()) {
            throw MalformedFileException("Unexpected end of line.");
        }
        readPlyFaceProperty(token, mesh, f, p, loadedInfo);
    }
}

template<FaceConcept FaceType, MeshConcept MeshType>
void readPlyFaceBin(
    std::istream&                 file,
    FaceType&                     f,
    MeshType&                     mesh,
    MeshInfo&                     loadedInfo,
    const std::list<PlyProperty>& faceProperties,
    std::endian                   end)
{
    for (const PlyProperty& p : faceProperties) {
        readPlyFaceProperty(file, mesh, f, p, loadedInfo, end);
    }
}

template<FaceMeshConcept MeshType>
void writePlyFaces(
    std::ostream&    file,
    const PlyHeader& header,
    const MeshType&  mesh)
{
    using FaceType = MeshType::FaceType;

    FileType format;
    if (header.format() == ply::ASCII) {
        format.isBinary = false;
    }
    else if (header.format() == ply::BINARY_BIG_ENDIAN) {
        format.endian = std::endian::big;
    }

    // indices of vertices that do not consider deleted vertices
    std::vector<uint> vIndices = mesh.vertexCompactIndices();

    for (const FaceType& f : mesh.faces()) {
        for (const PlyProperty& p : header.faceProperties()) {
            bool hasBeenWritten = false;
            if (p.name == ply::vertex_indices) {
                detail::writePlyFaceIndices(file, p, mesh, vIndices, f, format);
                hasBeenWritten = true;
            }
            if (p.name >= ply::nx && p.name <= ply::nz) {
                if constexpr (HasPerFaceNormal<MeshType>) {
                    io::writeProperty(
                        file, f.normal()[p.name - ply::nx], p.type, format);
                    hasBeenWritten = true;
                }
            }
            if (p.name >= ply::red && p.name <= ply::alpha) {
                if constexpr (HasPerFaceColor<MeshType>) {
                    io::writeProperty(
                        file, f.color()[p.name - ply::red], p.type, format);
                    hasBeenWritten = true;
                }
            }
            if (p.name == ply::quality) {
                if constexpr (HasPerFaceQuality<MeshType>) {
                    io::writeProperty(file, f.quality(), p.type, format);
                    hasBeenWritten = true;
                }
            }
            if (p.name == ply::texcoord) {
                if constexpr (HasPerFaceWedgeTexCoords<MeshType>) {
                    io::writeProperty(
                        file, f.vertexNumber() * 2, p.listSizeType, format);
                    for (const auto& tc : f.wedgeTexCoords()) {
                        io::writeProperty(file, tc.u(), p.type, format);
                        io::writeProperty(file, tc.v(), p.type, format);
                    }
                    hasBeenWritten = true;
                }
            }
            if (p.name == ply::texnumber) {
                if constexpr (HasPerFaceWedgeTexCoords<MeshType>) {
                    io::writeProperty(file, f.textureIndex(), p.type, format);
                    hasBeenWritten = true;
                }
            }
            if (p.name == ply::unknown) {
                if constexpr (HasPerFaceCustomComponents<MeshType>) {
                    if (mesh.hasPerFaceCustomComponent(p.unknownPropertyName)) {
                        io::writeCustomComponent(
                            file, f, p.unknownPropertyName, p.type, format);
                        hasBeenWritten = true;
                    }
                }
            }
            if (!hasBeenWritten) {
                // be sure to write something if the header declares some
                // property that is not in the mesh
                io::writeProperty(file, 0, p.type, format);
            }
        }
        if (!format.isBinary)
            file << std::endl;
    }
}

template<FaceMeshConcept MeshType, LoggerConcept LogType>
void readPlyFaces(
    std::istream&    file,
    const PlyHeader& header,
    MeshType&        mesh,
    MeshInfo&        loadedInfo,
    LogType&         log)
{
    using FaceType = MeshType::FaceType;
    mesh.reserveFaces(header.numberFaces());

    log.startProgress("Reading faces", header.numberFaces());

    for (uint fid = 0; fid < header.numberFaces(); ++fid) {
        uint      ffid = mesh.addFace();
        FaceType& f    = mesh.face(ffid);
        if (header.format() == ply::ASCII) {
            detail::readPlyFaceTxt(
                file, f, mesh, loadedInfo, header.faceProperties());
        }
        else {
            std::endian end = header.format() == ply::BINARY_BIG_ENDIAN ?
                                  std::endian::big :
                                  std::endian::little;
            detail::readPlyFaceBin(
                file, f, mesh, loadedInfo, header.faceProperties(), end);
        }

        log.progress(fid);
    }

    log.endProgress();
}

} // namespace vcl::detail

#endif // VCL_LOAD_SAVE_PLY_DETAIL_FACE_H
