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

	Puzzle8StateManager sm;	// Initialize the state manager.
	Puzzle8PQ openList;	// Initialize the priority queue.
	vector<Puzzle8State> states; //List of all states created so far

	//start with inputted puzzle
	Puzzle8State input(puzzle);	// Create a state from its string representation.
	states.push_back(input);

	//check if input is goal state
	if (input.ManhattanDistance() == 8)
	{
		//of goal state then generate and return
		sm.GenerateState(input);
		return;
	}
	//if not goal state then generate and continue 
	else {
		sm.GenerateState(input);
	}


	//create bool to tell if state has been explored
	//puzzle8statemanager is combination of closed and open - it is all total expansions

	//Create expansions of input
	vector<Puzzle8State> inputExpansion = input.GetExpansions();
	expansions++;
	//mark input as an expanded state
	states.at(sm.GetStateId(input)).setExpanded();

	//loop through input's expansions, and if it isn't in the closed list place it in the open list
	for (int i = 0; i < inputExpansion.size(); ++i)
	{
		Puzzle8State tempState = inputExpansion.at(i);

		//if state is generated check if its been expanded yet, if not add a clone to open list
		if (sm.IsGenerated(tempState))
		{
			//if the state hasn't been expanded yet then add a clone to the open list
			if (!states.at(sm.GetStateId(tempState)).isExpanded() == false)
			{
				//Add to open list
				double f = tempState.getCostToState() + 1 + w * tempState.ManhattanDistance();
				openList.push(PQElement(sm.GetStateId(tempState), f));
			}
		}
		//If the state hasn't been generated yet then generate it and add to open list
		else {
			states.push_back(tempState);
			sm.GenerateState(tempState);

			//add to open list
			double f = (double)tempState.getCostToState() + w * tempState.ManhattanDistance();
			openList.push(PQElement(sm.GetStateId(tempState), f));
		}
	}

	
	//Loop until open list is empty or solution state is found
	while(openList.empty() == false) {

		//expand the next state on the open list
		PQElement next = openList.top();

		Puzzle8State tempState = states.at(next.id);

		//If the next element is the solution return - You are done!
		if (tempState.ManhattanDistance() == 0)
		{
			//SOLUTION FOUND
			
			//set the cost to the cost to get to goal node
			cost = tempState.getCostToState();
			return;
		}

		//if the state hasn't been expanded yet, expand it now
		if (states.at(sm.GetStateId(tempState)).isExpanded() == false)
		{
			//create expansions of the state
			vector<Puzzle8State> stateExpansions = tempState.GetExpansions();
			expansions++;
			//mark state as expanded
			states.at(sm.GetStateId(tempState)).setExpanded();

			//loop through input's expansions, and if it isn't in the closed list place it in the open list
			for (int i = 0; i < stateExpansions.size(); ++i)
			{
				Puzzle8State tempState = stateExpansions.at(i);

				//if state is generated check if its been expanded yet, if not add a clone to open list
				if (sm.IsGenerated(tempState))
				{
					//if the state hasn't been expanded yet then add a clone to the open list
					if (states.at(sm.GetStateId(tempState)).isExpanded() == false)
					{
						//Add to open list
						double f = tempState.getCostToState() + 1 + w * tempState.ManhattanDistance();
						openList.push(PQElement(sm.GetStateId(tempState), f));
					}
				}
				//If the state hasn't been generated yet then generate it and add to open list
				else {
					states.push_back(tempState);
					sm.GenerateState(tempState);

					//add to open list
					double f = tempState.getCostToState() + 1 + w * tempState.ManhattanDistance();
					openList.push(PQElement(sm.GetStateId(tempState), f));
				}
			}					
		}

		//Remove the top element from the priority queue.
		openList.pop();
	}
}


