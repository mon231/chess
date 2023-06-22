#include "Game.h"

Game::Game(const char* board) :
	_turn(Player::WHITE_PLAYER)
{
	size_t y_index = 0;
	size_t x_index = 0;
	std::string loc = "##"; //create std::string with place for x and y (2 char length std::string)

	for (y_index = 0; y_index < BOARD_LENGTH; y_index++) {
		for (x_index = 0; x_index < BOARD_LENGTH; x_index++) {
			loc[0] = FIRST_INDEX_VALUE_OF_X + x_index;
			loc[1] = FIRST_INDEX_VALUE_OF_Y - y_index;

			switch (tolower(board[y_index * BOARD_LENGTH + x_index]))
			{
			case QUEEN:
				_board.emplace_back(new Queen(board[y_index * BOARD_LENGTH + x_index], loc));
				break;

			case KING:
				_board.emplace_back(new King(board[y_index * BOARD_LENGTH + x_index], loc));

				if (_board[_board.size() - 1]->get_owner() == Player::BLACK_PLAYER)
				{
					_black_king = _board[_board.size() - 1].get();
				}
				else
				{
					_white_king = _board[_board.size() - 1].get();
				}

				break;

			case PAWN:
				_board.emplace_back(new Pawn(board[y_index * BOARD_LENGTH + x_index], loc));
				break;

			case ROOK:
				_board.emplace_back(new Rook(board[y_index * BOARD_LENGTH + x_index], loc));
				break;

			case BISHOP:
				_board.emplace_back(new Bishop(board[y_index * BOARD_LENGTH + x_index], loc));
				break;

			case KNIGHT:
				_board.emplace_back(new Knight(board[y_index * BOARD_LENGTH + x_index], loc));
				break;

			case DRAGON:
				_board.emplace_back(new Dragon(board[y_index * BOARD_LENGTH + x_index], loc));
				break;

			case EMPTY_CELL_CHAR:
				_board.emplace_back();
				break;
			}
		}
	}
}

std::string Game::parse_string() const {
	std::string res = "";
	int i = 0;
	int j = 0;

	for (i = 0; i < BOARD_LENGTH; i++) {
		for (j = 0; j < BOARD_LENGTH; j++) {
			if (_board[i * BOARD_LENGTH + j] == nullptr) {
				res += EMPTY_CELL_CHAR;
			} else {
				res += _board[i * BOARD_LENGTH + j]->get_type();
			}
		}
	}

	res += _turn + '0';
	return res;
}

