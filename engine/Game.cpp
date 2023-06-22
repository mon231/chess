#include "Game.h"

Game::Game() :
	_turn(Players::WHITE_PLAYER), _board(std::vector<Piece*>()), _white_king(nullptr), _black_king(nullptr)
{}

Game::~Game() {
	int i = 0;

	for (i = this->_board.size() - 1; i >= 0; i--) {
		delete this->_board[i];
		this->_board.pop_back();
	}
}

std::string Game::parse_string() const {
	std::string res = "";
	int i = 0;
	int j = 0;

	for (i = 0; i < BOARD_LENGTH; i++) {
		for (j = 0; j < BOARD_LENGTH; j++) {
			if (this->_board[i * BOARD_LENGTH + j] == nullptr) {
				res += EMPTY_CELL_CHAR;
			} else {
				res += this->_board[i * BOARD_LENGTH + j]->get_type();
			}
		}
	}

	res += this->_turn + '0';
	return res;
}

void Game::init(const char* board) {
	int i = 0; //y
	int j = 0; //x
	std::string loc = "##"; //create std::string with place for x and y (2 char length std::string)

	for (i = 0; i < BOARD_LENGTH; i++) {
		for (j = 0; j < BOARD_LENGTH; j++) {
			loc[0] = FIRST_INDEX_VALUE_OF_X + j;
			loc[1] = FIRST_INDEX_VALUE_OF_Y - i;

			switch (tolower(board[i * BOARD_LENGTH + j])) {
			case QUEEN:
				this->_board.push_back((Piece*)new Queen(board[i * BOARD_LENGTH + j], loc));
				break;

			case KING:
				this->_board.push_back((Piece*)new King(board[i * BOARD_LENGTH + j], loc));

				if (this->_board[this->_board.size() - 1]->get_owner() == Players::BLACK_PLAYER) {
					this->_black_king = this->_board[this->_board.size() - 1];
				} else { //if last added piece (king) owned to the white player
					this->_white_king = this->_board[this->_board.size() - 1];
				}

				break;

			case PAWN:
				this->_board.push_back((Piece*)new Pawn(board[i * BOARD_LENGTH + j], loc));
				break;

			case ROOK:
				this->_board.push_back((Piece*)new Rook(board[i * BOARD_LENGTH + j], loc));
				break;

			case BISHOP:
				this->_board.push_back((Piece*)new Bishop(board[i * BOARD_LENGTH + j], loc));
				break;

			case KNIGHT:
				this->_board.push_back((Piece*)new Knight(board[i * BOARD_LENGTH + j], loc));
				break;

			case DRAGON:
				this->_board.push_back((Piece*)new Dragon(board[i * BOARD_LENGTH + j], loc));
				break;

			case EMPTY_CELL_CHAR:
				this->_board.push_back(nullptr);
				break;
			}
		}
	}
}

