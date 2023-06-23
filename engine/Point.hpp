#pragma once
#include <string>

class Point
{
public:
	explicit Point();
	explicit Point(size_t x, size_t y);

	bool is_diagonalled_to(const Point& other) const;
	bool is_in_row_or_column_with(const Point& other) const;
	bool is_on_line_between(const Point& a, const Point& b) const;

	bool operator==(const Point& other) const;
	bool operator!=(const Point& other) const;

	size_t get_x() const;
	size_t get_y() const;

	size_t get_delta_x(const Point& other) const;
	size_t get_delta_y(const Point& other) const;

private:
	void init_point(const std::string& location, size_t board_size);

private:
	size_t _x;
	size_t _y;
};
