#include "Point.h"

Point::Point() :
	_x(0), _y(0) 
{}

Point::Point(const std::string& location) {
	init_point(location);
}

void Point::init_point(const std::string& location) {
	_x = location[INDEX_OF_X] - FIRST_INDEX_VALUE_OF_X;	//first index of
	_y = FIRST_INDEX_VALUE_OF_Y - location[INDEX_OF_Y];	//array is zero
}

int Point::get_x() const {
	return _x;
}

int Point::get_y() const {
	return _y;
}

bool Point::is_diagonalled_to(const Point& other) const {
	return abs(get_x() - other.get_x()) == abs(get_y() - other.get_y());
}

bool Point::is_in_same_line_with(const Point& other) const {
	return get_x() == other.get_x() || get_y() == other.get_y();
}

bool Point::operator!=(const Point& other) const {
	return get_x() != other.get_x() || get_y() != other.get_y();
}
