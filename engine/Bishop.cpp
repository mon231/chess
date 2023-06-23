#include "Bishop.h"

Bishop::Bishop(const char type, const Point& location) :
	Piece(type, location)
{}

bool Bishop::is_reachable(const Point& new_location, bool is_there_a_player) const {
	return new_location.is_diagonalled_to(get_point());
}
