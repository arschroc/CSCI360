#include "helper.h"
#include <algorithm>
#include <sstream>

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

void printVVS(vvs& attributeTable) {
	//loop throught able printing each row at a time
	for (int row = 0; row < attributeTable.size(); row++) {
		for (int column = 0; column < attributeTable[row].size(); column++) {
			cout << attributeTable[row][column] << " ";
		}
		cout << endl;
	}
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
		node->splitOn = split;
		int index = getAttributeIndex(split, attributes);

		//loop through all values of splitting attribute creating new nodes
		for (int i = 1; i < attributes[index].size(); i++) {
			TreeNode* newNode = (TreeNode*) new TreeNode;
			newNode->value = attributes[index][i];
			node->childrenValues.push_back(attributes[index][i]);
			newNode->isLeafNode = false; //not a leaf node if a splitting node
			newNode->splitOn = split;
			//update the examples to not include splitting value
			vvs updatedExamples = updateExamples(examples, split, attributes[index][i]); 
			//recurse to create sub trees and set those as the children
			node->children.push_back(decisionTreeLearning(updatedExamples, attributes, newNode));
		}
	}

	return node;
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
		vi counts = valueFrequency(examples, column);
		vd attributeEntropy;
		string columnName = examples[0][column];

		//Reset entropy to 0
		double columnEntropy = 0.0;

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
					entropy -= (val/frequencies[frequencies.size()-1])*(log(val/frequencies[frequencies.size()-1]) / log(2));
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

		//Calculate total column entropy 
		for (int i = 1; i < counts.size(); i++) {
			columnEntropy += ((double) counts[i] * (double) attributeEntropy[i]);
		}

		columnEntropy = columnEntropy / ((double) counts[counts.size() - 1]);

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
		int index = getAttributeIndex(node->splitOn, attributes);
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