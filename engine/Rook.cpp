#include "Rook.hpp"

Rook::Rook(const char type, const Point& location) :
	Piece(type, location) 
{}

bool Rook::is_reachable(const Point& new_location, bool is_there_a_player) const {
	return new_location.is_in_row_or_column_with(_location);
}
