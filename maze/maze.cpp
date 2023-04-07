/**
 * @brief implementation of maze.h
 * @author Kyle Huang
 * @file maze.cpp
 * @version 12.5
 * @date 5/9/2022
**/

#include "maze.h"

// CONSTRUCTOR/DESTRUCTOR

/**
 * @brief Default-Arg Constructor: a new Maze Element:: Maze Element object
 * @param data maze cell block character: ' ' or empty char as a path,
 *             '#' as a wall, 'o' as the goal/target, '.' as a marker (visited)
 * PRE: data is optional
 * POST: creates a Maze element
 */
MazeElement::MazeElement(const char& data) : data{ data } {}

/**
 * @brief Constructor: reads the maze file to fill the 2D array maze
 * @param fileName name of the maze file
 * PRE: requires a file
 * POST: reads the maze
**/
Maze::Maze() {}

/**
 * @brief Destructor: deletes the 2D array when it is out of scope
 * PRE: None
 * POST: clears the 2D maze array
 */
Maze::~Maze() {
	this->clear2DMaze();
}

// PUBLIC METHOD

/**
 * @brief reads the maze file to fill 2D array maze
 * @param fileName name of the maze file
 * First line of the maze file contains the row, size, and start coordinate pair
 * The first two numbers are the numbers of rows and columns
 * The last two numbers of the third and fourth are the start coordinate of
 * the y-coordinate (row) and the x-coordinate (column)
 * PRE: requires a file
 * POST: reads the maze file, creates the 2D maze array, then solves it.
 *       Also, checks if the file has correct information.
 */
void Maze::readMazeFile(const std::string& fileName) {
	this->error = false; // reset error to false
	this->clear2DMaze(); // clear array
	// this->create2DMaze(fileName); // create the 2D array first
	std::ifstream file(fileName); // file inputstream
	std::string line; // line from file
	std::getline(file, line); // skip first line
	unsigned int currRow{}; // file line index or row
	bool hasExit{};
	std::string num;
	std::stringstream s(line);
	unsigned count{};

	// try and catch starter line parsing error
	try {
		// set row, col, and start coordinate
		while (s >> num) {
			if (count == 0) this->row = stoi(num);
			else if (count == 1) this->col = stoi(num);
			else if (count == 2) this->startY = stoi(num);
			else if (count == 3) this->startX = stoi(num);
			else break;
			++count;
			if (count >= 4) break;
		}
	}
	catch (...) {
		std::cout << "Error: when reading the starter line"
			<< " of the size and start coordinate of the maze\n";
		this->error = true;
		return;
	}


	// if size is 0, below, or out of range, then exit program
	if (this->row <= 0 || this->col <= 0 || this->row > UINT_MAX
		|| this->col > UINT_MAX) {
		std::cout << "Error: row and column size have to be at least 1"
			<< " or the size is too big\n";
		this->error = true;
	}

	// if start coordinate is out of bounds, exit program
	if (this->startY < 0 || this->startX < 0 || this->startY >= this->row
		|| this->startX >= this->col) {
		std::cout << "Error: start coordinate is out of bounds\n";
		this->error = true;
	}

	if (this->error) return;

	// create 2D maze array
	if (this->mazeArray == nullptr) {
		this->mazeArray = new MazeElement * [row];
		for (int i = 0; i < row; ++i) this->mazeArray[i] = new MazeElement[col];
	}

	// if file exists and accessible to read
	if (file.is_open()) {
		// loop the file line
		while (std::getline(file, line) && currRow < this->row) {
			// std::cout << line << std::endl;
			unsigned int currCol{}; // line character index
			// loop through each character of the line
			while (currCol < col) {
				// add maze elements in the 2D maze array
				MazeElement temp(line[currCol]);
				this->mazeArray[currRow][currCol] = temp;
				// store the end/exit/target coordinates
				if (line[currCol] == 'o') {
					hasExit = true;
					this->endY = currRow;
					this->endX = currCol;
				}
				++currCol;
			}
			++currRow;
		}
		// if file does not exists or not accessible to read, then end program
	}
	else {
		std::cout << "Error: no file exception\n";
		this->error = true;
		return;
	}

	// if no exit, end program
	if (!hasExit) {
		std::cout << "Error: no exit\n";
		this->error = true;
		return;
	}

	if (this->error) return;

	file.close(); // close the file stream
	this->solveMaze(); // solve maze
}

// PRIVATE METHODS

/**
 * @brief facade maze solver
 * PRE: None
 * POST: calls a helper functions to solve the maze
**/
void Maze::solveMaze() {
	this->solveMazeHelper(this->startY, this->startX, 0);
	this->backtrackHelper(this->endY, this->endX);
}

