#include <fstream>
#include <sstream>
#include <string>
#include <iostream>     // std::cout
#include <algorithm>    // std::random_shuffle
#include <vector>       // std::vector
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand

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

    //randomizes the lines parsed
    random_shuffle ( lines.begin(), lines.end() );

    //inserts the features at the end of the vector
    //lines.insert(lines.begin(), firstLine);
    lines.push_back(firstLine);

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

		//Parse the dataset
		string dataFile = argv[1];

		//dataSet is a vector of every line from the inputted text file
		vector<string> dataset = parseDataset(dataFile);

		//create a vector of the features
		string featureString = dataset.back();
		dataset.pop_back();
		vector<string> features = parseByComma(featureString);

	}
	else {
		cout << "Program needs 1 argument." << endl;
	}

	return 0;
}
