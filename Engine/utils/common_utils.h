#pragma once


struct coord_t {
	int x, y;

	// overload for == operator to use coord_t directly in the conditions
	friend bool operator==(const coord_t& coords1, const coord_t& coords2) {
		return coords1.x == coords2.x && coords1.y == coords2.y;
	}

	friend bool operator!=(const coord_t& coords1, const coord_t& coords2) {
		return !(coords1 == coords2);
	}
};


struct fcoord_t {
	float x, y;

	friend bool operator==(const fcoord_t& coords1, const fcoord_t& coords2) {
		return coords1.x == coords2.x && coords1.y == coords2.y;
	}

	friend bool operator!=(const fcoord_t& coords1, const fcoord_t& coords2) {
		return !(coords1 == coords2);
	}
};


namespace UtilFunc {

	inline int _min(int a, int b) {
		return a < b ? a : b;
	}

	inline int _max(int a, int b) {
		return a < b ? b : a;
	}

}