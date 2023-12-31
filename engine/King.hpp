#pragma once
#include "Piece.hpp"

class King : public Piece
{
public:
	explicit King(const char type, const Point& location);
	virtual bool is_reachable(const Point& new_location, bool is_there_a_player) const;
};
