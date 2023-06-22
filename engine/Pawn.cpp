#include "Pawn.h"

Pawn::Pawn(const char type, const std::string& location) :
	Piece(type, location), _is_first_step(true) //haven't been eating yet
{}

bool Pawn::is_reachable(const Point& new_location, bool is_there_a_player) const {
	int movement_direction = get_owner() == Player::BLACK_PLAYER ? STEP_FORWARD : STEP_BACKWORD;

	if (new_location.get_y() == get_point().get_y() + movement_direction && is_there_a_player && (new_location.get_x() == get_point().get_x() + 1 || new_location.get_x() == get_point().get_x() - 1)) { //if eating other piece
		return true;
	}

	if (new_location.get_x() == get_point().get_x() && new_location.get_y() == get_point().get_y() + movement_direction && !is_there_a_player) { //move one forward
		return true;
	}

	if (new_location.get_x() == get_point().get_x() && new_location.get_y() == get_point().get_y() + movement_direction * DOUBLE && _is_first_step && !is_there_a_player) { //move double forward, first step
		return true;
	}

	return false;
}

void Pawn::update_location(const std::string& new_location) {
	_is_first_step = false;
	_place.init_point(new_location);
}
