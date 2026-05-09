#pragma once

#include <vector>
#include <limits>       // numeric_limits
#include <algorithm>	// fill
#include <cmath>

#include <iostream>

#include "../utils/common_utils.h"
#include "../utils/math_utils.h"
#include "../utils/render_utils.h" // namespace RenderConsts, Render (Utils, Objects::Colors)


class RaycastEngine {

    // cache cos/sin instead of each step calculating
    std::vector<float> sinTable, cosTable;
    float angleStep = 0.0f;

    static constexpr int TRIG_TABLE_SIZE = 3600; // 0.1 precision points

public:

    static constexpr float FOV = MathUtils::Consts::QUART_PI; // field of view, pi/4
    static constexpr float DEPTH = 16.0f; // max render distance

    RaycastEngine();


    // simple raycast algorithm, returns float distance from player to object
    float castRay_stepped(const std::vector<std::vector<char>>& map,
        const fcoord_t& playerCoord, float rayAngle);

    // Implementation of faster DDA raycast algorithm (improve x10)
    // Optimized algorithm that crosses grid from one cell to second with one step only.
    // Guaranteed to visit every cell in the ray path once
    float castRay_dda(const std::vector<std::vector<char>>& map,
        const fcoord_t& playerCoord, float rayAngle);


private:

    // fast search of cached cos
    inline float getCachedCos(float angle) const {

        // normalization
        MathUtils::Angles::normalizeAngle(angle);

        // find index in the table
        int index = static_cast<int>(angle / angleStep);
        if (index >= cosTable.size())
            index = 0;

        return cosTable[index];
    }


    // sin analog
    inline float getCachedSin(float angle) const {

        MathUtils::Angles::normalizeAngle(angle);

        int index = static_cast<int>(angle / angleStep);
        if (index >= sinTable.size()) index = 0;

        return sinTable[index];
    }

public:
// pre-compute:
    
    // Computes sin, cos values via compilation, not to compute it every raycasting step
    void precomputeTrigTables();
    // Show computed values
    void showPrecomputedTrigTables();
};
