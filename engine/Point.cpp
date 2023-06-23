#include "Point.h"

Point::Point() :
	_x(), _y() 
{}

Point::Point(size_t x, size_t y) :
	_x(x), _y(y)
{}

Point::Point(const std::string& location) 
{
	init_point(location);
}

void Point::init_point(const std::string& location) 
{
	_x = location[INDEX_OF_X] - FIRST_INDEX_VALUE_OF_X;
	_y = FIRST_INDEX_VALUE_OF_Y - location[INDEX_OF_Y];
}

size_t Point::get_x() const 
{
	return _x;
}

size_t Point::get_y() const 
{
	return _y;
}

size_t Point::get_delta_x(const Point& other) const
{
	return std::max(_x, other._x) - std::min(_x, other._x);
}

size_t Point::get_delta_y(const Point& other) const
{
	return std::max(_y, other._y) - std::min(_y, other._y);
}

bool Point::is_diagonalled_to(const Point& other) const 
{
	return get_delta_x(other) == get_delta_y(other);
}

bool Point::is_in_row_or_column_with(const Point& other) const 
{
	return (_x == other._x) || (_y == other._y);
}

bool Point::operator==(const Point& other) const
{
	return (_x == other._x) && (_y == other._y);
}

bool Point::operator!=(const Point& other) const
{
	return !(*this == other);
}
