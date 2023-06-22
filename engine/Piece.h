#pragma once
#include "Point.h"

enum Player { NOT_ANY_PLAYER = -1, WHITE_PLAYER, BLACK_PLAYER };

class Piece
{
private:
	char _type;	//also letter and 'isWhite'

protected:
	Point _place;

public:
	explicit Piece(const char type, const std::string& location);

	virtual void update_location(const std::string& new_location);
	virtual bool is_reachable(const Point& new_location, bool is_there_a_player) const = 0;

	char get_type() const;
	Player get_owner() const;
	const Point& get_point() const;
};
