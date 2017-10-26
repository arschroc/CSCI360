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

//Parses the dataset
vector<string> parseDataset(string dataFile) {
	string line;
	ifstream file(dataFile); 
	vector <string> lines;

	//save the first line
	string firstLine = "";
	getline(file, firstLine);

    while (getline(file, line))
    {
        lines.push_back(line);
    }

    file.close();

    //randomizes the lines parsed
    srand(time(0));
    random_shuffle ( lines.begin(), lines.end() );
    lines.insert(lines.begin(), firstLine);

    //inserts the features at the end of the vector
    //lines.insert(lines.begin(), firstLine);
    //lines.push_back(firstLine);

    return lines;
}

vector<string> parseByComma(string input) {
	istringstream ss(input);
	string word;
	vector <string> words;

	while(getline(ss, word, ',')) {
    	words.push_back(word);
	}

	return words;
}

double OutputPart2(int depth, vs datasetPart2, vvs attributesInfo) {
		if (depth > 9)
		{
			cout << " " << depth << "\t";
		}
		else 
		{
			cout << "  " << depth << "\t";
		}
		
		vvs trainingDataTablePart2;
		vvs validationDataTablePart2;
		vvs testingDataTablePart2;

		//training set size = 80% creation
		int trainingSizePart2 = datasetPart2.size() * 6 / 10;
		int validationSizePart2 = datasetPart2.size() * 8 / 10;

		for (int i = 0; i < trainingSizePart2; ++i)
		{
			parseStringIntoTable(datasetPart2.at(i), trainingDataTablePart2);
		}

		//Testing set creating
		for (int i = trainingSizePart2; i < validationSizePart2; ++i)
		{
			parseStringIntoTable(datasetPart2.at(i), validationDataTablePart2);
		}

		for (int i = validationSizePart2; i < datasetPart2.size(); ++i)
		{
			parseStringIntoTable(datasetPart2.at(i), testingDataTablePart2);
		}

		TreeNode* root2 = new TreeNode;
		root2->depth = 0;
		root2 = decisionTreeLearningDepth(trainingDataTablePart2, attributesInfo, root2, depth);
		
		//Output
		//Test agains the decision tree we just made
		vs predictedClassLabelsPart2;
		vs actualClassLabelsPart2;

		//Output against train set
		for (int i = 1; i < trainingDataTablePart2.size(); i++)	
		{
			string data = trainingDataTablePart2[i][0];
			actualClassLabelsPart2.push_back(data);
		}
		
		for (int i = 1; i < trainingDataTablePart2.size(); i++)	
		{
			string test = "";
			if (i == trainingDataTablePart2.size() - 1)
			{
				test = "0";
			}
			else {
				test = testData(trainingDataTablePart2[i], root2, attributesInfo);
			}

			predictedClassLabelsPart2.push_back(test);
		}

		double accuracy2Train = part1PercentAccuracy(actualClassLabelsPart2, predictedClassLabelsPart2);
		//cout << "Part 2 train" << "%" << " for depth " << depth << " is ";
		cout << setprecision(4);
		cout << accuracy2Train << "\t";
		

		//Output against validation set
		predictedClassLabelsPart2.clear();
		actualClassLabelsPart2.clear();

		for (int i = 1; i < validationDataTablePart2.size(); i++)	
		{
			string data = validationDataTablePart2[i][0];
			actualClassLabelsPart2.push_back(data);
		}
		
		for (int i = 1; i < validationDataTablePart2.size(); i++)	
		{
			string test = "";
			if (i == validationDataTablePart2.size() - 1)
			{
				test = "0";

			}
			else {
				test = testData(validationDataTablePart2[i], root2, attributesInfo);
			}

			predictedClassLabelsPart2.push_back(test);
		}

		double accuracy2Valid = part1PercentAccuracy(actualClassLabelsPart2, predictedClassLabelsPart2);
		//cout << "Part 2 valid" << "%" << " for depth " << depth << " is ";
		cout << setprecision(4);
		cout << accuracy2Valid << endl;
		
		return accuracy2Valid;
}

double FinalOutputPart2(int depth, vs datasetPart2, vvs attributesInfo) {
	vvs trainingDataTablePart2;
	vvs testingDataTablePart2;

	//training set size = 80% creation
	int trainingSizePart2 = datasetPart2.size() * 8 / 10;

	for (int i = 0; i < trainingSizePart2; ++i)
	{
		parseStringIntoTable(datasetPart2.at(i), trainingDataTablePart2);
	}

	for (int i = trainingSizePart2; i < datasetPart2.size(); ++i)
	{
		parseStringIntoTable(datasetPart2.at(i), testingDataTablePart2);
	}

	TreeNode* root2 = new TreeNode;
	root2->depth = 0;
	root2 = decisionTreeLearningDepth(trainingDataTablePart2, attributesInfo, root2, depth);

	//Test agains the decision tree we just made
	vs predictedClassLabelsPart2;
	vs actualClassLabelsPart2;

	//Output against test set
	for (int i = 1; i < testingDataTablePart2.size(); i++)	
		{
			string data = testingDataTablePart2[i][0];
			actualClassLabelsPart2.push_back(data);
		}
		
		for (int i = 1; i < testingDataTablePart2.size(); i++)	
		{
		string test = "";
		if (i == testingDataTablePart2.size() - 1)
		{
			test = "0";
		}
		else {
			test = testData(testingDataTablePart2[i], root2, attributesInfo);
		}

		predictedClassLabelsPart2.push_back(test);
	}

	double accuracy2Test = part1PercentAccuracy(actualClassLabelsPart2, predictedClassLabelsPart2);
	return accuracy2Test;	
}


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
		

		//PART 2

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

		double part2Accuracy = FinalOutputPart2(bestDepth, datasetPart2, attributesInfo);

		cout << "Part 2 had a maximum accuracy at depth of " << bestDepth << endl;
		cout << "Part 2 maximum accuracy is " << part2Accuracy << endl;

	}
	else {
		cout << "Program needs 1 argument." << endl;
	}

	return 0;
}


