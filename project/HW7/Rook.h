#pragma once

#include "Piece.h"

class Rook : Piece
{
public:
	Rook(const char type, const std::string& location);
//	use default ~Rook() destructor.

	virtual bool is_affordable(const Point& new_location, bool is_there_a_player) const;
};