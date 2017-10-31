#include "helper.h"
#include <algorithm>
#include <sstream>

//Parses the dataset from a file
vs parseDataset(string dataFile) {
	string line;
	ifstream file(dataFile); 
	vs lines;

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


    return lines;
}

vs parseByComma(string input) {
	istringstream ss(input);
	string word;
	vs words;

	while(getline(ss, word, ',')) {
    	words.push_back(word);
	}

	return words;
}

void parseStringIntoTable(string& line, vvs& attributeTable) {
	istringstream ss(line);
	string temp;
	vs strings;

	//seperate the line by commas and add each string into strings vector
	while(std::getline(ss, temp, ',')) {
    	strings.push_back(temp);
	}

	//push strings onto vector of vector of strings
	attributeTable.push_back(strings);
}

vvs generateAttributesVector(vvs &dataTable)
{
	//vector that will be returned
	vvs attributesVector;

	//loop through each attribute and create a vector of its possible values
	for (int column = 0; column < dataTable[0].size(); ++column)
	{
		vs attribute;
		attribute.push_back(dataTable[0][column]);

		for (int row = 1; row < dataTable.size(); ++row)
		{
			string potentialValue = dataTable[row][column];
			//if attribute doesn't contain current value then add it
			if (find(attribute.begin(), attribute.end(), potentialValue) == attribute.end())
			{
				attribute.push_back(potentialValue);
			}
		}

		attributesVector.push_back(attribute);
	}

	return attributesVector;
}

TreeNode* decisionTreeLearning(vvs& examples, vvs& attributes, TreeNode* node) {

	//if examples is empty then return plurality-value(parent_examples)
	if (vvsIsEmpty(examples)) {
		return NULL;
	}
	//if all examples have the same classification then return the classification
	else if (sameClassification(examples)) {
		node->isLeafNode = true;
		//Assign the correct class value
		node->value = examples[1][0];
		return node;
	} 
	else {
		//decide what column to split on
		string split = whereToSplit(examples);
		node->split = split;
		int index = getAttributeIndex(split, attributes);

		//loop through all values of splitting attribute creating new nodes
		for (int i = 1; i < attributes[index].size(); i++) {
			TreeNode* newNode = (TreeNode*) new TreeNode;
			newNode->value = attributes[index][i];
			node->childrenValues.push_back(attributes[index][i]);
			newNode->isLeafNode = false; //not a leaf node if a splitting node
			newNode->split = split;
			newNode->depth = node->depth + 1;
			//update the examples to not include splitting value
			vvs updatedExamples = updateExamples(examples, split, attributes[index][i]); 
			//recurse to create sub trees and set those as the children
			node->children.push_back(decisionTreeLearning(updatedExamples, attributes, newNode));
		}
	}

	return node;
}

TreeNode* decisionTreeLearningDepth(vvs& examples, vvs& attributes, TreeNode* node, int maxDepth) {

	//cout << "node depth is " << node->depth << endl;
	//cout << "max depth is " << maxDepth << endl;

	//if examples is empty then return plurality-value(parent_examples)
	if (vvsIsEmpty(examples)) {
		return NULL;
	}
	//if all examples have the same classification then return the classification
	else if (sameClassification(examples)) {
		node->isLeafNode = true;
		//Assign the correct class value
		node->value = examples[1][0];
		return node;
	}
	else if(node->depth == maxDepth) {
		node->isLeafNode = true;
		node->value = mostCommonValue(examples);
		//node->value = to_string(rand()%2);
		return node;
	}
	else {
		//decide what column to split on
		string split = whereToSplit(examples);
		node->split = split;
		int index = getAttributeIndex(split, attributes);

		//cout << "Split on " << split << endl;
		//cout << "Node depth is " << node->depth << endl;

		//loop through all values of splitting attribute creating new nodes
		for (int i = 1; i < attributes[index].size(); i++) {
			TreeNode* newNode = (TreeNode*) new TreeNode;
			newNode->depth = node->depth + 1;
			newNode->value = attributes[index][i];
			node->childrenValues.push_back(attributes[index][i]);
			newNode->isLeafNode = false; //not a leaf node if a splitting node
			newNode->split = split;
			//update the examples to not include splitting value
			vvs updatedExamples = updateExamples(examples, split, attributes[index][i]); 
			//recurse to create sub trees and set those as the children
			node->children.push_back(decisionTreeLearningDepth(updatedExamples, attributes, newNode, maxDepth));
		}
	}

	return node;
}

string mostCommonValue(vvs examples) {
	int zeroCounter = 0;
	int oneCounter = 0;

	for (int i = 1; i < examples.size(); ++i)
	{
		if (examples[i][0] == "0")
		{
			zeroCounter++;
		}
		else {
			oneCounter++;
		}
	}

	if (zeroCounter > oneCounter)
	{
		return "0";
	}
	return "1";
}

bool vvsIsEmpty(vvs &table)
{
	return (table.size() == 1);
}

bool sameClassification(vvs &dataTable)
{
	string firstValue = dataTable[1][0];
	for (int i = 1; i < dataTable.size(); i++) {
		if (firstValue != dataTable[i][0]) {
			return false;
		}
	}
	return true;
}

