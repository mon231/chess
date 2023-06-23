#include "Queen.hpp"

Queen::Queen(const char type, const Point& location) :
	Piece(type, location)
{}

bool Queen::is_reachable(const Point& new_location, bool is_there_a_player) const {
	if (new_location.is_in_row_or_column_with(_location)) {
		return true;
	}

	if (new_location.is_diagonalled_to(_location)) {
		return true;
	}

	return false;
}
