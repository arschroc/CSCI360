#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <math.h>
#include <float.h>
#include <cstdlib>
#include <iomanip>
#include "TreeNode.h"

using namespace std;


/*
 *
 * Typedefs to be used in both helper.cpp and main.cpp and TreeNode.h
 *
 */
typedef vector<int> vi;
typedef vector<double> vd;
typedef vector<string> vs;
typedef vector<vs> vvs;
typedef map<string, int> msi;



/*
 *
 *	Helper Functions
 *
 */

//Parses inputted txt file into a vector of strings for each line
vs parseDataset(string);

//parses a string into an array of strings by splitting origional string by commas
vs parseByComma(string);

//Parses an inputed string and creates a vector of vector of strings
void parseStringIntoTable(string&, vvs&);	

//Generates a vector of vector of strings to hold mushroom attributes and thier values
vvs generateAttributesVector(vvs&); 

//Build the decision tree given examples, attributes, and parent_examples
TreeNode* decisionTreeLearning(vvs&, vvs&, TreeNode*);

//Build the decision tree given examples, attributes, and parent_examples and also has a max depth
TreeNode* decisionTreeLearningDepth(vvs&, vvs&, TreeNode*, int);

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

//Updates examples removing the trait that we split on
vvs updateExamples(vvs&, string&, string);

//runs a single instance of the test data through the decision tree. Returns the predicted class label
string testData(vs&, TreeNode*, vvs&); 

//returns the index of a string in a vector of strings
int getVectorIndex(vs&, string); 

//Prints percent accuracy for part1
double part1PercentAccuracy(vs&, vs&);

//Returns the most common value of poisonous and non poisonous
string mostCommonValue(vvs);

//Output for table of part2
double OutputPart2(int, vs, vvs);

//Final output for part2
double FinalOutputPart2(int, vs, vvs);