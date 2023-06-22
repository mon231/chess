#include "Bishop.h"

Bishop::Bishop(const char type, const std::string& location) :
	Piece(type, location)
{}

bool Bishop::is_affordable(const Point& new_location, bool is_there_a_player) const {
	return new_location.is_diagonalled_to(this->get_point());
}