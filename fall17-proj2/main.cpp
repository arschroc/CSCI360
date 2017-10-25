#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>    
#include <vector>      
#include <ctime>       
#include <cstdlib>      

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


int main(int argc, char *argv[]) {

	// Needs 1 argument.
	if (argc == 2) {

		//TESTING

		//FIRST ATTEMPT
		/*
		//Parse the dataset
		string dataFile = argv[1];

		//dataSet is a vector of every line from the inputted text file
		vector<string> dataset = parseDataset(dataFile);

		//create a vector of the features
		string featureString = dataset.back();
		dataset.pop_back();
		vector<string> features = parseByComma(featureString);
		*/

		//SECOND ATTEMPT
		//Parse the dataset
		string dataFile = argv[1];

		//dataSet is a vector of every line from the inputted text file
		vs dataset = parseDataset(dataFile);
		vs dataset2 = dataset;
		vvs totalDataTable;
		vvs trainingDataTable;
		vvs testingDataTable;

		//create total data table
		for (int i = 0; i < dataset2.size() - 1; ++i)
		{
			parseStringIntoTable(dataset2.at(i), totalDataTable);
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
		


		//PRINT OUTPUT
		double accuracy = part1PercentAccuracy(actualClassLabels, predictedClassLabels);
		cout << "Part 1 accuracy is ";
		cout << accuracy;
		cout << "%" << endl;
		

	}
	else {
		cout << "Program needs 1 argument." << endl;
	}

	return 0;
}
