#include "King.hpp"
#include "Macros.hpp"

King::King(const char type, const Point& location) :
	Piece(type, location)
{}

bool King::is_reachable(const Point& new_location, bool is_there_a_player) const {
	CHECK_AND_RETURN(
		new_location.get_delta_x(_location) <= 1 &&
		new_location.get_delta_y(_location) <= 1,
		false);

	return _location != new_location;
}
