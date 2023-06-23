#include "Point.hpp"
#include <algorithm>

Point::Point() :
	_x(), _y() 
{}

Point::Point(size_t x, size_t y) :
	_x(x), _y(y)
{}

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

bool Point::is_on_line_between(const Point& b, const Point& c) const
{
	if ((_x * (b._y - c._y) + b._x * (c._y - _y) + c._x * (_y - b._y)) != 0)
	{
		return false;
	}

	size_t distAB = (b._x - _x) * (b._x - _x) + (b._y - _y) * (b._y - _y);
	size_t distAC = (c._x - _x) * (c._x - _x) + (c._y - _y) * (c._y - _y);
	size_t distBC = (c._x - b._x) * (c._x - b._x) + (c._y - b._y) * (c._y - b._y);

	return (distBC >= distAB) && (distBC >= distAC);
}

bool Point::operator==(const Point& other) const
{
	return (_x == other._x) && (_y == other._y);
}

bool Point::operator!=(const Point& other) const
{
	return !(*this == other);
}
