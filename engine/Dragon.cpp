#include "Dragon.hpp"
#include "Macros.hpp"

Dragon::Dragon(const char type, const Point& location) :
	Piece(type, location)
{}

bool Dragon::is_reachable(const Point& new_location, bool is_there_a_player) const {
	if (new_location.is_in_row_or_column_with(_location)) 
	{
		return true;
	}

	if (new_location.is_diagonalled_to(_location)) 
	{
		return true;
	}

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
