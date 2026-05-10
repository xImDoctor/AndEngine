#include "RaycastEngine.h"


RaycastEngine::RaycastEngine() {
	precomputeTrigTables();
}

// Get drawing symbol for wall based on its distance
float RaycastEngine::castRay_stepped(const std::vector<std::vector<char>>& map, const fcoord_t& playerCoord, float rayAngle) {

	int mapHeight = static_cast<int>(map.size());
	int mapWidth = mapHeight > 0 ? static_cast<int>(map[0].size()) : 0;

	// angle normalization
	//normalizeAngle(rayAngle);	// inside of precomputed search now

	fcoord_t rayCoord = playerCoord;	// cast rays from player pos

	fcoord_t rayDirection;

	// use precomputed values
	//rayDirection.x = cosf(rayAngle);
	//rayDirection.y = sinf(rayAngle);
	rayDirection = { getCachedCos(rayAngle), getCachedSin(rayAngle) };

	float distance = 0.0f;
	float stepSize = 0.01f;  // ray step size

	bool isWallHit = false;

	while (!isWallHit && distance < DEPTH) {
		distance += stepSize;

		// ray new position
		rayCoord.x = playerCoord.x + rayDirection.x * distance;
		rayCoord.y = playerCoord.y + rayDirection.y * distance;

		// check map border
		if (rayCoord.x < 0 || rayCoord.x >= mapWidth || rayCoord.y < 0 || rayCoord.y >= mapHeight) {
			isWallHit = true;
			distance = DEPTH;
		}
		else {
			// check if wall is hit
			coord_t mapCoord = { static_cast<int>(rayCoord.x), static_cast<int>(rayCoord.y) };
			if (map[mapCoord.y][mapCoord.x] == Render::Objects::WALL)
				isWallHit = true;
		}

	}

	return distance;
}


// DDA version of raycast
float RaycastEngine::castRay_dda(const std::vector<std::vector<char>>& map, const fcoord_t& playerCoord, float rayAngle) {

	int mapHeight = static_cast<int>(map.size());
	int mapWidth = mapHeight > 0 ? static_cast<int>(map[0].size()) : 0;

	if (!mapHeight || !mapWidth)
		return DEPTH;


	// use cached values instead
	//normalizeAngle(rayAngle);
	//fcoord_t rayDirection = { cosf(rayAngle), sinf(rayAngle) };

	fcoord_t rayDirection = { getCachedCos(rayAngle), getCachedSin(rayAngle) };
	fcoord_t rayCoord = playerCoord;

	// which box of the map (grid) we are in; int coord struct
	coord_t mapCoord = { static_cast<int>(rayCoord.x), static_cast<int>(rayCoord.y) };

	// border check as at the stepped alg
	if (mapCoord.x < 0 || mapCoord.x >= mapWidth || mapCoord.y < 0 || mapCoord.y >= mapHeight)
		return DEPTH;

	// we have directly hit a wall
	if (map[mapCoord.y][mapCoord.x] == Render::Objects::WALL)
		return 0.0f;

	// init max  (limited) float value for alg
	constexpr float FLOAT_INFINITY = std::numeric_limits<float>::infinity();

	// delta distances of alg
	fcoord_t deltaDistance;
	deltaDistance.x = (rayDirection.x == 0.0f) ? FLOAT_INFINITY : std::fabsf(1.0f / rayDirection.x);
	deltaDistance.y = (rayDirection.y == 0.0f) ? FLOAT_INFINITY : std::fabsf(1.0f / rayDirection.y);

	coord_t stepCoord;			// int coords of each step for grid cells
	fcoord_t sideDistance;	// side distance coord

	if (rayDirection.x < 0.0f) {
		stepCoord.x = -1;
		sideDistance.x = (rayCoord.x - mapCoord.x) * deltaDistance.x;
	}
	else {
		stepCoord.x = 1;
		sideDistance.x = (mapCoord.x + 1 - rayCoord.x) * deltaDistance.x;
	}

	if (rayDirection.y < 0.0f) {
		stepCoord.y = -1;
		sideDistance.y = (rayCoord.y - mapCoord.y) * deltaDistance.y;
	}
	else {
		stepCoord.y = 1;
		sideDistance.y = (mapCoord.y + 1 - rayCoord.y) * deltaDistance.y;
	}

	bool isWallHit = false;
	int side = 0;	// if 0 then hit vertical side (x), if 1 then hit horizontal side (y)

	while (!isWallHit) {

		if (sideDistance.x < sideDistance.y) {	// vertical side
			sideDistance.x += deltaDistance.x;
			mapCoord.x += stepCoord.x;
			side = 0;
		}
		else {									// else horiz one
			sideDistance.y += deltaDistance.y;
			mapCoord.y += stepCoord.y;
			side = 1;
		}

		// check bounds again
		if (mapCoord.x < 0 || mapCoord.x >= mapWidth || mapCoord.y < 0 || mapCoord.y >= mapHeight)
			return DEPTH;

		// if coords crosses with wall - we hit it
		if (map[mapCoord.y][mapCoord.x] == Render::Objects::WALL)
			isWallHit = true;

	}

	// calc perpendicular distance (projected on camera direction)
	float perpDistance;

	if (side)	// 1, horiz line
		perpDistance = (mapCoord.y - playerCoord.y + (1 - stepCoord.y) / 2.0f) / rayDirection.y;
	else		// vertical line
		perpDistance = (mapCoord.x - playerCoord.x + (1 - stepCoord.x) / 2.0f) / rayDirection.x;

	// if distance bigger then max distance, return max one
	if (perpDistance > DEPTH)
		return DEPTH;

	return perpDistance;
}



// Computes sin, cos values via compilation, not to compute it every raycasting step
void RaycastEngine::precomputeTrigTables() {

	cosTable.clear();
	sinTable.clear();
	cosTable.reserve(TRIG_TABLE_SIZE);
	sinTable.reserve(TRIG_TABLE_SIZE);

	angleStep = MathUtils::Consts::TWO_PI / TRIG_TABLE_SIZE;

	for (int i = 0; i < TRIG_TABLE_SIZE; ++i) {
		float angle = i * angleStep;
		cosTable.push_back(cosf(angle));
		sinTable.push_back(sinf(angle));
	}

}

// Show computed values
void RaycastEngine::showPrecomputedTrigTables() {

	std::cout << "cos values:" << std::endl;

	int count = 0;

	for (const auto value : cosTable) {
		if (count == 10)
			break;

		std::cout << value << " | ";

		++count;
	}

	count = 0;
	std::cout << std::endl << "sin values:" << std::endl;

	for (const auto value : sinTable) {
		if (count == 10)
			break;

		std::cout << value << " | ";

		++count;
	}
	std::cout << std::endl;

}