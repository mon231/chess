#pragma once
#include "Piece.h"

class Queen : public Piece
{
public:
	explicit Queen(const char type, const std::string& location);
	virtual bool is_reachable(const Point& new_location, bool is_there_a_player) const;
};
