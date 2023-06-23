#pragma once
#include "Piece.hpp"

class Bishop : public Piece
{
public:
	explicit Bishop(const char type, const Point& location);
	virtual bool is_reachable(const Point& new_location, bool is_there_a_player) const;
};
