#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <string>

enum Color {
	NONE = 0,
	RED = 1,
	BLUE = 2
};

enum Rotation {
	LEFT = 0,
	RIGHT = 1
};

struct BoardSquare {
	bool is_not_occupiable : 1;
	Color color : 2;
	Rotation rotation : 1;
	
	inline bool is_goal() const {return this->is_not_occupiable && this->color != Color::NONE;}
	inline bool is_piece() const {return !this->is_not_occupiable && this->color != Color::NONE;}
	inline bool is_empty() const {return !this->is_not_occupiable && this->color == Color::NONE;}
};

class Board {
	/*
	Y   76543210 (X)
	0	X    \\G
	1	X    \\\
	2	     \\\
	3	L       
	4	     ///
	5	X    ///
	6	X    //G
	*/

public:
	BoardSquare _board[7][8] = {};
	
	Board() {
		//Red
		this->_board[0][0] = BoardSquare{1,RED};
		this->_board[0][1] = BoardSquare{0,RED,LEFT};
		this->_board[0][2] = BoardSquare{0,RED,LEFT};
		this->_board[1][0] = BoardSquare{0,RED,LEFT};
		this->_board[1][1] = BoardSquare{0,RED,LEFT};
		this->_board[1][2] = BoardSquare{0,RED,LEFT};
		this->_board[2][0] = BoardSquare{0,RED,LEFT};
		this->_board[2][1] = BoardSquare{0,RED,LEFT};
		this->_board[2][2] = BoardSquare{0,RED,LEFT};
		//Blue
		this->_board[6][0] = BoardSquare{1,BLUE};
		this->_board[6][1] = BoardSquare{0,BLUE,RIGHT};
		this->_board[6][2] = BoardSquare{0,BLUE,RIGHT};
		this->_board[5][0] = BoardSquare{0,BLUE,RIGHT};
		this->_board[5][1] = BoardSquare{0,BLUE,RIGHT};
		this->_board[5][2] = BoardSquare{0,BLUE,RIGHT};
		this->_board[4][0] = BoardSquare{0,BLUE,RIGHT};
		this->_board[4][1] = BoardSquare{0,BLUE,RIGHT};
		this->_board[4][2] = BoardSquare{0,BLUE,RIGHT};
		//off limits
		this->_board[0][7] = BoardSquare{1,NONE};
		this->_board[1][7] = BoardSquare{1,NONE};
		this->_board[3][7] = BoardSquare{1,NONE};
		this->_board[5][7] = BoardSquare{1,NONE};
		this->_board[6][7] = BoardSquare{1,NONE};
	}
	
	Color winner() {
		int laser_x = 7, laser_y = 3;
		enum Direction {UP, DOWN, LEFT, RIGHT} laser_direction = RIGHT;
		
		//in python this would be `0 <= laser_x < 8 and 0 <= laser_y < 7` ugh its so much cleaner
		while (0 <= laser_x && laser_x < 8 && 0 <= laser_y && laser_y < 7) {
			const BoardSquare &laser_square = this->_board[laser_y][laser_x];
			if (laser_square.is_goal()) return laser_square.color;
			else if (laser_square.color != Color::NONE) {
				if (laser_square.rotation == Rotation::LEFT) {
					laser_direction = (Direction)(laser_direction^2);
				}
				else if (laser_square.rotation == Rotation::RIGHT) {
					laser_direction = (Direction)(laser_direction^3);
				}
			}
			if (laser_direction == Direction::UP) --laser_y;
			else if (laser_direction == Direction::DOWN) ++laser_y;
			else if (laser_direction == Direction::RIGHT) --laser_x;
			else if (laser_direction == Direction::LEFT) ++laser_x;
		}
		return Color::NONE;
	}
	
	BoardSquare& get_square(int x, int y) {
		if (x < 0 || x > 7) throw std::invalid_argument("x is out of bounds");
		if (y < 0 || y > 6) throw std::invalid_argument("y is out of bounds");
		return _board[y][x];
	}
	
	bool is_valid_move(int start_x, int start_y, int end_x, int end_y) {
		//TODO: this sucks
		if (!this->get_square(start_x, start_y).is_piece()) {
			return false;
			throw std::invalid_argument(std::string("no piece at coordinates ")+std::to_string(start_x)+","+std::to_string(start_y));
		}
		if (!this->get_square(end_x, end_y).is_empty()) {
			return false;
		}
		switch (abs(start_x-end_x)) {
			case 0: return true;
			case 1: return true;
			case 2: {
				if (abs(start_y-end_y)==2) {
					if (this->get_square((start_x+end_x)/2, (start_y+end_y)/2).is_piece() && this->get_square((start_x+end_x)/2, (start_y+end_y)/2).color != this->get_square((start_x)/2, (start_y)/2).color) {
						return true;
					}
					return false;
				} else return false;
			}
			default: return false;
		}
	}
	
	void move_piece(int start_x, int start_y, int end_x, int end_y) {
		//TODO - check if move is valid move
		if (is_valid_move(start_x, start_y, end_x, end_y)) {
			
		}
	}
	
	friend std::ostream& operator<<(std::ostream& stream, const Board& board) {
		for (int i = 0; i < 7; ++i) {
			for (int j = 7; j >= 0; --j) {
				const BoardSquare &p = board._board[i][j];
				if (p.is_goal()) {
					stream << 'G';
				}
				else if (p.is_not_occupiable) {
					stream << 'X';
				}
				else if (p.is_piece()) {
					if (p.rotation == RIGHT) {
						stream << '/';
					}
					else if (p.rotation == LEFT) {
						stream << '\\';
					}
				}
				else {
					stream << ' ';
				}
				
			}
			stream << std::endl;
		}
		return stream;
	}
};

int main(int argc, char **argv) {
	Board b = Board();
	std::cout << b << (int)b.winner();
	return 0;
}