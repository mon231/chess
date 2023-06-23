#pragma once
#include "Piece.h"

class Dragon : public Piece
{
public:
	explicit Dragon(const char type, const Point& location);
	virtual bool is_reachable(const Point& new_location, bool isThereAPlayer) const;
};

/* Dragon can move like any piece, and skip over other pieces in his way */
