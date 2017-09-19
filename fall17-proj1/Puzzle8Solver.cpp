#include "Puzzle8Solver.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <iomanip>

#include "Puzzle8State.h"
#include "Puzzle8StateManager.h"
#include "Puzzle8PQ.h"

using namespace std;

void WeightedAStar(std::string puzzle, double w, int & cost, int & expansions) {
	cost = 0;
	expansions = 0;

	// TODO
	Puzzle8StateManager sm;	// Initialize the state manager.
	Puzzle8PQ openList;	// Initialize the priority queue.

	//start with inputted puzzle
	Puzzle8State input(puzzle);	// Create a state from its string representation.

	if (sm.IsGenerated(input)) { // You can check if a state has already been generated before ...
		// ... and get its ids by using the function GetStateId.
		cout<<"State "<<input.GetLinearizedForm()<<" has already been generated with id "<<sm.GetStateId(input)<<endl;
	}
	else if (input.ManhattanDistance() == 0) {
		cout<<"State "<<input.GetLinearizedForm()<<" is generated with id "<<sm.GenerateState(input)<<endl;
		cout << "Solution Reached" << endl;
	}
	else {

		// GenerateState only generates a state if it has not been generated before.
		// It returns the id of the state (if it already exists, returns previous id, otherwise, generates a new id).
		cout<<"State "<<input.GetLinearizedForm()<<" is generated with id "<<sm.GenerateState(input)<<endl;
	}

	//TODO
	//create expansions of input


	//Loop until open list is empty or solution state is found


		
}

