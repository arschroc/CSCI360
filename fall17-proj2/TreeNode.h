#include <cstdlib>
using namespace std;

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
	int depth;
};