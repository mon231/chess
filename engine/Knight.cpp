#include "Knight.h"

Knight::Knight(const char type, const std::string& location) :
	Piece(type, location)
{}

bool Knight::is_reachable(const Point& new_location, bool is_there_a_player) const {
	if ((new_location.get_x() == get_point().get_x() - 2 || new_location.get_x() == get_point().get_x() + 2) && (new_location.get_y() == get_point().get_y() - 1 || new_location.get_y() == get_point().get_y() + 1)) {
		return true;
	}

	if ((new_location.get_y() == get_point().get_y() - 2 || new_location.get_y() == get_point().get_y() + 2) && (new_location.get_x() == get_point().get_x() - 1 || new_location.get_x() == get_point().get_x() + 1)) {
		return true;
	}

	return false;
}
