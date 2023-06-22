#pragma once
#include "Piece.h"

class Knight : public Piece
{
public:
	Knight(const char type, const std::string& location);
	virtual bool is_reachable(const Point& new_location, bool is_there_a_player) const;
};
