#pragma once

#include <vector>
#include <memory>
#include <string>
#include <iostream>

#include "Pawn.hpp"
#include "King.hpp"
#include "Rook.hpp"
#include "Piece.hpp"
#include "Queen.hpp"
#include "Knight.hpp"
#include "Bishop.hpp"
#include "Dragon.hpp"

#define EMPTY_CELL_CHAR '#'
#define QUEEN 'q'
#define KING 'k'
#define ROOK 'r'
#define KNIGHT 'n'
#define BISHOP 'b'
#define PAWN 'p'
#define DRAGON 'd'

enum CommandResult {
	VALID_MOVE = 0, 
	CHESS, 
	SOURCE_HASNT_CURRENT_PLAYERS_PIECE, 
	INVALID_CURRENT_COLOR_IN_DEST, 
	INVALID_SELF_CHESS, 
	ILLEGAL_INDEXES, 
	ILLEGAL_MOVEMENT, 
	DEST_SAME_AS_SOURCE, 
	MATE
};

class Game
{	
public:
	explicit Game(const char* board, size_t board_length);
	std::string to_string() const;

	void draw() const;
	bool is_check() const;
	bool is_move_interrupted(const Point& src, const Point& dst) const;

	CommandResult exec_command(const std::string& command);

private:
	const Piece* get_current_player_king() const;
	bool is_point_on_board(const Point& point) const;
	bool exists_piece_at_point(const Point& point) const;

	Point parse_point(const std::string& point_cords) const;
	std::unique_ptr<Piece>& board_at_point(const Point& point);
	const std::unique_ptr<Piece>& board_at_point(const Point& point) const;

private:
	static std::unique_ptr<Piece> create_piece(char type, const Point& location);	

private:
	Player _turn;
	const size_t _board_length;
	std::vector<std::unique_ptr<Piece>> _board;

	Piece* _white_king;
	Piece* _black_king;
};
