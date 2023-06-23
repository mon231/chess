#include "Game.hpp"
#include "Macros.hpp"

Game::Game(const char* board) :
	_board(),
	_turn(Player::WHITE_PLAYER),
	_white_king(),
	_black_king()
{
	for (size_t y_index = 0; y_index < BOARD_LENGTH; ++y_index) {
		for (size_t x_index = 0; x_index < BOARD_LENGTH; ++x_index) {
			Point current_position{ x_index, y_index };
			const char piece_type = board[(y_index * BOARD_LENGTH) + x_index];

			_board.emplace_back(create_piece(piece_type, current_position));

			if (std::tolower(piece_type) != KING)
			{
				continue;
			}

			if (_board[_board.size() - 1]->get_owner() == Player::BLACK_PLAYER)
			{
				_black_king = _board[_board.size() - 1].get();
			}
			else
			{
				_white_king = _board[_board.size() - 1].get();
			}
		}
	}
}

std::string Game::to_string() const {
	std::string res = "";

	for (size_t y_index = 0; y_index < BOARD_LENGTH; y_index++)
	{
		for (size_t x_index = 0; x_index < BOARD_LENGTH; x_index++)
		{
			Point current_point{ x_index, y_index };

			if (!exists_piece_at_point(current_point))
			{
				res += EMPTY_CELL_CHAR;
			}
			else
			{
				res += board_at_point(current_point)->get_type();
			}
		}
	}

	res += _turn + '0';
	return res;
}

CommandResult Game::exec_command(const std::string& command) {
	Point source{ command.substr(0, 2) };
	Point destination{ command.substr(2, 2) }; 

	CHECK_AND_RETURN(
		is_point_on_board(source) && 
		is_point_on_board(destination),
		CommandResult::ILLEGAL_INDEXES);
	
	CHECK_AND_RETURN(
		source != destination, 
		CommandResult::DEST_SAME_AS_SOURCE);

	CHECK_AND_RETURN(
		exists_piece_at_point(source) &&
		board_at_point(source)->get_owner() == _turn, 
		CommandResult::SOURCE_HASNT_CURRENT_PLAYERS_PIECE);

	CHECK_AND_RETURN(
		!exists_piece_at_point(destination) ||
		board_at_point(destination)->get_owner() != _turn,
		CommandResult::INVALID_CURRENT_COLOR_IN_DEST);
	
	// TODO: bugfix
	CHECK_AND_RETURN(
		board_at_point(source)->is_reachable(destination, exists_piece_at_point(destination)) &&
		!is_move_interrupted(source, destination),
		CommandResult::ILLEGAL_MOVEMENT);

	bool is_check_mate = false;
	std::unique_ptr<Piece> original_piece_in_dest;

	if (exists_piece_at_point(destination))
	{
		is_check_mate = tolower(board_at_point(destination)->get_type()) == KING;
		original_piece_in_dest.reset(board_at_point(destination).release());
	}

	board_at_point(destination).reset(board_at_point(source).release());
	board_at_point(destination)->update_location(destination);

	for (size_t i = 0; i < BOARD_LENGTH && !is_check_mate; i++)
	{ //when check mate enemy, it's legal to have a self check
		for (size_t j = 0; j < BOARD_LENGTH; j++)
		{
			if (

				(_board[(i * BOARD_LENGTH) + j] != nullptr) &&
				(_board[(i * BOARD_LENGTH) + j]->get_owner() != _turn) &&
				(_board[(i * BOARD_LENGTH) + j]->is_reachable(get_current_player_king()->get_point(), true)) &&
				(!is_move_interrupted(_board[i * BOARD_LENGTH + j]->get_point(), get_current_player_king()->get_point())))
			{
				board_at_point(source).reset(board_at_point(destination).release());
				board_at_point(source)->update_location(source);

				board_at_point(destination).reset(original_piece_in_dest.release());
				return CommandResult::INVALID_SELF_CHESS;
			}
		}
	}

	/*If a pawn achived to max place on board, replace him with a dragon*/
	if ((tolower(board_at_point(destination)->get_type()) == PAWN) &&
		((destination.get_y() == BOARD_LENGTH - 1) || (destination.get_y() == 0))) 
	{
		board_at_point(destination).reset(new Dragon(_turn == Player::WHITE_PLAYER ? toupper(DRAGON) : tolower(DRAGON), destination));
	}

	if (_turn == Player::WHITE_PLAYER)
	{
		_turn = Player::BLACK_PLAYER;
	}
	else
	{
		_turn = Player::WHITE_PLAYER;
	}

	if (is_check_mate)
	{
		return CommandResult::MATE;
	}
	else if (is_check())
	{
		return CommandResult::CHESS;
	}
	else
	{
		return CommandResult::VALID_MOVE;
	}
}

