#pragma once

#include <vector>

#include "utils/common_utils.h"
#include "utils/render_utils.h"	// namespace RenderConsts, Render (Utils, Objects::Colors)
#include "utils/math_utils.h"

// Interface between console and gl renderers
class IRenderer {

public:
	virtual void render(const std::vector<std::vector<char>>& map,
		const fcoord_t& playerCoord, float playerAngle, bool useDDA);

	virtual ~IRenderer() = default;
};