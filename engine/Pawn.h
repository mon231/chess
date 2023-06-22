#pragma once
#include "Piece.h"

#define DOUBLE 2
#define STEP_FORWARD 1
#define STEP_BACKWORD -1

class Pawn : public Piece
{
private:
	bool _is_first_step;

public:
	explicit Pawn(const char type, const std::string& location);
	virtual void update_location(const std::string& new_location) override;
	virtual bool is_reachable(const Point& new_location, bool isThereAPlayer) const;
};
