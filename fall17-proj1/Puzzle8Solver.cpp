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
	vector<Puzzle8State> states; //List of all states created so far

	//start with inputted puzzle
	Puzzle8State input(puzzle);	// Create a state from its string representation.
	states.push_back(input);

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
	//create bool to tell if state has been explored
	//puzzle8statemanager is combination of closed and open - it is all total expansions

	//create expansions of input
	vector<Puzzle8State> inputExpansion = input.GetExpansions();
	expansions++;

	//loop through input's expansions, and if it isn't in the closed list place it in the open list
	for (int i = 0; i < inputExpansion.size(); ++i)
	{
		Puzzle8State tempState = inputExpansion.at(i);

		//if state is created check if its been expanded yet, if not add a clone to open list
		if (sm.IsGenerated(tempState))
		{
			//if the state hasn't been expanded yet then add a clone to the open list
			if (!states.at(sm.GetStateId(tempState)).isExpanded())
			{
				//TODO
				//add to open list
			}
		}
		//If the state hasn't been created yet then create it and add to open list
		else {
			states.push_back(tempState);
			sm.GenerateState(tempState);

			//TODO
			//add to open list
		}
	}

	//TODO
	//Loop until open list is empty or solution state is found	
}


