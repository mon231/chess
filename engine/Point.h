#pragma once

#include <string>
#include <math.h>

#ifndef BOARD_LENGTH
#define BOARD_LENGTH 9
#endif

#define INDEX_OF_X 0
#define INDEX_OF_Y 1
#define FIRST_INDEX_VALUE_OF_X 'a'
#define FIRST_INDEX_VALUE_OF_Y ('0' + BOARD_LENGTH)
#define DISTANCE_BETWEEN(p1, p2) sqrt(pow(p1.get_x() - p2.get_x(), 2) + pow(p1.get_y() - p2.get_y(), 2)) /*Macro command to find double distabce between two points*/
#define IS_BETWEEN(a, b, c) (a.get_x() * (b.get_y() - c.get_y()) + b.get_x() * (c.get_y() - a.get_y()) + c.get_x() * (a.get_y() - b.get_y()) == 0) && pow(DISTANCE_BETWEEN(b, c), 2) + pow(DISTANCE_BETWEEN(c, a), 2) >= pow(DISTANCE_BETWEEN(a, b), 2) && pow(DISTANCE_BETWEEN(b, c), 2) + pow(DISTANCE_BETWEEN(a, b), 2) >= pow(DISTANCE_BETWEEN(c, a), 2) /*Macro command to check if 3 points are on the same line, when a is between b to c*/

class Point
{
public:
	explicit Point();
	explicit Point(size_t x, size_t y);
	explicit Point(const std::string& location);

	bool is_diagonalled_to(const Point& other) const;
	bool is_in_row_or_column_with(const Point& other) const;

	bool operator==(const Point& other) const;
	bool operator!=(const Point& other) const;

	size_t get_x() const;
	size_t get_y() const;

	size_t get_delta_x(const Point& other) const;
	size_t get_delta_y(const Point& other) const;

private:
	void init_point(const std::string& location);

private:
	size_t _x;
	size_t _y;
};
