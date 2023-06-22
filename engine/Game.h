#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "Piece.h"
#include "Pawn.h"
#include "King.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "Queen.h"
#include "Dragon.h"

#define BOARD_LENGTH 9
#define EMPTY_CELL_CHAR '#'
#define QUEEN 'q'
#define KING 'k'
#define ROOK 'r'
#define KNIGHT 'n'
#define BISHOP 'b'
#define PAWN 'p'
#define DRAGON 'd'

enum ResultOfCommand {VALID_MOVE, CHESS, SOURCE_HASNT_CURRENT_PLAYERS_PIECE, INVALID_CURRENT_COLOR_IN_DEST, INVALID_SELF_CHESS, ILLEGAL_INDEXES, ILLEGAL_MOVEMENT, DEST_SAME_AS_SOURCE, MATE};

class Game
{
private:
	std::vector<Piece*> _board; //array of Piece*
	Players _turn;
	Piece* _white_king;
	Piece* _black_king;
	
public:
	Game();
	~Game();

	void init(const char* board);

	std::string parse_string() const;
	void draw() const;
	bool is_check() const;
	bool is_move_interrupted(const Point& src, const Point& dst) const;

	ResultOfCommand exec_command(const std::string& command);
};
