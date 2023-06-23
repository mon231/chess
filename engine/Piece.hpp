#pragma once
#include "Point.hpp"

enum Player { NOT_ANY_PLAYER = -1, WHITE_PLAYER, BLACK_PLAYER };

class Piece
{
public:
	explicit Piece(const char type, const Point& location);

	virtual void update_location(const Point& new_location);
	virtual bool is_reachable(const Point& new_location, bool is_there_a_player) const = 0;

	char get_type() const;
	Player get_owner() const;
	const Point& get_point() const;

private:
	char _type;	//also letter and 'isWhite'

protected:
	Point _location;
};