/**
 * @brief solves the maze using recursion
 * @return true if the maze has been solved
 * @return false if the maze has not been solved
 * PRE: requires row, column, and steps integers
 * POST: finds the shortest path to the exit
 */
bool Maze::solveMazeHelper(unsigned int row, unsigned int col,
	unsigned int steps) {
	if (this->isInBounds(row, col)) {
		// if the maze element is not visited
		if (this->mazeArray[row][col].isVisited == false) {
			// mark the current maze element as visited and assign the
			// number of steps
			this->mazeArray[row][col].isVisited = true; // mark visited
			this->mazeArray[row][col].steps = steps; // mark distance/steps
			this->mazeArray[row][col].isOnPath = true; // mark on path

			// check the current maze element surroundings for an 
			// available path to traverse to
			bool up = this->solveMazeHelper(row - 1, col, steps + 1);
			bool right = this->solveMazeHelper(row, col + 1, steps + 1);
			bool down = this->solveMazeHelper(row + 1, col, steps + 1);
			bool left = this->solveMazeHelper(row, col - 1, steps + 1);

			// if there is a path open, return true
			if (up || right || down || left) return true;

			// no path, mark false
			this->mazeArray[row][col].isOnPath = false;
			// if the maze element has been visited
		}
		else {
			// if it finds a shorter number of steps, then assign the new
			// step value to this current maze element
			if (steps < this->mazeArray[row][col].steps) {
				this->mazeArray[row][col].steps = steps; // update steps
				this->mazeArray[row][col].isOnPath = true; // mark on path

				// traverse through all directions
				bool up = this->solveMazeHelper(row - 1, col, steps + 1);
				bool right = this->solveMazeHelper(row, col + 1, steps + 1);
				bool down = this->solveMazeHelper(row + 1, col, steps + 1);
				bool left = this->solveMazeHelper(row, col - 1, steps + 1);

				// if there is a path open, return true
				if (up || right || down || left) return true;

				// no path, mark false
				this->mazeArray[row][col].isOnPath = false;
				// if it finds a greater or equal number of steps, then return false
			}
			else return false;
		}
		// if exit is found, it is done
		if (this->mazeArray[row][col].data == 'o') return true;
	}
	return false; // if out of bounds, return false
}

/**
 * @brief once it reaches the exit, it backtracks to the element with the least
 *        amounts of steps and it assigns it with
 *        a dot to mark the shortest path
 * @param row row or y-coordinate
 * @param col column or x-coordinate
 */
void Maze::backtrackHelper(unsigned int row, unsigned int col) {
	// if the current coordinate matches the start coordinate, stop.
	if (row == this->startY && col == this->startX) return;

	// calculate minimum steps from all 4 directions
	// up
	unsigned long int up = ULONG_MAX;
	if (this->isInBounds(row - 1, col))
		up = this->mazeArray[row - 1][col].steps;
	// right
	unsigned long int right = ULONG_MAX;
	if (this->isInBounds(row, col + 1))
		right = this->mazeArray[row][col + 1].steps;
	// down
	unsigned long int down = ULONG_MAX;
	if (this->isInBounds(row + 1, col))
		down = this->mazeArray[row + 1][col].steps;
	// left
	unsigned long int left = ULONG_MAX;
	if (this->isInBounds(row, col - 1))
		left = this->mazeArray[row][col - 1].steps;

	// make changes to nearest element with the least amount of steps
	// if up has the least amount of steps, print a path marker
	if (up <= right && up <= down && up <= left
		&& this->mazeArray[row - 1][col].isOnPath) {
		this->mazeArray[row - 1][col].data = '.';
		return this->backtrackHelper(row - 1, col);
	}
	// if right has the least amount of steps, print a path marker
	if (right <= down && right <= left && right <= up
		&& this->mazeArray[row][col + 1].isOnPath) {
		this->mazeArray[row][col + 1].data = '.';
		return this->backtrackHelper(row, col + 1);
	}
	// if down has the least amount of steps, print a path marker
	if (down <= left && down <= up && down <= right
		&& this->mazeArray[row + 1][col].isOnPath) {
		this->mazeArray[row + 1][col].data = '.';
		return this->backtrackHelper(row + 1, col);
	}
	// if left has the least amount of steps, print a path marker
	if (left <= up && left <= right && left <= down
		&& this->mazeArray[row][col - 1].isOnPath) {
		this->mazeArray[row][col - 1].data = '.';
		return this->backtrackHelper(row, col - 1);
	}
}

