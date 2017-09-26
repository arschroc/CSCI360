#ifndef STATE_8_PUZZLE_H
#define STATE_8_PUZZLE_H

#include <string>
#include <cassert>
#include <iostream>
#include <vector>
#include <cmath>
#include <math.h>
#include <vector>

// Represents an 8-puzzle state as a 3x3 array of chars. Each char can take values in range '0'-'9' (chars, not integers).
// '0' represents the blank tile.
// Provides GetKey to uniquely represent the puzzle as an integer (for hashing).
// You can extend this class to generate successors and get heuristic distance to '012345678', which is always the goal state.

class Puzzle8State {
public:
	Puzzle8State(std::string s = "012345678") {
		assert(s.length() == 9);
		for (int r = 0; r < 3; r++)
			for (int c = 0; c < 3; c++)
				tiles[r][c] = s[r*3 + c];
		expanded = false;
		costToState = 0;
	}

	// Key generated as an integer for the hash function in Puzzle8StateManager.
	int GetKey() {
		int key = 0;
		for (int r = 0; r < 3; r++)
			for (int c = 0; c < 3; c++)
				key = key*10 + int(tiles[r][c] - '0');
		return key;
	}

	bool isExpanded() {
		return expanded;
	}

	void setExpanded() {
		expanded = true;
	}

	// Return the linearized form as a string. (You don't need to use this.)
	std::string GetLinearizedForm () {
		std::string s = "";
		for (int r = 0; r < 3; r++)
			for (int c = 0; c < 3; c++)
				s += tiles[r][c];
		return s;
	}


	// Return the linearized form of expansion as a string
	std::string GetLinearizedFormWithTiles (char** newTiles) {
		std::string s = "";
		for (int r = 0; r < 3; r++)
			for (int c = 0; c < 3; c++)
				s += newTiles[r][c];
		return s;
	}
	
	// Sets the cost to reach the state
	void setCostToState(int cost) {
		costToState = cost;
	}
	
	//returns the cost to reach the state
	int getCostToState() {
		return costToState;
	}

	// Print the puzzle in a 3x3 layout. (You don't need to use this.)
	void Print(std::ostream & out = std::cout) {
		for (int r = 0; r < 3; r++) {
			for (int c = 0; c < 3; c++) {
				out<<tiles[r][c]<<" ";
			}
			out<<std::endl;
		}
		out<<GetKey()<<std::endl;
	}

	//returns total manhattan distance from the goal state
	//goal state:
	/*
	0, 1, 2
	3, 4, 5
	6, 7, 8
	targetX = x % 3
	targetY = y / 3
	*/
	int ManhattanDistance() {

		int manhattanDistanceSum = 0;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				int value = (int)tiles[i][j] - 48;
				if (value != 0)
				{
					int targetX = value % 3;
					int targetY = value / 3;
					int xDif = abs(j - targetX);
					int yDif = abs(i - targetY);
					manhattanDistanceSum += xDif + yDif;
				}
			}
		}

		return manhattanDistanceSum;
	}


	//Creates a shallow copy of the tiles
	char** CreateNewTiles() {
	char** newTiles = 0;
      	newTiles = new char*[3];
		for (int i = 0; i < 3; ++i)
		{
			newTiles[i] = new char[3];
			for (int j = 0; j < 3; ++j)
			{
				newTiles[i][j] = tiles[i][j];
			}
		}

		return newTiles;
	}

	//returns vector of all expansions from current state
	std::vector<Puzzle8State> GetExpansions() {
		std::vector<Puzzle8State> v;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				int value = (int)tiles[i][j] - 48;
				if (value == 0)
				{
					//swap left
					if (i > 0)
					{
						char **newTiles = CreateNewTiles();
						char temp = newTiles[i][j];
						newTiles[i][j] = newTiles[i-1][j];
						newTiles[i-1][j] = temp;
						Puzzle8State state(GetLinearizedFormWithTiles(newTiles));
						int newCost = costToState + 1;
						state.setCostToState(newCost);
						v.push_back(state);
					}

					//swap right
					if (i < 2)
					{	
						char **newTiles = CreateNewTiles();
						char temp = newTiles[i][j];
						newTiles[i][j] = newTiles[i+1][j];
						newTiles[i+1][j] = temp;
						Puzzle8State state(GetLinearizedFormWithTiles(newTiles));
						int newCost = costToState + 1;
						state.setCostToState(newCost);
						v.push_back(state);
					}

					//swap bottom
					if (j > 0)
					{
						char **newTiles = CreateNewTiles();
						char temp = newTiles[i][j];
						newTiles[i][j] = newTiles[i][j-1];
						newTiles[i][j-1] = temp;
						Puzzle8State state(GetLinearizedFormWithTiles(newTiles));
						int newCost = costToState + 1;
						state.setCostToState(newCost);
						v.push_back(state);
					}

					//swap top
					if (j < 2)
					{
						char **newTiles = CreateNewTiles();
						char temp = newTiles[i][j];
						newTiles[i][j] = newTiles[i][j+1];
						newTiles[i][j+1] = temp;
						Puzzle8State state(GetLinearizedFormWithTiles(newTiles));
						int newCost = costToState + 1;
						state.setCostToState(newCost);
						v.push_back(state);
					}					
				}
			}
		}

		return v;
	}


private:

	// tiles[r][c] is the tile (or blank) at row r (0-2) and column c (0-2)
	// 0th row is the top row, and 0th column is the leftmost column.
	char tiles[3][3];
	bool expanded;
	int costToState;

};

#endif
