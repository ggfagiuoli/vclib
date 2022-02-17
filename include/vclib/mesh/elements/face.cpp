/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#include "face.h"

namespace vcl {

template<typename... Args>
Face<Args...>::Face()
{
}

template<typename... Args>
Face<Args...>::Face(const std::vector<VertexType*>& list)
{
	setVertices(list);
}

template<typename... Args>
template<typename... V>
Face<Args...>::Face(V... args)
{
	setVertices({args...});
}

/**
 * @brief Sets a list of Vertex references to the face. If the Face size is dynamic, will take care
 * to update the also the number of adjacent faces and the number of wedge components, if these
 * components are part of the Face and if the size of the Face is changed. On the contrary, if the
 * Face size is static, the number of vertices of the list must be equal to the size of the Face.
 *
 * @param list
 */
template<typename... Args>
void Face<Args...>::setVertices(const std::vector<VertexType*>& list)
{
	using F = Face<Args...>;

	VRefs::setVertices(list);

	static const int VN = F::VERTEX_NUMBER;
	if constexpr (VN < 0) {

		if constexpr (face::hasOptionalAdjacentEdges<F>()) {
			using T = typename F::OptionalAdjacentEdges;

			if (T::isAdjEdgesEnabled())
				T::resizeAdjEdges(list.size());
		}
		else if constexpr (face::hasAdjacentEdges<F>()) {
			using T = typename F::AdjacentEdges;

			T::resizeAdjEdges(list.size());
		}

		if constexpr (face::hasOptionalAdjacentFaces<F>()) {
			using T = typename F::OptionalAdjacentFaces;

			if (T::isAdjFacesEnabled())
				T::resizeAdjFaces(list.size());
		}
		else if constexpr (face::hasAdjacentFaces<F>()) {
			using T = typename F::AdjacentFaces;

			T::resizeAdjFaces(list.size());
		}

		if constexpr (face::hasOptionalWedgeColors<F>()) {
			using T = typename F::OptionalWedgeColors;

			if (T::isWedgeColorsEnabled())
				T::resizeWedgeColors(list.size());
		}
		else if constexpr (face::hasWedgeColors<F>()) {
			using T = typename F::WedgeColors;

			T::resizeWedgeColors(list.size());
		}

		if constexpr (face::hasOptionalWedgeTexCoords<F>()) {
			using T = typename F::OptionalWedgeTexCoords;

			if (T::isWedgeTexCoordsEnabled())
				T::resizeWedgeTexCoords(list.size());
		}
		else if constexpr (face::hasWedgeTexCoords<F>()) {
			using T = typename F::WedgeTexCoords;

			T::resizeWedgeTexCoords(list.size());
		}
	}
}

template<typename... Args>
template<typename... V>
void Face<Args...>::setVertices(V... args)
{
	setVertices({args...});
}

template<typename... Args>
template<typename Element>
void Face<Args...>::importFrom(const Element& f)
{
	(Args::importFrom(f), ...);
}

/**
 * @brief Resize the number of Vertex References of the Face, taking care of updating also the
 * number of adjacent faces and the number of wedge components of the Face, if these components
 * are part of the Face.
 *
 * If n is greater than the old number of vertex references, n vertex references (and relative
 * wedge components) will be added. If n is lower than the old number of vertex references, the
 * difference of vertex references (and relative wedge components) will be removed.
 *
 * This member function is available only **if the face is polygonal (its size is dynamic, N < 0)**.
 *
 * @param n: the new number of vertices.
 */
template<typename... Args>
template<int M>
VCL_ENABLE_IF(M < 0, void)
Face<Args...>::resizeVertices(uint n)
{
	using F = Face<Args...>;

	VRefs::resizeVertices(n);

	if constexpr (face::hasOptionalAdjacentEdges<F>()) {
		using T = typename F::OptionalAdjacentEdges;

		if (T::isAdjEdgesEnabled())
			T::resizeAdjEdges(n);
	}
	else if constexpr (face::hasAdjacentEdges<F>()) {
		using T = typename F::AdjacentEdges;

		T::resizeAdjEdges(n);
	}

	if constexpr (face::hasOptionalAdjacentFaces<F>()) {
		using T = typename F::OptionalAdjacentFaces;

		if (T::isAdjFacesEnabled())
			T::resizeAdjFaces(n);
	}
	else if constexpr (face::hasAdjacentFaces<F>()) {
		using T = typename F::AdjacentFaces;

		T::resizeAdjFaces(n);
	}

	if constexpr (face::hasOptionalWedgeColors<F>()) {
		using T = typename F::OptionalWedgeColors;

		if (T::isWedgeColorsEnabled())
			T::resizeWedgeColors(n);
	}
	else if constexpr (face::hasWedgeColors<F>()) {
		using T = typename F::WedgeColors;

		T::resizeWedgeColors(n);
	}

	if constexpr (face::hasOptionalWedgeTexCoords<F>()) {
		using T = typename F::OptionalWedgeTexCoords;

		if (T::isWedgeTexCoordsEnabled())
			T::resizeWedgeTexCoords(n);
	}
	else if constexpr (face::hasWedgeTexCoords<F>()) {
		using T = typename F::WedgeTexCoords;

		T::resizeWedgeTexCoords(n);
	}
}

template<typename... Args>
template<int M>
VCL_ENABLE_IF(M < 0, void)
Face<Args...>::pushVertex(VertexType* v)
{
	using F = Face<Args...>;

	VRefs::pushVertex(v);

	if constexpr (face::hasOptionalAdjacentEdges<F>()) {
		using T = typename F::OptionalAdjacentEdges;

		if (T::isAdjEdgesEnabled())
			T::pushAdjEdge(nullptr);
	}
	else if constexpr (face::hasAdjacentEdges<F>()) {
		using T = typename F::AdjacentEdges;

		T::pushAdjEdge(nullptr);
	}

	if constexpr (face::hasOptionalAdjacentFaces<F>()) {
		using T = typename F::OptionalAdjacentFaces;

		if (T::isAdjFacesEnabled())
			T::pushAdjFace(nullptr);
	}
	else if constexpr (face::hasAdjacentFaces<F>()) {
		using T = typename F::AdjacentFaces;

		T::pushAdjFace(nullptr);
	}

	if constexpr (face::hasOptionalWedgeColors<F>()) {
		using T = typename F::OptionalWedgeColors;

		if (T::isWedgeColorsEnabled())
			T::pushWedgeColors(Color());
	}
	else if constexpr (face::hasWedgeColors<F>()) {
		using T = typename F::WedgeColors;

		T::pushWedgeTexColors(Color());
	}

	if constexpr (face::hasOptionalWedgeTexCoords<F>()) {
		using S = typename F::WedgeTexCoordScalarType;
		using T = typename F::OptionalWedgeTexCoords;

		if (T::isWedgeTexCoordsEnabled())
			T::pushWedgeTexCoord(TexCoord<S>());
	}
	else if constexpr (face::hasWedgeTexCoords<F>()) {
		using S = typename F::WedgeTexCoordScalarType;
		using T = typename F::WedgeTexCoords;

		T::pushWedgeTexCoord(TexCoord<S>());
	}
}

template<typename... Args>
template<int M>
VCL_ENABLE_IF(M < 0, void)
Face<Args...>::insertVertex(uint i, VertexType* v)
{
	using F = Face<Args...>;

	VRefs::insertVertex(i, v);

	if constexpr (face::hasOptionalAdjacentEdges<F>()) {
		using T = typename F::OptionalAdjacentEdges;

		if (T::isAdjEdgesEnabled())
			T::insertAdjEdge(i, nullptr);
	}
	else if constexpr (face::hasAdjacentEdges<F>()) {
		using T = typename F::AdjacentEdges;

		T::insertAdjEdge(i, nullptr);
	}

	if constexpr (face::hasOptionalAdjacentFaces<F>()) {
		using T = typename F::OptionalAdjacentFaces;

		if (T::isAdjFacesEnabled())
			T::insertAdjFace(i, nullptr);
	}
	else if constexpr (face::hasAdjacentFaces<F>()) {
		using T = typename F::AdjacentFaces;

		T::insertAdjFace(i, nullptr);
	}

	if constexpr (face::hasOptionalWedgeColors<F>()) {
		using T = typename F::OptionalWedgeColors;

		if (T::isWedgeColorsEnabled())
			T::insertWedgeColor(i, Color());
	}
	else if constexpr (face::hasWedgeColors<F>()) {
		using T = typename F::WedgeColors;

		T::insertWedgeColor(i, Color());
	}

	if constexpr (face::hasOptionalWedgeTexCoords<F>()) {
		using S = typename F::WedgeTexCoordScalarType;
		using T = typename F::OptionalWedgeTexCoords;

		if (T::isWedgeTexCoordsEnabled())
			T::insertWedgeTexCoord(i, TexCoord<S>());
	}
	else if constexpr (face::hasWedgeTexCoords<F>()) {
		using S = typename F::WedgeTexCoordScalarType;
		using T = typename F::WedgeTexCoords;

		T::insertWedgeTexCoord(i, TexCoord<S>());
	}
}

template<typename... Args>
template<int M>
VCL_ENABLE_IF(M < 0, void)
Face<Args...>::eraseVertex(uint i)
{
	using F = Face<Args...>;

	VRefs::eraseVertex(i);

	if constexpr (face::hasOptionalAdjacentEdges<F>()) {
		using T = typename F::OptionalAdjacentEdges;

		if (T::isAdjEdgesEnabled())
			T::eraseAdjEdge(i);
	}
	else if constexpr (face::hasAdjacentEdges<F>()) {
		using T = typename F::AdjacentEdges;

		T::eraseAdjEdge(i);
	}

	if constexpr (face::hasOptionalAdjacentFaces<F>()) {
		using T = typename F::OptionalAdjacentFaces;

		if (T::isAdjFacesEnabled())
			T::eraseAdjFace(i);
	}
	else if constexpr (face::hasAdjacentFaces<F>()) {
		using T = typename F::AdjacentFaces;

		T::eraseAdjFace(i);
	}

	if constexpr (face::hasOptionalWedgeColors<F>()) {
		using T = typename F::OptionalWedgeColors;

		if (T::isWedgeColorsEnabled())
			T::eraseWedgeColor(i);
	}
	else if constexpr (face::hasWedgeColors<F>()) {
		using T = typename F::WedgeColors;

		T::eraseWedgeColor(i);
	}

	if constexpr (face::hasOptionalWedgeTexCoords<F>()) {
		using T = typename F::OptionalWedgeTexCoords;

		if (T::isWedgeTexCoordsEnabled())
			T::eraseWedgeTexCoord(i);
	}
	else if constexpr (face::hasWedgeTexCoords<F>()) {
		using T = typename F::WedgeTexCoords;

		T::eraseWedgeTexCoord(i);
	}
}

template<typename... Args>
template<int M>
VCL_ENABLE_IF(M < 0, void)
Face<Args...>::clearVertices()
{
	using F = Face<Args...>;

	VRefs::clearVertices();

	if constexpr (face::hasOptionalAdjacentEdges<F>()) {
		using T = typename F::OptionalAdjacentEdges;

		if (T::isAdjEdgesEnabled())
			T::clearAdjEdges();
	}
	else if constexpr (face::hasAdjacentEdges<F>()) {
		using T = typename F::AdjacentEdges;

		T::clearAdjEdges();
	}

	if constexpr (face::hasOptionalAdjacentFaces<F>()) {
		using T = typename F::OptionalAdjacentFaces;

		if (T::isAdjFacesEnabled())
			T::clearAdjFaces();
	}
	else if constexpr (face::hasAdjacentFaces<F>()) {
		using T = typename F::AdjacentFaces;

		T::clearAdjFaces();
	}

	if constexpr (face::hasOptionalWedgeColors<F>()) {
		using T = typename F::OptionalWedgeColors;

		if (T::isWedgeColorsEnabled())
			T::clearWedgeColor();
	}
	else if constexpr (face::hasWedgeColors<F>()) {
		using T = typename F::WedgeColors;

		T::clearWedgeColor();
	}

	if constexpr (face::hasOptionalWedgeTexCoords<F>()) {
		using T = typename F::OptionalWedgeTexCoords;

		if (T::isWedgeTexCoordsEnabled())
			T::clearWedgeTexCoord();
	}
	else if constexpr (face::hasWedgeTexCoords<F>()) {
		using T = typename F::WedgeTexCoords;

		T::clearWedgeTexCoord();
	}
}

} // namespace vcl
