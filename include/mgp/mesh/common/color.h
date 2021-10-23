/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMMON_COLOR_H
#define MGP_MESH_COMMON_COLOR_H

#include <mgp/space/color.h>

#include "info.h"

namespace mgp::common {

class Color : public virtual Info
{
public:
	typedef mgp::Color ColorType;
	const mgp::Color& color() const { return c; }
	mgp::Color&       color() { return c; }

private:
	mgp::Color c;
};

} // namespace mgp::common

#endif // MGP_MESH_COMMON_COLOR_H
