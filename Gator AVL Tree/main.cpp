#include "AVL.h"

// https://cplusplus.com/reference/string/string/
int main() {

	AVL* avlTree = new AVL();
	string line;
	getline(cin, line); // gets first line which states how many commands will be run
	int count = stoi(line);

	for (int i = 0; i < count; i++) {
		getline(cin, line);
		int whitespaceCounter = std::count(line.begin(), line.end(), ' '); // checks white spaces to see if there is going to be only first name or first & last name
		istringstream in(line);
		string command;
		in >> command;

		if (command == "insert") {
			bool fullname = false;
			bool successful = true;
			string name;
			string firstname;
			string lastname;
			string ufID;
			getline(in, firstname, '"');
			in >> firstname;
			if (whitespaceCounter > 2) {
				getline(in, lastname, '"');
				name = firstname + lastname;
			}
			if (whitespaceCounter < 3) {
				name = firstname;
				name.erase(remove(name.begin(), name.end(), '"')); // removes extra quotation marks  https://cplusplus.com/reference/algorithm/remove/, https://cplusplus.com/reference/string/string/
			}
			for (char c : name) {
				if (!isalpha(c) && c != ' ') {
					cout << "unsuccessful" << endl;
					successful = false;
					break;
				}
			}
			if (successful) {
				getline(in, ufID, ' ');
				in >> ufID;
				for (char c : ufID) {
					if (!isdigit(c) || ufID.length() != 8) {
						cout << "unsuccessful" << endl;
						successful = false;
						break;
					}
				}
				if (successful) {
					avlTree->insertHelper(stoi(ufID), name);
				}
			}
		}

		else if (command == "remove") {
			bool successful = true;
			string ufID;
			in >> ufID;
			for (char c : ufID) {
				if (!isdigit(c) && ufID.length() != 8) {
					cout << "unsuccessful" << endl;
					successful = false;
					break;
				}
			}
			if (successful) {
				avlTree->removeIDhelper(stoi(ufID));
				cout << "successful" << endl;
			}
		}

		else if (command == "search") {
			bool check;
			string input;
			string input1;
			string input2;
			getline(in, input, ' ');
			in >> input1;
			if (whitespaceCounter > 1) {
				in >> input2;
				input = input1 + ' ' + input2;
			}
			else {
				input = input1;
			}
			if (!isdigit(input.at(2))) {
				getline(in, input, '"');
				input.erase(remove(input.begin(), input.end(), '"'));
				input.erase(remove(input.begin(), input.end(), '"'));
			}
			for (char c : input) {
				if (isdigit(c) && input.length() == 8) {
					check = true;
				}
				else if (isalpha(c) && c != ' ') {
					check = false;
				}

			}
			if (check == true) {
				avlTree->searchIDhelper(stoi(input));
			}
			if (check == false) {
				avlTree->searchNameHelper(input);
			}
		}

		else if (command == "removeInorder") {
			string nth;
			int n;
			bool check = true;
			getline(in, nth, ' ');
			in >> nth;
			n = stoi(nth);
			if (n < 0) {
				cout << "unsuccessful";
				check = false;
			}
			if (check) {
				avlTree->removeInorderHelper(stoi(nth));
			}
		}

		else if (command == "printLevelCount") {
			avlTree->printLevelCountHelper();
		}
		else if (command == "printInorder") {
			avlTree->printInorderHelper();
			cout << endl;
		}
		else if (command == "printPreorder") {
			avlTree->printPreorderHelper();
			cout << endl;
		}
		else if (command == "printPostorder") {
			avlTree->printPostorderHelper();
			cout << endl;
		}

		else {
			cout << "unsuccessful" << endl;
		}
	}
	delete avlTree;
	return 0;
}
