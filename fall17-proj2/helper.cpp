#include "helper.h"
#include <algorithm>

void parseString(string& someString, vvs& attributeTable) {
	int attributeCount = 0;
	vs vectorOfStrings;
	while (someString.length() != 0 && someString.find(',') != string::npos)
	{
		size_t pos;
		string singleAttribute;
		pos = someString.find_first_of(',');
		singleAttribute = someString.substr(0, pos);
		vectorOfStrings.push_back(singleAttribute);
		someString.erase(0, pos+1);
	}
	vectorOfStrings.push_back(someString);
	attributeTable.push_back(vectorOfStrings);
	vectorOfStrings.clear();
}

void printVVS(vvs& attributeTable) {
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
			vvs updatedExamples = pruneTable(examples, split, attributes[index][i]); 
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
				vvs table = pruneTable(examples, columnName, examples[i][column]);

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

vvs pruneTable(vvs &attributeTable, string &colName, string value)
{
	vvs prunedTable;
	int column = -1;
	vs headerRow;
	for (int i = 0; i < attributeTable[0].size(); i++) {
		if (attributeTable[0][i] == colName) {
			column = i;
			break;
		}
	}
	for (int i = 0; i < attributeTable[0].size(); i++) {
		 if (i != column) {
		 	headerRow.push_back(attributeTable[0][i]);
		 }
	}
	prunedTable.push_back(headerRow);
	for (int i = 0; i < attributeTable.size(); i++) {
		vs auxRow;
		if (attributeTable[i][column] == value) {
			for (int j = 0; j < attributeTable[i].size(); j++) {
				if(j != column) {
					auxRow.push_back(attributeTable[i][j]);
				}
			}
			prunedTable.push_back(auxRow);
		}
	}
	return prunedTable;
}

string testData(vs &singleLine, TreeNode* nodePtr, vvs &attributes, string defaultClass)
{
	string prediction;
	while (!nodePtr->isLeafNode && !nodePtr->children.empty()) {
		int index = getAttributeIndex(nodePtr->splitOn, attributes);
		string value = singleLine[index];
		int childIndex = returnIndexOfVector(nodePtr->childrenValues, value);
		nodePtr = nodePtr->children[childIndex];
		if (nodePtr == NULL) {
			prediction = defaultClass;
			break;
		}
		prediction = nodePtr->value;
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

int returnIndexOfVector(vs &stringVector, string value)
{
	for (int i = 0; i < stringVector.size(); i++) {
		if (stringVector[i] == value)	{
			return i;
		}
	}
	return -1;
}

double printPredictionsAndCalculateAccuracy(vs &givenData, vs &predictions)
{
	int correct = 0;
	for (int i = 0; i < givenData.size(); i++) {
		if (givenData[i] == predictions[i]) {
			correct++;
		} 
	}
	return (double) correct/givenData.size() * 100;
}