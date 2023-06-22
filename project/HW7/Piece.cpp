#include "Piece.h"

Piece::Piece(const char type, const std::string& location) :
	_type(type), _place(Point(location))
{}

void Piece::update_location(const std::string& new_location) {
	this->_place.init_point(new_location);
}

char Piece::get_type() const {
	return this->_type;
}

Players Piece::get_owner() const {
	if (islower(this->get_type())) {
		return Players::BLACK_PLAYER;
	} else {
		return Players::WHITE_PLAYER;
	}
}

const Point& Piece::get_point() const {
	return this->_place;
}