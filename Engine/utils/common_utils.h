#pragma once
#include <iostream>
#include <limits>
#include <cctype>
#include <exception>
#include <string>

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


namespace StreamUtils {

	// Clears input stream to prevent errors with its buffer
	void clearInputStream();
	void softClearInputStream();

	// Input unsigned int in the safety, especially for proj_tests.h
	// Idea: we input string and check if this is number, then try to cast uint 
	//			after stoul (string to unsigned long)
	// Returns input success status
	bool safeInputUInt(unsigned int& val);
}