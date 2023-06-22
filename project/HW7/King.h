#pragma once

#include "Piece.h"

class King : Piece
{
public:
	King(const char type, const std::string& location);
//	use default ~King() destructor.

	virtual bool is_affordable(const Point& new_location, bool is_there_a_player) const;
};