#pragma once
#include "Piece.hpp"

class Knight : public Piece
{
public:
	explicit Knight(const char type, const Point& location);
	virtual bool is_reachable(const Point& new_location, bool is_there_a_player) const;
};
