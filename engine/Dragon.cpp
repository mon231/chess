#include "Dragon.h"

Dragon::Dragon(const char type, const Point& location) :
	Piece(type, location)
{}

bool Dragon::is_reachable(const Point& new_location, bool is_there_a_player) const {
	if (new_location.is_in_row_or_column_with(get_point())) {
		return true;
	}

	if (new_location.is_diagonalled_to(get_point())) {
		return true;
	}

	if ((new_location.get_x() == get_point().get_x() - 2 || new_location.get_x() == get_point().get_x() + 2) && (new_location.get_y() == get_point().get_y() - 1 || new_location.get_y() == get_point().get_y() + 1)) {
		return true;
	}

	if ((new_location.get_y() == get_point().get_y() - 2 || new_location.get_y() == get_point().get_y() + 2) && (new_location.get_x() == get_point().get_x() - 1 || new_location.get_x() == get_point().get_x() + 1)) {
		return true;
	}

	return false;
}
