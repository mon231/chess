#pragma once
#include "Piece.h"

/*Dragon can move like queen or like knight, and he can skip other pieces in his way to his goal, so it doesn't need to be checked in the "is_move_interrupted" method*/
class Dragon : public Piece
{
public:
	Dragon(const char type, const std::string& location);
	virtual bool is_reachable(const Point& new_location, bool isThereAPlayer) const;
};
