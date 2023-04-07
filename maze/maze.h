/**
 * @brief interface-like for maze.cpp
 * @author Kyle Huang
 * @file maze.h
 * @version 7.8
 * @date 5/9/2022
**/

// double head guard: avoid duplication of this file
#pragma once
#ifndef MAZE_H
#define MAZE_H

// packages
#include <iostream> // console input/output
#include <fstream> // reading/writing from/to a file
#include <sstream> // string stream used for splitting
#include <climits> // constants with limits of fundamental integral types

/**
 * @brief stores a char data for maze cell block and a boolean if it is
 *        visited or not. For more information, check the cpp file.
 *
 *        Maze array key:
 *        ' ' or blank-spaced char as a path
 *        '#' as a wall
 *        'o' as the goal/target/exit
 *        '.' as a marker that leads to the shortest path
**/
struct MazeElement {
    friend class Maze;
private:
    // stores maze cell block character
    char data{};

    // stores the steps into the current element
    unsigned int steps{};

    // marks the maze cell block if it was visited or not
    bool isVisited{};

    // marks if it is a path or not
    bool isOnPath{};

    // Default-Arg Constructor: assigns data
    MazeElement(const char& data = ' ');
};

/**
 * @brief Maze using 2D array where it finds a shortest path to the goal
 *        with recursion.
 *
 *        The maze is from a text file: first line is the starter text that
 *        contains the grid size and start coordinate. The first number of the
 *        coordinate pair is the y-coordinate and the second number is the
 *        x-coordinate.
 *
 *                           (size of maze)          (start coordinate pair)
 *        Ex: 20 20 1 1 -> row size | column size | y-coordinate | x-coordinate
 *
 *        y-coordinate = row (first number)
 *        x-coordinate = column (second number)
**/
class Maze {
    // 2D array Maze containing Maze elements
    MazeElement** mazeArray{ nullptr };
    //         grid size (y, x) start coordinate (y,x) end/exit coordinate(y,x)
    long long int row{}, col{}, startY{}, startX{}, endY{}, endX{};
    bool error = {}; // error for file reading

    // Private method: clear 2D maze array
    void clear2DMaze();

    // Private method: maze solver facade
    void solveMaze();

    // Private method: solves the maze using recursion
    bool solveMazeHelper(unsigned int row, unsigned int col,
        unsigned int steps);

    // Private Method: once it reaches the exit, it backtracks to the element
    // with the least amounts of steps and it assigns it with a dot to mark the
    // shortest path
    void backtrackHelper(unsigned int row, unsigned int col);

    // Private method: checks if the coordinate is in bounds of the 2D maze
    // array
    bool isInBounds(const unsigned int& y, const unsigned int& x);

public:
    // Default-Arg Constructor: reads the maze file to fill the 2D array
    // maze
    Maze();

    // Destructor: deletes the 2D array when it is out of scope
    ~Maze();

    // Method: reads the maze file to fill 2D array maze
    void readMazeFile(const std::string& fileName = "maze");

    // Method: prints the characters (walls, path, spaces,
    // start & end coordinates) of the maze
    void printMaze();

    /* getters and setter(s) */

    // get start y-coordinate
    unsigned int getStartY();

    // get start x-coordinate
    unsigned int getStartX();

    // get row size
    unsigned int getRowSize();

    // get column size
    unsigned int getColumnSize();

    // get end y-coordinate
    unsigned int getEndY();

    // get end x-coordinate
    unsigned int getEndX();

    // change start coordinate
    void setStartCoordinate(const unsigned int& y, const unsigned int& x);

    // change end/exit coordinate
    void setEndCoordinate(const unsigned int& y, const unsigned int& x);
};

#endif