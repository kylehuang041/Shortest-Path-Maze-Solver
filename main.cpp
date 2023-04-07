/**
 * @brief Test the maze file
 * @author Kyle Huang
 * @file test.cpp
 * @version 3.5
 * @date 5/9/2022
**/

#include <filesystem> // access file directory
#include "maze/maze.h" // import maze
using namespace std;

int main() {
	Maze m1;
	string mazeFilePath = "./mazeFiles";

	for (const auto &file: std::filesystem::directory_iterator(mazeFilePath)) {
		m1.readMazeFile(file.path());
		m1.printMaze();
		cout << endl;
	}
}