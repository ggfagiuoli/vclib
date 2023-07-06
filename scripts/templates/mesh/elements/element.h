%HEADER%

#ifndef VCL_MESH_ELEMENTS_%EL_U%_H
#define VCL_MESH_ELEMENTS_%EL_U%_H

#include <vclib/concepts/mesh/elements/%EL%.h>

#include "element.h"

namespace vcl {

/**
 * @brief The %EL_UC% class represents an %EL_UC% element of the
 * vcl::Mesh class.
 *
 * Using the %EL_UC%Container class, it is possible to add a vector of
 * %EL_UC% elements to a mesh, and manage them with the member functions
 * exposed by the %EL_UC%Container. Each %EL_UC% element exposes
 * all the member functions of its Component types.
 *
 * @tparam MeshType The type of the parent mesh.
 * @tparam Comps The types of the components of the element.
 *
 * @ingroup elements
 */
template<typename MeshType, typename... Comps>
class %EL_UC% : public Element<%EL_U%, MeshType, Comps...>
{
};

template<typename MeshType, typename... Comps>
class %EL_UC%<MeshType, TypeWrapper<Comps...>> :
		public %EL_UC%<MeshType, Comps...>
{
};

} // namespace vcl

#endif // VCL_MESH_ELEMENTS_%EL_U%_H
