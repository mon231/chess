#include "Piece.hpp"

Piece::Piece(const char type, const Point& location) :
	_type(type), _location(location)
{}

void Piece::update_location(const Point& new_location)
{
	_location = new_location;
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
	return _location;
}
