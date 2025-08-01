#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2025                                                    *
#* Visual Computing Lab                                                      *
#* ISTI - Italian National Research Council                                  *
#*                                                                           *
#* All rights reserved.                                                      *
#*                                                                           *
#* This program is free software; you can redistribute it and/or modify      *
#* it under the terms of the Mozilla Public License Version 2.0 as published *
#* by the Mozilla Foundation; either version 2 of the License, or            *
#* (at your option) any later version.                                       *
#*                                                                           *
#* This program is distributed in the hope that it will be useful,           *
#* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
#* Mozilla Public License Version 2.0                                        *
#* (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
#****************************************************************************/

### Build settings
set(CMAKE_CXX_STANDARD 20)

# In case of building shared libraries, on windows we need to export all symbols
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GNUInstallDirs)

if (WIN32)
    add_compile_definitions(NOMINMAX)
    add_compile_definitions(_USE_MATH_DEFINES)
    if (MSVC)
        # remove possible loss of data warnings in msvc, leave the others
        add_compile_options(/w44244 /w44267 /w44305)
    endif()
endif()

if (VCLIB_INSTALL_MODULE_CORE)
    set(VCLIB_CORE_EXCLUDE_FROM_ALL_OPTION)
else()
    set(VCLIB_CORE_EXCLUDE_FROM_ALL_OPTION EXCLUDE_FROM_ALL)
endif()

if (VCLIB_INSTALL_MODULE_EXTERNAL)
    set(VCLIB_EXTERNAL_EXCLUDE_FROM_ALL_OPTION)
else()
    set(VCLIB_EXTERNAL_EXCLUDE_FROM_ALL_OPTION EXCLUDE_FROM_ALL)
endif()

if (VCLIB_INSTALL_MODULE_RENDER)
    set(VCLIB_RENDER_EXCLUDE_FROM_ALL_OPTION)
else()
    set(VCLIB_RENDER_EXCLUDE_FROM_ALL_OPTION EXCLUDE_FROM_ALL)
endif()

if (VCLIB_INSTALL_MODULE_BINDINGS)
    set(VCLIB_BINDINGS_EXCLUDE_FROM_ALL_OPTION)
else()
    set(VCLIB_BINDINGS_EXCLUDE_FROM_ALL_OPTION EXCLUDE_FROM_ALL)
endif()
