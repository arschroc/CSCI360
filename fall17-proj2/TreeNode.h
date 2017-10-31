#include <cstdlib>
using namespace std;

/*
 *
 * TreeNode Struct for creating the DecisionTree
 *
 */
struct TreeNode
{
	//Keep track of children
	vector<string> childrenValues;
	vector<TreeNode*> children;

	//keeps track of the value of the node and attribute it is split on
	string split;
	string value;

	//keeps track of whether its a leaf node and its current depth
	bool isLeafNode;
	int depth;
};