#include "Rook.h"

Rook::Rook(const char type, const std::string& location) :
	Piece(type, location) 
{}

bool Rook::is_affordable(const Point& new_location, bool is_there_a_player) const {
	if (new_location.is_in_same_line_with(this->get_point())) {
		return true;
	}
	
	return false;
}
