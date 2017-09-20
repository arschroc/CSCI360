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
		cout << "Solution " << input.GetLinearizedForm() << " Reached" << endl;
		return;
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
			cout << tempState.GetLinearizedForm() << " has ManhattanDistance of " << tempState.ManhattanDistance() << endl; 
			double f = (double)tempState.getCostToState() + w * tempState.ManhattanDistance();
			openList.push(PQElement(sm.GetStateId(tempState), f));
		}
	}

	//TESTING
	/*
	cout << "Size of open list " << openList.size() << endl;
	while (!openList.empty()) {	// While the priority queue is not empty
		PQElement next = openList.top(); // The element with the minimum f-val.
		cout<<"Next state to expand is "<<next.id<<" with f-value "<<next.f << " and form " << states.at(next.id).GetLinearizedForm() <<endl;
		openList.pop();	// Remove the top element from the priority queue.
	}
	*/
	
	//TODO
	//Loop until open list is empty or solution state is found
	//bool test = true;
	while(openList.empty() == false) {
		//test = false;

		//expand the next state on the open list
		PQElement next = openList.top();

		cout<<"Next state to expand is "<<next.id<<" with f-value "<<next.f << " and form " << states.at(next.id).GetLinearizedForm() <<endl;
		Puzzle8State tempState = states.at(next.id);
		cost = tempState.getCostToState();

		//If the next element is the solution return - You are done!
		if (tempState.ManhattanDistance() == 0)
		{
			cout << "Solution " << tempState.GetLinearizedForm() << " Reached" << endl;
			return;
		}

		//if the state hasn't been expanded yet, expand it now
		if (states.at(sm.GetStateId(tempState)).isExpanded() == false)
		{
			//create expansions of the state
			vector<Puzzle8State> stateExpansions = tempState.GetExpansions();
			cout << "Number of expansiosn from " << tempState.GetLinearizedForm() << " is " << stateExpansions.size() << endl;
			expansions++;
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
					else {
						cout << "Duplicate " << tempState.GetLinearizedForm() << endl;
					}
				}
				//If the state hasn't been generated yet then generate it and add to open list
				else {
					states.push_back(tempState);
					sm.GenerateState(tempState);

					//add to open list
					cout << tempState.GetLinearizedForm() << " has ManhattanDistance of " << tempState.ManhattanDistance() << endl; 
					double f = tempState.getCostToState() + 1 + w * tempState.ManhattanDistance();
					openList.push(PQElement(sm.GetStateId(tempState), f));
				}
			}					
		}

		openList.pop();
	}


	/*
	0, 1, 2
	3, 4, 5
	6, 7, 8

	8, 7, 6		  1   2   3   4   5   6   7   8
	5, 4, 0       2 + 4 + 3 + 0 + 2 + 4 + 2 + 4 = 21 
	2, 1, 3

	8, 7, 6
	5, 4, 3       3 + 4 + 2 + 0 + 2 + 4 + 2 + 4 = 21
	2, 0, 1

	*/	
}


