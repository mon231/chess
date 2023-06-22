#pragma once

#include "Piece.h"

class Knight : Piece
{
public:
	Knight(const char type, const std::string& location);
//	use default ~Knight() destructor.

	virtual bool is_affordable(const Point& new_location, bool is_there_a_player) const;
};