string whereToSplit(vvs &examples)
{
	//variables
	double minEntropy = DBL_MAX;
	int split = 0;
	vi entropies;

	//loop through each column and decide which to split on
	for (int column = 1; column < examples[0].size(); column++) {
		msi map;
		vi freq = valueFrequency(examples, column);
		vd attributeEntropy;
		string columnName = examples[0][column];

		//Go through each example calculating entropy for each attribute
		for (int i = 1; i < examples.size()-1; i++) {
			double entropy = 0.0;
			

			//If value of attribute hasn't been seen before then create it and update entropy
			if (map.find(examples[i][column]) == map.end()) { 
				map[examples[i][column]] = 1;

				//prune the dataTable of new attribute
				vvs table = updateExamples(examples, columnName, examples[i][column]);

				int size = table[0].size()-1;
				vi frequencies = valueFrequency(table, size);

				//calculate entropy
				for (int j = 0; j < frequencies.size(); j++) {
					double val = (double) frequencies[j];

					//Calculate the entropy for each iteration of a value
					double p = val/frequencies[frequencies.size()-1];
					double b = log(val/frequencies[frequencies.size()-1]) / log(2);
					entropy = entropy - (p*b);
				}

				//add entropy of attribute
				attributeEntropy.push_back(entropy);

				//reset entropy
				entropy = 0.0;
			} 

			// Update sightings of value of attribute in column
			else { 		
				map[examples[i][column]]++;
			}
		}

		//Reset entropy to 0
		double columnEntropy = 0.0;

		//Find column entropy for each attribute
		for (int i = 1; i < freq.size(); i++) {
			columnEntropy = columnEntropy + ((double) freq[i] * (double) attributeEntropy[i]);
		}

		columnEntropy = columnEntropy / ((double) freq[freq.size() - 1]);

		//Calculate if new column entropy is the new minimum entropy
		if (columnEntropy <= minEntropy) {
			minEntropy = columnEntropy;
			split = column;
		}
	}

	return examples[0][split];
}

vi valueFrequency(vvs &table, int column)
{
	bool exists = false;
	int index;
	vs strings;
	vi frequencies;

	for (int i = 1; i < table.size(); i++) {

		//loop through values already discovered
		for (int j = 0; j < strings.size(); j++) {

			//if the value has been seen already mark it as so and continue out of inner loop
			if (strings[j] == table[i][column]) {
				exists = true;
				index = j;
				break;
			} 

			else {
				exists = false;
			}
		}

		//If the value has already been seen then update the frequency
		if (exists) {
			frequencies[index]++;
		} 
		//if value is new then add it and set the frequency to 1
		else {
			frequencies.push_back(1);
			strings.push_back(table[i][column]);
		}
	}

	//Add up all of the frequencies
	int sum = 0;

	for (int i = 0; i < frequencies.size(); i++) {
		sum += frequencies[i];
	}

	frequencies.push_back(sum);
	return frequencies;
}

vvs updateExamples(vvs &attributes, string &columnString, string value)
{
	vvs updatedExamples;
	int column = -1;
	vs attributeRow;

	//Find the column we are removing
	for (int i = 0; i < attributes[0].size(); i++) {
		if (attributes[0][i] == columnString) {
			column = i;
			break;
		}
	}

	//Remove the desired attribute
	for (int i = 0; i < attributes[0].size(); i++) {
		 if (i != column) {
		 	attributeRow.push_back(attributes[0][i]);
		 }
	}

	//push the updated attributes into updatedExamples
	updatedExamples.push_back(attributeRow);

	//Finish created examples list table
	for (int i = 0; i < attributes.size(); i++) {
		vs row; //used to create a row

		if (attributes[i][column] == value) {
			for (int j = 0; j < attributes[i].size(); j++) {

				//Dont add the column that we removed
				if(j != column) {
					row.push_back(attributes[i][j]);
				}
			}

			updatedExamples.push_back(row);
		}
	}
	return updatedExamples;
}

string testData(vs &row, TreeNode* node, vvs &attributes)
{
	string prediction;

	//Traverse through nodes until leaf is reached (no more children left)
	while (!node->isLeafNode && !node->children.empty()) {
		//gets the index of current attribute
		int index = getAttributeIndex(node->split, attributes);
		string value = row[index];

		//get index of next child
		int childIndex = getVectorIndex(node->childrenValues, value);

		//set node to childs
		node = node->children[childIndex];

		//If no prediction can be made, assume poisonous to be safe
		if (node == NULL) {
			prediction = "1"; 
			break;
		}

		//if a prediction exists set that
		prediction = node->value;
	}
	return prediction;
}

int getAttributeIndex(string &attribute, vvs &attributes)
{
	//loop through all of the attributes and find what number column corresponds with columnName
	for (int i = 0; i < attributes.size(); i++) {
		if (attributes[i][0] == attribute) {
			return i;
		}
	}

	//if column index was never found then return -1
	return -1;
}

int getVectorIndex(vs &stringVector, string value)
{
	for (int i = 0; i < stringVector.size(); i++) {
		if (stringVector[i] == value)	{
			return i;
		}
	}

	//if vector doesn't have value then return -1
	return -1;
}

double part1PercentAccuracy(vs &actual, vs &prediction)
{
	int correct = 0;

	for (int i = 0; i < actual.size(); i++) {

		//if actual == prediction increment correct counter
		if (actual[i] == prediction[i]) {
			correct++;
		} 
	}

	//return percent of correct predictions
	return (double) correct/actual.size() * 100;
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