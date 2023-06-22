#pragma once

#include "Piece.h"

#define DOUBLE 2
#define STEP_FORWARD 1
#define STEP_BACKWORD -1

class Pawn : Piece
{
private:
	bool _is_first_step;

public:
	Pawn(const char type, const std::string& location);
//	use default ~Pawn() destructor.

	virtual void update_location(const std::string& new_location) override;
	virtual bool is_affordable(const Point& new_location, bool isThereAPlayer) const;
};