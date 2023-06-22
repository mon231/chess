#include "Queen.h"

Queen::Queen(const char type, const std::string& location) :
	Piece(type, location)
{}

bool Queen::is_reachable(const Point& new_location, bool is_there_a_player) const {
	if (new_location.is_in_same_line_with(get_point())) {
		return true;
	}

	if (new_location.is_diagonalled_to(get_point())) {
		return true;
	}

	return false;
}
