#include "Pawn.hpp"
#include "Macros.hpp"

Pawn::Pawn(const char type, const Point& location) :
	Piece(type, location), _is_first_step(true)
{}

bool Pawn::is_reachable(const Point& new_location, bool is_there_a_player) const {
	const int movement_direction = get_owner() == Player::BLACK_PLAYER ? STEP_FORWARD : STEP_BACKWORD;

	if (is_there_a_player &&
		new_location.get_y() == _location.get_y() + movement_direction &&
		new_location.get_delta_x(_location) == 1)
	{
		return true;
	}

	CHECK_AND_RETURN(!is_there_a_player, false);
	CHECK_AND_RETURN(new_location.get_x() == _location.get_x(), false);

	if (new_location.get_y() == _location.get_y() + movement_direction)
	{
		return true;
	}

	if (_is_first_step &&
		new_location.get_y() == _location.get_y() + (movement_direction * DOUBLE)) {
		return true;
	}

	return false;
}

void Pawn::update_location(const Point& new_location)
{
	_is_first_step = false;
	_location = new_location;
}
