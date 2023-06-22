#include "King.h"

King::King(const char type, const std::string& location) :
	Piece(type, location)
{}

bool King::is_affordable(const Point& new_location, bool is_there_a_player) const {
	if (new_location.get_x() == this->get_point().get_x() && (new_location.get_y() == this->get_point().get_y() - 1 || new_location.get_y() == this->get_point().get_y() + 1)) {
		return true;
	}

	if (new_location.get_y() == this->get_point().get_y() && (new_location.get_x() == this->get_point().get_x() - 1 || new_location.get_x() == this->get_point().get_x() + 1)) {
		return true;
	}

	if ((new_location.get_y() == this->get_point().get_y() - 1 || new_location.get_y() == this->get_point().get_y() + 1) && (new_location.get_x() == this->get_point().get_x() - 1 || new_location.get_x() == this->get_point().get_x() + 1)) {
		return true;
	}

	return false;
}