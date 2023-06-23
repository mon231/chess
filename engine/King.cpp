#include "King.h"

King::King(const char type, const Point& location) :
	Piece(type, location)
{}

bool King::is_reachable(const Point& new_location, bool is_there_a_player) const {
	if (new_location.get_x() == get_point().get_x() && (new_location.get_y() == get_point().get_y() - 1 || new_location.get_y() == get_point().get_y() + 1)) {
		return true;
	}

	if (new_location.get_y() == get_point().get_y() && (new_location.get_x() == get_point().get_x() - 1 || new_location.get_x() == get_point().get_x() + 1)) {
		return true;
	}

	if ((new_location.get_y() == get_point().get_y() - 1 || new_location.get_y() == get_point().get_y() + 1) && (new_location.get_x() == get_point().get_x() - 1 || new_location.get_x() == get_point().get_x() + 1)) {
		return true;
	}

	return false;
}
