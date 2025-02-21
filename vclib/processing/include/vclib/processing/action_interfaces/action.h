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

#ifndef VCL_PROCESSING_ACTION_INTERFACES_ACTION_H
#define VCL_PROCESSING_ACTION_INTERFACES_ACTION_H

#include <vclib/misc/logger.h>
#include <vclib/misc/string.h>
#include <vclib/types.h>

#include <algorithm>
#include <memory>

namespace vcl::proc {

class Action
{
    inline static ConsoleLogger log;

public:
    enum class Type {
        IO_IMAGE_ACTION = 0,
        IO_MESH_ACTION,
        // LOAD_MESH_ACTION,
        // SAVE_MESH_ACTION,
        // FILTER_MESH_ACTION,

        COUNT
    };

    Action()          = default;
    virtual ~Action() = default;

    /**
     * @brief Returns a new instance of the action.
     *
     * Assuming that your action is called `MyAction`, you should implement this
     * member function as follows:
     *
     * @code{.cpp}
     * std::shared_ptr<Action> clone() const override
     * {
     *    return std::make_shared<MyAction>(*this);
     * }
     * @endcode
     *
     * @return A shared pointer to the new instance of the action.
     */
    [[nodiscard]] virtual std::shared_ptr<Action> clone() const = 0;

    /**
     * @brief Returns the name of the action.
     *
     * The name of the action should be unique.
     *
     * @return The name of the action.
     */
    virtual std::string name() const = 0;

    /**
     * @brief Returns the type of the action.
     *
     * See the enum class @ref Type for the list of available types.
     *
     * @return The type of the action.
     */
    virtual Type type() const = 0;

protected:
    static ConsoleLogger& logger() { return log; }

private:
    // members used by the action manager
    std::string identifier() const { return identifierFromName(name()); }

    static std::string identifierFromName(const std::string& name)
    {
        std::string n = name;

        std::replace(n.begin(), n.end(), ' ', '_');
        n = vcl::toLower(n);

        return n;
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTION_INTERFACES_ACTION_H
