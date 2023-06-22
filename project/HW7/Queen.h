#pragma once

#include "Piece.h"

class Queen : Piece
{
public:
	Queen(const char type, const std::string& location);
//	use default ~Queen() destructor.

	virtual bool is_affordable(const Point& new_location, bool is_there_a_player) const;
};