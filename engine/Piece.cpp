#include "Piece.h"

Piece::Piece(const char type, const std::string& location) :
	_type(type), _place(Point(location))
{}

void Piece::update_location(const std::string& new_location) {
	_place.init_point(new_location);
}

char Piece::get_type() const {
	return _type;
}

Player Piece::get_owner() const {
	if (islower(get_type())) {
		return Player::BLACK_PLAYER;
	} else {
		return Player::WHITE_PLAYER;
	}
}

const Point& Piece::get_point() const {
	return _place;
}
