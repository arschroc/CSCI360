#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <math.h>
#include <float.h>
#include <cstdlib>
#include <iomanip>

using namespace std;


/*
 *
 * Typedefs to be used in both helper.cpp and main.cpp
 *
 */
typedef vector<string> vs;
typedef vector<vs> vvs;
typedef vector<int> vi;
typedef map<string, int> msi;
typedef vector<double> vd;


/*
 *
 * TreeNode Struct for creating the DecisionTree
 *
 */
struct TreeNode
{
	string splitOn;
	string value;
	bool isLeafNode;
	vector<string> childrenValues;
	vector<TreeNode*> children;
};


/*
 *
 *	Helper Functions
 *
 */

//Parses an inputed string and creates a vector of vector of strings
void parseString(string&, vvs&);	

//Prints a vector of vector of strings					
void printVVS(vvs&); 

//Generates a vector of vector of strings to hold mushroom attributes and thier values
vvs generateAttributesVector(vvs&); 

//Build the decision tree given examples, attributes, and parent_examples
TreeNode* decisionTreeLearning(vvs&, vvs&, TreeNode*);

//returns true if vector of vector of strings is empty
bool vvsIsEmpty(vvs&); 

//checks if all of the remaining examples have the same classification (0 or 1)
bool sameClassification(vvs&); 

//Returns attribute name of where to split
string whereToSplit(vvs&);

//returns vi with the frequence in which each value appears for an attribute
//used to calculate entropy
vi valueFrequency(vvs&, int);

//returns the index of an attribute
int getAttributeIndex(string&, vvs&); 


vvs pruneTable(vvs&, string&, string); //prunes a table based on a column/attribute's name and the value of that attribute. Removes that column and all instances that have that value for that column
string testData(vs&, TreeNode*, vvs&, string); //runs a single instance of the test data through the decision tree. Returns the predicted class label
int returnIndexOfVector(vs&, string); //returns the index of a string in a vector of strings
double printPredictionsAndCalculateAccuracy(vs&, vs&);