/**
 * @brief checks if the coordinate is in bounds and is not a wall of the 2D
 *        maze array. If so, return true. Otherwise, return false.
 *	      (y, x) coordinate pair.
 * @param y row
 * @param x column
 * @return true if coordinate is in bounds
 * @return false if coordinate is out of bounds
 * PRE: requires a coordinate pair to check
 * POST: returns true if the coordinate is in bounds. Otherwise, returns false.
 */
bool Maze::isInBounds(const unsigned int& y, const unsigned int& x) {
	if (y < 0 || y >= this->row || x < 0 || x >= this->col
		|| this->mazeArray[y][x].data == '#')
		return false;
	return true;
}

/**
 * @brief prints the characters (walls, path, spaces, start & end
 *        coordinates) of the maze
 * PRE: None
 * POST: prints the 2D maze array
**/
void Maze::printMaze() {
	if (this->error) return;
	for (unsigned int i = 0; i < this->row; ++i) {
		for (unsigned int j = 0; j < this->col; ++j) {
			// start is blank
			if (i == this->startY && j == this->startX)
				std::cout << "\033[1;34m" << ' ' << "\033[0m";
			// exit is green
			else if (this->mazeArray[i][j].data == 'o')
				std::cout << "\033[1;32m" << this->mazeArray[i][j].data
				<< "\033[0m";
			// path is red
			else if (this->mazeArray[i][j].data == '.')
				std::cout << "\033[1;31m" << this->mazeArray[i][j].data
				<< "\033[0m";
			// walls are white
			else std::cout << this->mazeArray[i][j].data;
		}
		std::cout << std::endl;
	}
}

/**
 * @brief clears the 2D maze array
 * PRE: None
 * POST: clears the 2D maze array
 */
void Maze::clear2DMaze() {
	// delete 2D array
	if (this->mazeArray != nullptr) {
		for (int i = 0; i < row; ++i) {
			delete[] this->mazeArray[i];
			this->mazeArray[i] = nullptr;
		}
		delete[] this->mazeArray;
		this->mazeArray = nullptr;
	}

	// reset values to default
	this->startY = 0;
	this->startX = 0;
	this->row = 0;
	this->col = 0;
}

// PUBLIC GETTERS AND SETTER(S) METHODS

/**
 * @brief get row size
 * @return int
 * PRE: None
 * POST: returns the row size
 */
unsigned int Maze::getRowSize() {
	return this->row;
}

/**
 * @brief get column size
 * @return int
 * PRE: None
 * POST: returns the column size
 */
unsigned int Maze::getColumnSize() {
	return this->row;
}

/**
 * @brief get start y-coordinate
 * @return int
 * PRE: None
 * POST: returns the start y-coordinate
 */
unsigned int Maze::getStartY() {
	return this->startY;
}

/**
 * @brief get start x-coordinate
 * @return int
 * PRE: None
 * POST: returns the start x-coordinate
 */
unsigned int Maze::getStartX() {
	return this->startX;
}

/**
 * @brief get end/exit y-coordinate
 * @return int
 * PRE: None
 * POST: returns the end/exit y-coordinate
 */
unsigned int Maze::getEndY() {
	return this->startY;
}

/**
 * @brief get end/exit x-coordinate
 * @return int
 * PRE: None
 * POST: returns the end/exit x-coordinate
 */
unsigned int Maze::getEndX() {
	return this->startX;
}

/**
 * @brief Set the start coordinate
 * @param row row/y-coordinate
 * @param col column/x-coordinate
 * PRE: requires the new start y and x coordinate
 * POST: set new start coordinate
 */
void Maze::setStartCoordinate(const unsigned int& y, const unsigned int& x) {
	if (this->isInBounds(y, x)) {
		this->startY = y;
		this->startX = x;
	}
	else std::cout << "Error: invalid start coordinate\n";
}

/**
 * @brief Set the end/exit coordinate
 * @param row row/y-coordinate
 * @param col column/x-coordinate
 * PRE: requires the new end/exit y and x coordinate
 * POST: set new end/exit coordinate
 */
void Maze::setEndCoordinate(const unsigned int& y, const unsigned int& x) {
	if (this->isInBounds(y, x)) {
		// remove previous end/exit value and patch it with a blank character
		this->mazeArray[this->endY][this->endX].data = ' ';

		// assign new end/exit coordinate
		this->endY = y;
		this->endX = x;

		// assign the 'o' end/exit character to the new coordinate
		this->mazeArray[this->endY][this->endX].data = 'o';
	}
	else std::cout << "Error: invalid end/exit coordinate\n";
}