CommandResult Game::exec_command(const std::string& command) {
	Point source = Point(), destination = Point(), middle = Point();
	std::string src = command.substr(0, 2);
	std::string dst = command.substr(2, 2);

	bool is_check_mate = false;
	size_t src_x = src[INDEX_OF_X] - FIRST_INDEX_VALUE_OF_X, src_y = FIRST_INDEX_VALUE_OF_Y - src[INDEX_OF_Y], dst_x = dst[INDEX_OF_X] - FIRST_INDEX_VALUE_OF_X, dst_y = FIRST_INDEX_VALUE_OF_Y - dst[INDEX_OF_Y];

	if ((src_x < 0) || (src_y < 0) || (dst_x < 0 || dst_y < 0) ||
		(src_x >= BOARD_LENGTH) || (src_y >= BOARD_LENGTH) ||
		(dst_x >= BOARD_LENGTH) || (dst_y >= BOARD_LENGTH))
	{
		return CommandResult::ILLEGAL_INDEXES;
	}

	if (dst == src)
	{
		return CommandResult::DEST_SAME_AS_SOURCE;
	}

	if (_board[src_y * BOARD_LENGTH + src_x] == nullptr || _board[src_y * BOARD_LENGTH + src_x]->get_owner() != _turn)
	{
		return CommandResult::SOURCE_HASNT_CURRENT_PLAYERS_PIECE;
	}

	if (_board[dst_y * BOARD_LENGTH + dst_x] != nullptr && _board[dst_y * BOARD_LENGTH + dst_x]->get_owner() == _turn)
	{
		return CommandResult::INVALID_CURRENT_COLOR_IN_DEST;
	}

	if (!_board[src_y * BOARD_LENGTH + src_x]->is_reachable(Point{ dst }, _board[dst_y * BOARD_LENGTH + dst_x] != nullptr) || is_move_interrupted(Point{ src }, Point{ dst })) {
		return CommandResult::ILLEGAL_MOVEMENT;
	}

	std::unique_ptr<Piece> original_piece_in_dest;

	if (_board[dst_y * BOARD_LENGTH + dst_x] != nullptr)
	{
		is_check_mate = tolower(_board[dst_y * BOARD_LENGTH + dst_x]->get_type()) == KING;
		original_piece_in_dest.reset(_board[dst_y * BOARD_LENGTH + dst_x].release());
	}

	_board[dst_y * BOARD_LENGTH + dst_x].reset(_board[src_y * BOARD_LENGTH + src_x].release());
	_board[dst_y * BOARD_LENGTH + dst_x]->update_location(dst);

	for (size_t i = 0; i < BOARD_LENGTH && !is_check_mate; i++)
	{ //when check mate enemy, it's legal to have a self check
		for (size_t j = 0; j < BOARD_LENGTH; j++)
		{
			if (
				_board[(i * BOARD_LENGTH) + j] != nullptr &&
				_board[(i * BOARD_LENGTH) + j]->get_owner() != _turn &&
				_board[i * BOARD_LENGTH + j]->is_reachable(_turn == Player::WHITE_PLAYER ? _white_king->get_point() : _black_king->get_point(), true) && !is_move_interrupted(_board[i * BOARD_LENGTH + j]->get_point(), _turn == Player::WHITE_PLAYER ? _white_king->get_point() : _black_king->get_point())) {
				_board[(dst_y * BOARD_LENGTH) + dst_x]->update_location(src);
				_board[(src_y * BOARD_LENGTH) + src_x].reset(_board[dst_y * BOARD_LENGTH + dst_x].get());
				_board[(dst_y * BOARD_LENGTH) + dst_x].reset(original_piece_in_dest.release());
				return CommandResult::INVALID_SELF_CHESS;
			}
		}
	}

	/*If a pawn achived to max place on board, replace him with a dragon*/
	if ((dst_y == BOARD_LENGTH - 1 || dst_y == 0) && tolower(_board[dst_y * BOARD_LENGTH + dst_x]->get_type()) == PAWN) {
		_board[dst_y * BOARD_LENGTH + dst_x].reset(new Dragon(_turn == Player::WHITE_PLAYER ? toupper(DRAGON) : tolower(DRAGON), dst));
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
	int i = 0;
	int j = 0;
	int src_x = src.get_x(), src_y = src.get_y();
	std::string mid = "##"; //new string 2 char length
	Point middle = Point();

	if (_board[src_y * BOARD_LENGTH + src_x] == nullptr) {
		return true;
	}

	for (i = 0; i < BOARD_LENGTH; i++) {
		for (j = 0; j < BOARD_LENGTH; j++) {
			if (_board[i * BOARD_LENGTH + j] != nullptr) { //if there is cell with a piece
				mid[INDEX_OF_Y] = FIRST_INDEX_VALUE_OF_Y - i;
				mid[INDEX_OF_X] = FIRST_INDEX_VALUE_OF_X + j;

				middle.init_point(mid);

				if (middle != src && middle != dst) {
					switch (tolower(_board[src_y * BOARD_LENGTH + src_x]->get_type())) { //if type of src piece need to be checked -> y = ax + b
					case KNIGHT: //knight can skip other pieces
					case DRAGON: //dragon can also skip other pieces
					case KING:	 //king can move only one step, so he can't accidentally try skiping other piece while his movement is valid
						break;

					default:
						if (IS_BETWEEN(middle, src, dst)) { //Queen, Rook and Bishop moves in a straight line. Also pawn on first step might accidentally skip one piece
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

bool Game::is_check() const {
	int i = 0, j = 0;

	for (i = 0; i < BOARD_LENGTH; i++) {
		for (j = 0; j < BOARD_LENGTH; j++) {
			if (_board[i * BOARD_LENGTH + j] != nullptr) {
				if (_board[i * BOARD_LENGTH + j]->get_owner() != Player::BLACK_PLAYER && _board[i * BOARD_LENGTH + j]->is_reachable(_black_king->get_point(), true) && !is_move_interrupted(_board[i * BOARD_LENGTH + j]->get_point(), _black_king->get_point())) {
					return true;
				}
				else if (_board[i * BOARD_LENGTH + j]->get_owner() != Player::WHITE_PLAYER && _board[i * BOARD_LENGTH + j]->is_reachable(_white_king->get_point(), true) && !is_move_interrupted(_board[i * BOARD_LENGTH + j]->get_point(), _white_king->get_point())) {
					return true;
				}
			}
		}
	}

	return false;
}
