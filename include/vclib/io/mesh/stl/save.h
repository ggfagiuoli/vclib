/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_IO_STL_SAVE_H
#define VCL_IO_STL_SAVE_H

#include <vclib/algorithms/polygon.h>
#include <vclib/io/write.h>
#include <vclib/mesh/utils/mesh_info.h>
#include <vclib/misc/logger.h>

namespace vcl {

namespace detail {

void writeStlHeader(std::ostream& fp, bool magicsMode, bool binary)
{
    std::string header = "solid STL generated by VCLib";
    if (binary) {
        if (magicsMode) {
            std::string p;
            for (uint i = 0; i < 3; i++)
                p.push_back((char) 0x7f);
            header = "COLOR=" + p + " MATERIAL=" + p + " " + p + " " + p;
        }
        while (header.size() < 80)
            header.push_back(' ');
    }

    fp << header;
    if (!binary)
        fp << std::endl;
}

template<Point3Concept PointType, Point3Concept NormalType>
void writeSTLTriangle(
    std::ostream&     fp,
    const PointType&  p0,
    const PointType&  p1,
    const PointType&  p2,
    const NormalType& n,
    uint              attributes,
    bool              binary)
{
    if (binary) {
        for (uint i = 0; i < 3; ++i)
            io::writeFloat(fp, n[i]);

        for (uint i = 0; i < 3; ++i)
            io::writeFloat(fp, p0[i]);

        for (uint i = 0; i < 3; ++i)
            io::writeFloat(fp, p1[i]);

        for (uint i = 0; i < 3; ++i)
            io::writeFloat(fp, p2[i]);

        io::writeUShort(fp, attributes);
    }
    else {
        fp << "  facet normal " << n.x() << " " << n.y() << " " << n.z()
           << std::endl;
        fp << "    outer loop" << std::endl;

        fp << "      vertex " << p0.x() << " " << p0.y() << " " << p0.z()
           << std::endl;
        fp << "      vertex " << p1.x() << " " << p1.y() << " " << p1.z()
           << std::endl;
        fp << "      vertex " << p2.x() << " " << p2.y() << " " << p2.z()
           << std::endl;

        fp << "    endloop" << std::endl;
        fp << "  endfacet" << std::endl;
    }
}

} // namespace detail

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveStl(
    const MeshType& m,
    std::ostream&   fp,
    const MeshInfo& info,
    LogType&        log        = nullLogger,
    bool            binary     = false,
    bool            magicsMode = false)
{
    MeshInfo meshInfo(m);

    // make sure that the given info contains only components that are actually
    // available in the mesh. meshInfo will contain the intersection between the
    // components that the user wants to save and the components that are
    // available in the mesh.
    meshInfo = info.intersect(meshInfo);

    log.log(0, "Saving STL file");

    detail::writeStlHeader(fp, magicsMode, binary);

    if constexpr (HasFaces<MeshType>) {
        using FaceType = MeshType::FaceType;

        if (binary) {
            io::writeInt(fp, m.faceNumber());
        }

        log.startProgress("Loading STL file", m.faceNumber());

        for (const FaceType& f : m.faces()) {
            // For each triangle write the normal, the three coords and a short
            auto n = vcl::faceNormal(f);

            unsigned short attributes = 0;

            if constexpr (HasPerFaceColor<MeshType>) {
                if (meshInfo.hasFaceColors()) {
                    if (magicsMode)
                        attributes = 32768 | f.color().bgr5();
                    else
                        attributes = 32768 | f.color().rgb5();
                }
            }

            if (f.vertexNumber() == 3) {
                detail::writeSTLTriangle(
                    fp,
                    f.vertex(0)->coord(),
                    f.vertex(1)->coord(),
                    f.vertex(2)->coord(),
                    n,
                    attributes,
                    binary);
            }
            else {
                std::vector<uint> tris = vcl::earCut(f);
                for (uint i = 0; i < tris.size(); i += 3) {
                    detail::writeSTLTriangle(
                        fp,
                        f.vertex(tris[i])->coord(),
                        f.vertex(tris[i + 1])->coord(),
                        f.vertex(tris[i + 2])->coord(),
                        n,
                        attributes,
                        binary);
                }
            }

            log.progress(m.index(f));
        }

        log.endProgress();
    }

    if (!binary) {
        fp << "endsolid VCLib" << std::endl;
    }
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveStl(
    const MeshType& m,
    std::ostream&   fp,
    LogType&        log        = nullLogger,
    bool            binary     = false,
    bool            magicsMode = false)
{
    MeshInfo info(m);
    saveStl(m, fp, info, log, binary, magicsMode);
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveStl(
    const MeshType& m,
    std::ostream&   fp,
    const MeshInfo& info,
    bool            binary,
    bool            magicsMode = false,
    LogType&        log        = nullLogger)
{
    saveStl(m, fp, info, log, binary, magicsMode);
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveStl(
    const MeshType& m,
    std::ostream&   fp,
    bool            binary,
    bool            magicsMode = false,
    LogType&        log        = nullLogger)
{
    MeshInfo info(m);
    saveStl(m, fp, info, log, binary, magicsMode);
}

/**
 * @brief saveStl
 * @param m
 * @param filename
 * @param info
 * @param[in] magicsMode: indicates whether the STL file should be saved in
 * Magics mode or not. Magics mode is a specific file format used by the Magics
 * software, which includes more information than the standard STL format, like
 * face colors. The magicsMode works only when binary flag is set to `true`.
 * @param binary
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveStl(
    const MeshType&    m,
    const std::string& filename,
    const MeshInfo&    info,
    LogType&           log        = nullLogger,
    bool               binary     = true,
    bool               magicsMode = false)
{
    std::ofstream fp = openOutputFileStream(filename, "stl");

    saveStl(m, fp, info, log, binary, false);
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveStl(
    const MeshType&    m,
    const std::string& filename,
    const MeshInfo&    info,
    bool               binary,
    bool               magicsMode = false,
    LogType&           log        = nullLogger)
{
    saveStl(m, filename, info, log, binary, magicsMode);
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveStl(
    const MeshType&    m,
    const std::string& filename,
    bool               binary,
    bool               magicsMode = false,
    LogType&           log        = nullLogger)
{
    MeshInfo info(m);
    saveStl(m, filename, info, log, binary, magicsMode);
}

/**
 * @brief saveStl
 * @param m
 * @param filename
 * @param[in] magicsMode: indicates whether the STL file should be saved in
 * Magics mode or not. Magics mode is a specific file format used by the Magics
 * software, which includes more information than the standard STL format, like
 * face colors. The magicsMode works only when binary flag is set to `true`.
 * @param binary
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveStl(
    const MeshType&    m,
    const std::string& filename,
    LogType&           log        = nullLogger,
    bool               binary     = true,
    bool               magicsMode = false)
{
    MeshInfo info(m);
    saveStl(m, filename, info, log, binary, magicsMode);
}

} // namespace vcl

#endif // VCL_IO_STL_SAVE_H