const Piece* Game::get_current_player_king() const
{
	switch (_turn)
	{
	case WHITE_PLAYER: return _white_king;
	case BLACK_PLAYER: return _black_king;
	default: throw std::runtime_error("Invalid player");
	}
}

bool Game::is_point_on_board(const Point& point) const
{
	return 
		(point.get_x() >= 0) && (point.get_y() >= 0) &&
		(point.get_x() < BOARD_LENGTH) && (point.get_y() < BOARD_LENGTH);
}

bool Game::exists_piece_at_point(const Point& point) const
{
	return board_at_point(point) != nullptr;
}

std::unique_ptr<Piece>& Game::board_at_point(const Point& point)
{
	return _board[(point.get_y() * BOARD_LENGTH) + point.get_x()];
}

const std::unique_ptr<Piece>& Game::board_at_point(const Point& point) const
{
	return _board[(point.get_y() * BOARD_LENGTH) + point.get_x()];
}

void Game::draw() const {
	int i = 0;
	int j = 0;

	for (i = 0; i < BOARD_LENGTH; i++) {
		for (j = 0; j < BOARD_LENGTH; j++) {
			if (_board[i * BOARD_LENGTH + j] != nullptr) {
				std::cout << " " << _board[i * BOARD_LENGTH + j]->get_type();
			} else {
				std::cout << " " << EMPTY_CELL_CHAR;
			}
		}

		std::cout << std::endl;
	}

	std::cout << std::endl;
}

bool Game::is_move_interrupted(const Point& src, const Point& dst) const
{
	CHECK_AND_RETURN(exists_piece_at_point(src), true);

	const char src_piece_type = std::tolower(board_at_point(src)->get_type());
	CHECK_AND_RETURN(
		(src_piece_type != KING) &&
		(src_piece_type != KNIGHT) &&
		(src_piece_type != DRAGON),
		false);

	for (size_t y_index = 0; y_index < BOARD_LENGTH; y_index++)
	{
		for (size_t x_index = 0; x_index < BOARD_LENGTH; x_index++)
		{
			Point current_point{ x_index, y_index };
			if ((current_point == src) || (current_point == dst))
			{
				continue;
			}

			CHECK_AND_RETURN(
				exists_piece_at_point(current_point) &&
				IS_BETWEEN(current_point, src, dst),
				true);
		}
	}

	return false;
}

bool Game::is_check() const {
	int i = 0, j = 0;

	for (i = 0; i < BOARD_LENGTH; i++)  
	{
		for (j = 0; j < BOARD_LENGTH; j++) 
		{
			if (_board[i * BOARD_LENGTH + j] != nullptr) 
			{
				if (_board[i * BOARD_LENGTH + j]->get_owner() != Player::BLACK_PLAYER && _board[i * BOARD_LENGTH + j]->is_reachable(_black_king->get_point(), true) && !is_move_interrupted(_board[i * BOARD_LENGTH + j]->get_point(), _black_king->get_point())) 
				{
					return true;
				}
				else if (_board[i * BOARD_LENGTH + j]->get_owner() != Player::WHITE_PLAYER && _board[i * BOARD_LENGTH + j]->is_reachable(_white_king->get_point(), true) && !is_move_interrupted(_board[i * BOARD_LENGTH + j]->get_point(), _white_king->get_point())) 
				{
					return true;
				}
			}
		}
	}

	return false;
}

std::unique_ptr<Piece> Game::create_piece(char type, const Point& location)
{
	if (type == EMPTY_CELL_CHAR)
	{
		return nullptr;
	}

	switch (tolower(type))
	{
	case PAWN: return std::make_unique<Pawn>(type, location);
	case ROOK: return std::make_unique<Rook>(type, location);
	case KING: return std::make_unique<King>(type, location);
	case QUEEN: return std::make_unique<Queen>(type, location);
	case BISHOP: return std::make_unique<Bishop>(type, location);
	case KNIGHT: return std::make_unique<Knight>(type, location);
	case DRAGON: return std::make_unique<Dragon>(type, location);
	default: throw std::runtime_error("Invalid piece type");
	}
}
