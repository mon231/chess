#pragma once

#include "Piece.h"

class Bishop : Piece
{
public:
	Bishop(const char type, const std::string& location);
//	use default ~Bishop() destructor.

	virtual bool is_affordable(const Point& new_location, bool is_there_a_player) const;
};