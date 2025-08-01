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

#ifndef VCL_IO_MESH_LOAD_MESH_H
#define VCL_IO_MESH_LOAD_MESH_H

#include "obj/load.h"
#include "off/load.h"
#include "ply/load.h"
#include "stl/load.h"

#ifdef VCLIB_WITH_TINYGLTF
#include "gltf/load.h"
#endif

#include "capability.h"

/**
 * @defgroup load_mesh Load mesh functions
 * @ingroup io_mesh
 *
 * @brief List of functions that allow to load from file an input Mesh.
 */

namespace vcl {

/**
 * @brief Returns the set of mesh formats supported for loading a single Mesh
 * from file.
 *
 * The set contains all the mesh formats that can be loaded using all the
 * external libraries compiled with VCLib.
 *
 * @return A set of mesh formats supported for loading.
 *
 * @ingroup load_mesh
 */
inline std::set<FileFormat> loadMeshFormats()
{
    std::set<FileFormat> ff;

    ff.insert(objFileFormat());
    ff.insert(offFileFormat());
    ff.insert(plyFileFormat());
    ff.insert(stlFileFormat());

#ifdef VCLIB_WITH_TINYGLTF
    ff.insert(gltfFileFormat());
#endif

    return ff;
}

/**
 * @brief Loads a mesh from a file with the given filename and stores it in the
 * given mesh object. Checks automatically the file format to load from the
 * given filename.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[out] m: The mesh object in which to store the loaded mesh.
 * @param[in] filename: The filename of the file containing the mesh data.
 * @param[out] loadedInfo: Information about the mesh components that have been
 * loaded from the file.
 * @param[in] settings: settings for loading the file.
 * @param[in, out] log: The logger object to use for logging messages during
 * loading. Default is the nullLogger object.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadMesh(
    MeshType&           m,
    const std::string&  filename,
    MeshInfo&           loadedInfo,
    const LoadSettings& settings,
    LogType&            log = nullLogger)
{
    FileFormat ff = FileInfo::fileFormat(filename);

    loadedInfo.clear();

    if (ff == objFileFormat()) {
        loadObj(m, filename, loadedInfo, settings, log);
    }
    else if (ff == offFileFormat()) {
        loadOff(m, filename, loadedInfo, settings, log);
    }
    else if (ff == plyFileFormat()) {
        loadPly(m, filename, loadedInfo, settings, log);
    }
    else if (ff == stlFileFormat()) {
        loadStl(m, filename, loadedInfo, settings, log);
    }
#ifdef VCLIB_WITH_TINYGLTF
    else if (ff == gltfFileFormat()) {
        loadGltf(m, filename, loadedInfo, settings, log);
    }
#endif
    else {
        throw UnknownFileFormatException(ff.extensions().front());
    }
}

/**
 * @brief Loads a mesh from a file with the given filename and stores it in the
 * given mesh object. Checks automatically the file format to load from the
 * given filename.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[out] m: The mesh object in which to store the loaded mesh.
 * @param[in] filename: The filename of the file containing the mesh data.
 * @param[out] loadedInfo: Information about the mesh components that have been
 * loaded from the file.
 * @param[in, out] log: The logger object to use for logging messages during
 * loading. Default is the nullLogger object.
 * @param[in] settings: settings for loading the file.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadMesh(
    MeshType&           m,
    const std::string&  filename,
    MeshInfo&           loadedInfo,
    LogType&            log      = nullLogger,
    const LoadSettings& settings = LoadSettings())
{
    loadMesh(m, filename, loadedInfo, settings, log);
}

/**
 * @brief Loads a mesh from a file with the given filename and stores it in the
 * given mesh object. Checks automatically the file format to load from the
 * given filename.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[out] m: The mesh object in which to store the loaded mesh.
 * @param[in] filename: The filename of the file containing the mesh data.
 * @param[in] settings: settings for loading the file.
 * @param[in, out] log: The logger object to use for logging messages during
 * loading. Default is the nullLogger object.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadMesh(
    MeshType&           m,
    const std::string&  filename,
    const LoadSettings& settings,
    LogType&            log = nullLogger)
{
    MeshInfo loadedInfo;
    loadMesh(m, filename, loadedInfo, settings, log);
}

/**
 * @brief Loads a mesh from a file with the given filename and stores it in the
 * given mesh object. Checks automatically the file format to load from the
 * given filename.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[out] m: The mesh object in which to store the loaded mesh.
 * @param[in] filename: The filename of the file containing the mesh data.
 * @param[in] settings: settings for loading the file.
 * @param[in, out] log: The logger object to use for logging messages during
 * loading. Default is the nullLogger object.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadMesh(
    MeshType&           m,
    const std::string&  filename,
    LogType&            log      = nullLogger,
    const LoadSettings& settings = LoadSettings())
{
    MeshInfo loadedInfo;
    loadMesh(m, filename, loadedInfo, settings, log);
}

/**
 * @brief Loads a mesh from a file with the given filename and stores it in the
 * returned mesh object. Checks automatically the file format to load from the
 * given filename.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] filename: The filename of the file containing the mesh data.
 * @param[out] loadedInfo: Information about the mesh components that have been
 * loaded from the file.
 * @param[in, out] log: The logger object to use for logging messages during
 * loading. Default is the nullLogger object.
 * @param[in] settings: settings for loading the file.
 * @return The mesh object containing the loaded mesh.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType loadMesh(
    const std::string&  filename,
    MeshInfo&           loadedInfo,
    const LoadSettings& settings,
    LogType&            log = nullLogger)
{
    MeshType m;
    loadMesh(m, filename, loadedInfo, settings, log);
    return m;
}

/**
 * @brief Loads a mesh from a file with the given filename and stores it in the
 * returned mesh object. Checks automatically the file format to load from the
 * given filename.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] filename: The filename of the file containing the mesh data.
 * @param[out] loadedInfo: Information about the mesh components that have been
 * loaded from the file.
 * @param[in, out] log: The logger object to use for logging messages during
 * loading. Default is the nullLogger object.
 * @param[in] settings: settings for loading the file.
 * @return The mesh object containing the loaded mesh.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType loadMesh(
    const std::string&  filename,
    MeshInfo&           loadedInfo,
    LogType&            log      = nullLogger,
    const LoadSettings& settings = LoadSettings())
{
    MeshType m;
    loadMesh(m, filename, loadedInfo, settings, log);
    return m;
}

/**
 * @brief Loads a mesh from a file with the given filename and stores it in the
 * returned mesh object. Checks automatically the file format to load from the
 * given filename.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] filename: The filename of the file containing the mesh data.
 * @param[in] settings: settings for loading the file.
 * @param[in, out] log: The logger object to use for logging messages during
 * loading. Default is the nullLogger object.
 * @return The mesh object containing the loaded mesh.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType loadMesh(
    const std::string&  filename,
    const LoadSettings& settings,
    LogType&            log = nullLogger)
{
    MeshInfo loadedInfo;
    return loadMesh<MeshType>(filename, loadedInfo, settings, log);
}

/**
 * @brief Loads a mesh from a file with the given filename and stores it in the
 * returned mesh object. Checks automatically the file format to load from the
 * given filename.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] filename: The filename of the file containing the mesh data.
 * @param[in, out] log: The logger object to use for logging messages during
 * loading. Default is the nullLogger object.
 * @param[in] settings: settings for loading the file.
 * @return The mesh object containing the loaded mesh.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType loadMesh(
    const std::string&  filename,
    LogType&            log      = nullLogger,
    const LoadSettings& settings = LoadSettings())
{
    MeshInfo loadedInfo;
    return loadMesh<MeshType>(filename, loadedInfo, settings, log);
}

} // namespace vcl

#endif // VCL_IO_MESH_LOAD_MESH_H
