#include "Pipe.hpp"
#include "Game.hpp"

#include <stdlib.h>
#include <iostream>

#define BOARD_FIRST_STATE "rnbkdqbnrppppppppp#############################################PPPPPPPPPRNBKDQBNR"

void copy_string_to_char_array(char* arr, std::string str) {
	int i = 0;

	for (i = 0; i < str.length(); i++) {
		arr[i] = str[i];
	}

	arr[i] = 0; //arr[str.length()] should be '\0' char
}

int main()
{

	static constexpr size_t BOARD_LENGTH = 9;
	Game game = Game(BOARD_FIRST_STATE, BOARD_LENGTH);

	std::string ans = "";
	std::string msg_from_graphics = "";
	char msg_to_graphics[1024] = { 0 }; // msgToGraphics should contain the board string accord the protocol

	system("start chessGraphics.exe");
	Sleep(2000);

	Pipe p = Pipe();
	while (!p.connect()) {
		std::cout << "Can't connect to graphics." << std::endl;
		std::cout << "If you wish to try again enter 0, else enter any other key: ";
		std::cin >> ans;

		if (ans[0] == '0') {
			std::cout << "Trying to connect again.." << std::endl;
			Sleep(5000);
		} else {
			p.close();
			return 0;
		}
	}

	copy_string_to_char_array(msg_to_graphics, game.to_string()); //msgToGraphics = first_board_as_string + color_of_first_player -> always white
	p.send_message_to_graphics(msg_to_graphics);   // send the board string

	msg_from_graphics = p.get_message_from_graphics(); //get message from graphics
	std::cout << "Current board is:" << std::endl;
	game.draw(); //draw first board

	msg_to_graphics[1] = 0; //make string end at second char (one letter + '\0' char)

	while (msg_from_graphics != "quit") {
		msg_to_graphics[0] = game.exec_command(msg_from_graphics) + '0'; // handle the string the sent from graphics according to the protocol

		std::cout << "Current board is:" << std::endl;
		game.draw(); //draw board after command was executed

		p.send_message_to_graphics(msg_to_graphics); // return result to graphics
		msg_from_graphics = p.get_message_from_graphics(); // get message from graphics
	}

	p.close();
	return 0;
}
