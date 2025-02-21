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

#ifndef VCL_PROCESSING_ACTION_INTERFACES_IO_MESH_ACTION_H
#define VCL_PROCESSING_ACTION_INTERFACES_IO_MESH_ACTION_H

#include <vclib/processing/action_interfaces/action.h>
#include <vclib/processing/parameter_vector.h>

#include <vclib/algorithms/mesh/update.h>
#include <vclib/io/file_format.h>
#include <vclib/io/file_info.h>
#include <vclib/space/complex/mesh_info.h>

namespace vcl::proc {

template<MeshConcept MeshType>
class IOMeshAction : public Action
{
public:
    enum class IOSupport {
        LOAD,
        SAVE,
        BOTH
    };

    /* ******************************************************************** *
     * Member functions that must/may be implemented by the derived classes *
     * ******************************************************************** */

    // From Action class

    [[nodiscard]] virtual std::shared_ptr<Action> clone() const = 0;

    virtual std::string name() const = 0;

    /**
     * @brief Returns the type of support for input/output operations.
     *
     * Possible values are:
     * - LOAD: the action supports only loading meshes;
     * - SAVE: the action supports only saving meshes;
     * - BOTH: the action supports both loading and saving meshes.
     *
     * @return the type of support for input/output operations
     */
    virtual IOSupport ioSupport() const = 0;

    /**
     * @brief Returns a vector of file formats and their capabilities.
     *
     * Each file format is defined by a list of extensions (all the possible
     * extensions that a file format could have) and a description.
     *
     * The capabilities of the file format are defined by the MeshInfo
     * class, and they are used to determine the information that can be
     * stored or loaded from the file.
     *
     * @return the vectir of file formats supported by the action and their
     * capabilities
     */
    virtual std::vector<std::pair<FileFormat, MeshInfo>> supportedFormats()
        const = 0;

    /**
     * @brief Returns the parameters to load the mesh.
     *
     * By default, the load function has no parameters.
     *
     * You should override this method if your load function requires
     * parameters.
     *
     * @return The parameters for loading the mesh.
     */
    virtual ParameterVector parametersLoad() const { return ParameterVector(); }

    /**
     * @brief Returns the parameters to save the mesh.
     *
     * By default, the save function has no parameters.
     *
     * You should override this method if your save function requires
     * parameters.
     *
     * @return The parameters for saving the mesh.
     */
    virtual ParameterVector parametersSave() const { return ParameterVector(); }

    /**
     * @brief Loads a mesh from the given file.
     *
     * This member function must be implemented by the derived classes if the
     * @ref ioSupport member function returns IOSupport::LOAD or
     * IOSupport::BOTH.
     *
     * @param[in] filename: the file to read from
     * @param[in] parameters: the parameters for loading the mesh
     * @param[out] loadedInfo: the information loaded from the file
     * @param[in] log: the logger to use
     * @return the mesh loaded from the file
     */
    virtual MeshType load(
        const std::string&     filename,
        const ParameterVector& parameters,
        vcl::MeshInfo&         loadedInfo,
        AbstractLogger&        log = logger()) const
    {
        if (ioSupport() == IOSupport::SAVE) {
            throw std::runtime_error(
                "The action " + name() + " does not support loading meshes.");
            return MeshType();
        }
        // This should never be reached - the action declared that is able to
        // load images, but it does not implement the load method.
        assert(0);
    };

    /**
     * @brief Saves the given mesh to the given file.
     *
     * This member function must be implemented by the derived classes if the
     * @ref ioSupport member function returns IOSupport::SAVE or
     * IOSupport::BOTH.
     *
     * @param[in] filename: the file to write to
     * @param[in] mesh: the mesh to save
     * @param[in] info: the information of the mesh to save in the file
     * @param[in] parameters: the parameters for saving the mesh
     * @param[in] log: the logger to use
     */
    virtual void save(
        const std::string&     filename,
        const MeshType&        mesh,
        const MeshInfo&        info,
        const ParameterVector& parameters,
        AbstractLogger&        log = logger()) const = 0;

    /* ************************************ *
     * Member functions already implemented *
     * ************************************ */

    Type type() const final { return Type::IO_MESH_ACTION; }

    MeshType load(
        const std::string&     filename,
        const ParameterVector& parameters,
        AbstractLogger&        log = logger()) const
    {
        MeshInfo info;
        auto     mesh = load(filename, parameters, info, log);
        return mesh;
    }

    MeshType load(
        const std::string& filename,
        AbstractLogger&    log = logger()) const
    {
        return load(filename, parametersLoad(), log);
    }

    void save(
        const std::string& filename,
        const MeshType&    mesh,
        AbstractLogger&    log = logger()) const
    {
        auto ext = FileInfo::extension(filename);
        save(filename, mesh, ext, parametersSave(), log);
    }

    void save(
        const std::string& filename,
        const MeshType&    mesh,
        const MeshInfo&    info,
        AbstractLogger&    log = logger()) const
    {
        save(filename, mesh, info, parametersSave(), log);
    }

    void save(
        const std::string&     filename,
        const MeshType&        mesh,
        const ParameterVector& parameters,
        AbstractLogger&        log = logger()) const
    {
        auto ext = FileInfo::extension(filename);
        save(filename, mesh, formatCapability(ext), parameters, log);
    }

protected:
    void postLoad(MeshType& mesh, const MeshInfo& loadedInfo) const
    {
        if constexpr (HasFaces<MeshType>) {
            if (!loadedInfo.hasFaceNormals()) {
                vcl::updatePerFaceNormals(mesh);
            }
            if (!loadedInfo.hasVertexNormals()) {
                vcl::updatePerVertexNormalsFromFaceNormals(mesh);
            }
        }
        vcl::updateBoundingBox(mesh);
    }

    MeshInfo formatCapability(const std::string& format) const
    {
        for (const auto& [f, info] : supportedFormats()) {
            if (f.matchExtension(format)) {
                return info;
            }
        }
        return MeshInfo();
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTION_INTERFACES_IO_MESH_ACTION_H
