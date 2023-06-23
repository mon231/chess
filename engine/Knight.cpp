#include "Knight.hpp"

Knight::Knight(const char type, const Point& location) :
	Piece(type, location)
{}

bool Knight::is_reachable(const Point& new_location, bool is_there_a_player) const 
{
	if ((new_location.get_delta_x(_location) == 2) && (new_location.get_delta_y(_location) == 1))
	{
		return true;
	}

	if ((new_location.get_delta_x(_location) == 1) && (new_location.get_delta_y(_location) == 2))
	{
		return true;
	}

	return false;
}
