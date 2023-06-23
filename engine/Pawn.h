#pragma once
#include "Piece.h"

#define DOUBLE 2
#define STEP_FORWARD 1
#define STEP_BACKWORD -1

class Pawn : public Piece
{
public:
	explicit Pawn(const char type, const Point& location);

	virtual void update_location(const Point& new_location) override;
	virtual bool is_reachable(const Point& new_location, bool isThereAPlayer) const;

private:
	bool _is_first_step;
};