ResultOfCommand Game::exec_command(const std::string& command) {
	int i = 0, j = 0;

	Point source = Point(), destination = Point(), middle = Point();
	std::string src = command.substr(0, 2);
	std::string dst = command.substr(2, 2);
	
	bool is_mate = false; //when eating king piece, not bonus
	size_t src_x = src[INDEX_OF_X] - FIRST_INDEX_VALUE_OF_X, src_y = FIRST_INDEX_VALUE_OF_Y - src[INDEX_OF_Y], dst_x = dst[INDEX_OF_X] - FIRST_INDEX_VALUE_OF_X, dst_y = FIRST_INDEX_VALUE_OF_Y - dst[INDEX_OF_Y];
	Piece* original_piece_in_dest = nullptr;

	if (src_x < 0 || src_y < 0 || dst_x < 0 || dst_y < 0 || src_x >= BOARD_LENGTH || src_y >= BOARD_LENGTH || dst_x >= BOARD_LENGTH || dst_y >= BOARD_LENGTH) {
		return ResultOfCommand::ILLEGAL_INDEXES;
	}

	if (dst == src) {
		return ResultOfCommand::DEST_SAME_AS_SOURCE;
	}

	if (this->_board[src_y * BOARD_LENGTH + src_x] == nullptr || this->_board[src_y * BOARD_LENGTH + src_x]->get_owner() != this->_turn) {
		return ResultOfCommand::SOURCE_HASNT_CURRENT_PLAYERS_PIECE;
	}

	if (this->_board[dst_y * BOARD_LENGTH + dst_x] != nullptr && this->_board[dst_y * BOARD_LENGTH + dst_x]->get_owner() == this->_turn) {
		return ResultOfCommand::INVALID_CURRENT_COLOR_IN_DEST;
	}

	if (!this->_board[src_y * BOARD_LENGTH + src_x]->is_affordable(Point{ dst }, this->_board[dst_y * BOARD_LENGTH + dst_x] != nullptr) || this->is_move_interrupted(Point{ src }, Point{ dst })) {
		return ResultOfCommand::ILLEGAL_MOVEMENT;
	}

	if (this->_board[dst_y * BOARD_LENGTH + dst_x] != nullptr) { //if was there a piece
		is_mate = tolower(this->_board[dst_y * BOARD_LENGTH + dst_x]->get_type()) == KING; //if eaten piece was king
		original_piece_in_dest = this->_board[dst_y * BOARD_LENGTH + dst_x]; //piece which will need to be free if movement is legal
	}

	this->_board[dst_y * BOARD_LENGTH + dst_x] = this->_board[src_y * BOARD_LENGTH + src_x]; //set new location index point to
	this->_board[src_y * BOARD_LENGTH + src_x] = nullptr; //set src (which current piece moved from) to null (there isn't any piece there)
	this->_board[dst_y * BOARD_LENGTH + dst_x]->update_location(dst); //update location of moved piece to it's new placement on the board

	for (i = 0; i < BOARD_LENGTH && !is_mate; i++) { //when check mate enemy, it's legal to have a self check
		for (j = 0; j < BOARD_LENGTH; j++) {
				/*if there is a piece in current cell*/			/*and it piece is owned to the enemy (other player)*/				/*and it piece can afford the king of current player (who played last turn which called to this method), when it is known that the king exists there, else was mate*/ /*and there isn't any piece which prevent him to eat the king in his turn (no interrupts for moving from it's current point to the king's point)*/
			if (this->_board[i * BOARD_LENGTH + j] != nullptr && this->_board[i * BOARD_LENGTH + j]->get_owner() != this->_turn && this->_board[i * BOARD_LENGTH + j]->is_affordable(this->_turn == Players::WHITE_PLAYER ? this->_white_king->get_point() : this->_black_king->get_point(), true) && !this->is_move_interrupted(this->_board[i * BOARD_LENGTH + j]->get_point(), this->_turn == Players::WHITE_PLAYER ? this->_white_king->get_point() : this->_black_king->get_point())) {
				this->_board[dst_y * BOARD_LENGTH + dst_x]->update_location(src);						 //make piece which moved point be it's original value
				this->_board[src_y * BOARD_LENGTH + src_x] = this->_board[dst_y * BOARD_LENGTH + dst_x]; //return pieces
				this->_board[dst_y * BOARD_LENGTH + dst_x] = original_piece_in_dest;					 //to their original position
				return ResultOfCommand::INVALID_SELF_CHESS;
			}
		}
	}
	
	/*If a pawn achived to max place on board, replace him with a dragon*/
	if ((dst_y == BOARD_LENGTH - 1 || dst_y == 0) && tolower(this->_board[dst_y * BOARD_LENGTH + dst_x]->get_type()) == PAWN) {
		delete this->_board[dst_y * BOARD_LENGTH + dst_x];
		this->_board[dst_y * BOARD_LENGTH + dst_x] = (Piece*)new Dragon(this->_turn == Players::WHITE_PLAYER ? toupper(DRAGON) : tolower(DRAGON), dst);
	}

	if (original_piece_in_dest != nullptr) { //if was a piece in original dst point
		delete original_piece_in_dest;		 //free it
	}

	if (this->_turn == Players::WHITE_PLAYER) { //change
		this->_turn = Players::BLACK_PLAYER;	//turn
	} else { //if this->_turn was Players::BLACK_PLAYER
		this->_turn = Players::WHITE_PLAYER;
	}

	//if arrived here, move was valid.

	if (is_mate) {
		return ResultOfCommand::MATE;
	} else if (this->is_check()) {
		return ResultOfCommand::CHESS;
	} else {
		return ResultOfCommand::VALID_MOVE;
	}
}

void Game::draw() const {
	int i = 0;
	int j = 0;

	for (i = 0; i < BOARD_LENGTH; i++) {
		for (j = 0; j < BOARD_LENGTH; j++) {
			if (this->_board[i * BOARD_LENGTH + j] != nullptr) {
				std::cout << " " << this->_board[i * BOARD_LENGTH + j]->get_type();
			} else {
				std::cout << " " << EMPTY_CELL_CHAR;
			}
		}

		std::cout << std::endl;
	}

	std::cout << std::endl;
}

bool Game::is_move_interrupted(const Point& src, const Point& dst) const {
	int i = 0;
	int j = 0;
	int src_x = src.get_x(), src_y = src.get_y();
	std::string mid = "##"; //new string 2 char length
	Point middle = Point();

	if (this->_board[src_y * BOARD_LENGTH + src_x] == nullptr) {
		return true;
	}

	for (i = 0; i < BOARD_LENGTH; i++) {
		for (j = 0; j < BOARD_LENGTH; j++) {
			if (this->_board[i * BOARD_LENGTH + j] != nullptr) { //if there is cell with a piece
				mid[INDEX_OF_Y] = FIRST_INDEX_VALUE_OF_Y - i;
				mid[INDEX_OF_X] = FIRST_INDEX_VALUE_OF_X + j;

				middle.init_point(mid);

				if (middle != src && middle != dst) {
					switch (tolower(this->_board[src_y * BOARD_LENGTH + src_x]->get_type())) { //if type of src piece need to be checked -> y = ax + b
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
			if (this->_board[i * BOARD_LENGTH + j] != nullptr) {
				if (this->_board[i * BOARD_LENGTH + j]->get_owner() != Players::BLACK_PLAYER && this->_board[i * BOARD_LENGTH + j]->is_affordable(this->_black_king->get_point(), true) && !this->is_move_interrupted(this->_board[i * BOARD_LENGTH + j]->get_point(), this->_black_king->get_point())) {
					return true;
				} else if (this->_board[i * BOARD_LENGTH + j]->get_owner() != Players::WHITE_PLAYER && this->_board[i * BOARD_LENGTH + j]->is_affordable(this->_white_king->get_point(), true) && !this->is_move_interrupted(this->_board[i * BOARD_LENGTH + j]->get_point(), this->_white_king->get_point())) {
					return true;
				}
			}
		}
	}

	return false;
}
