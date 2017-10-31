#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>    
#include <vector>      
#include <ctime>       
#include <cstdlib>    
#include <iomanip>  

#include "helper.h"

using namespace std;



int main(int argc, char *argv[]) {

	// Needs 1 argument.
	if (argc == 2) {

		//Parse the dataset
		string dataFile = argv[1];

		//dataSet is a vector of every line from the inputted text file
		vs dataset = parseDataset(dataFile);
		vs datasetForAttributes = dataset;
		vs datasetPart2 = dataset;
		vvs totalDataTable;
		vvs trainingDataTable;
		vvs testingDataTable;

		//create total data table
		for (int i = 0; i < datasetForAttributes.size(); ++i)
		{
			parseStringIntoTable(datasetForAttributes.at(i), totalDataTable);
		}

		
		//training set size = 80% creation
		int trainingSize = dataset.size() * 8 / 10;
		for (int i = 0; i < trainingSize; ++i)
		{
			parseStringIntoTable(dataset.at(i), trainingDataTable);
		}

		//Testing set creating
		for (int i = trainingSize; i < dataset.size(); ++i)
		{
			parseStringIntoTable(dataset.at(i), testingDataTable);
		}
		

		//Stores list of attributes and their possible values
		vvs attributesInfo = generateAttributesVector(totalDataTable);
		
		//Build the decision tree
		TreeNode* root = new TreeNode;
		root = decisionTreeLearning(trainingDataTable, attributesInfo, root); //build the decision tree

		//Test agains the decision tree we just made
		vs predictedClassLabels;
		vs actualClassLabels;

		for (int i = 1; i < testingDataTable.size(); i++)	
		{
			string data = testingDataTable[i][0];
			actualClassLabels.push_back(data);
		}
		
		for (int i = 1; i < testingDataTable.size(); i++)	
		{
			//TODO implement testDataOnDecisionTree
			string test = "";
			if (i == testingDataTable.size() - 1)
			{
				test = "0";

			}
			else {
				test = testData(testingDataTable[i], root, attributesInfo);
			}

			predictedClassLabels.push_back(test);
		}
		


		//PRINT OUTPUT FOR PART 1
		double accuracy = part1PercentAccuracy(actualClassLabels, predictedClassLabels);
		cout << "Part 1 accuracy is ";
		cout << accuracy;
		cout << "%" << endl;
		

		//PART 2 Table output

		cout << "\n";
		cout << "Part 2:" << endl;
		cout << "depth\ttrain%\tvalid%" << endl;
		
		double max = 0;
		int bestDepth = 0;
		for (int i = 1; i < 16; ++i)
		{
			double accuracy = OutputPart2(i, datasetPart2, attributesInfo);
			if (accuracy > max)
			{
				max = accuracy;
				bestDepth = i;
			}
		}

		//PART 2 Final output

		double part2Accuracy = FinalOutputPart2(bestDepth, datasetPart2, attributesInfo);

		cout << "Part 2 had a maximum accuracy at depth of " << bestDepth << endl;
		cout << "Part 2 maximum accuracy is " << part2Accuracy << endl;

	}
	else {
		cout << "Program needs 1 argument." << endl;
	}

	return 0;
}


