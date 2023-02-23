#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <cctype>
using namespace std;

class AVL {

private:
	struct TreeNode {
		int ufID;
		int height;
		int leftHeight;
		int rightHeight;
		TreeNode* left;
		TreeNode* right;
		string name;
		TreeNode() {};
		TreeNode(int _ufID, string _name) : ufID(_ufID), name(_name), left(nullptr), right(nullptr), height(1) {}; // when a tree node is inserted set these to default values
		~TreeNode() {};
	};

	// private methods that cannot be accessed directly from main
	TreeNode* leftRotate(TreeNode* node);
	TreeNode* rightRotate(TreeNode* node);
	void heightCalculator(TreeNode* node);
	int getBalanceFactor(TreeNode* node);
	void searchName(TreeNode* node, string name, bool& check);
	void searchID(TreeNode* node, int ufID, bool& check);
	void printInorder(TreeNode* root, vector<string>& names);
	void AddZeroes(TreeNode* root);
	void printPreorder(TreeNode* root, vector<string>& names);
	void printPostorder(TreeNode* root, vector<string>& names);
	void removeInorder(TreeNode* root, int nth, bool &check);
	int printLevelCount(TreeNode* root);
	TreeNode* insert(TreeNode* root, int ufID, string name);
	TreeNode* removeID(TreeNode* node, int ufID);
	TreeNode* findInorderSuccessor(TreeNode* node);
	// stores current root of AVL tree
	TreeNode* root;


public:

	// helper functions to allow main to access private functions
	void searchNameHelper(string name);
	void printLevelCountHelper();
	void searchIDhelper(int ufID);
	void insertHelper(int ufID, string name);
	void printInorderHelper();
	void printPreorderHelper();
	void printPostorderHelper();
	void removeInorderHelper(int nth);
	int countInorder = 0;
	int calculateHeightHelper(AVL::TreeNode* node);
	void removeIDhelper(int ufID);
	AVL() : root(nullptr) {};
	~AVL() { delete root; }
};



void AVL::heightCalculator(TreeNode* root) {

	// update height for no children
	if (root->right == nullptr && root->left == nullptr) {
		root->height = 1;
	}

	// update height for only right child
	else if (root->left == nullptr && root->right != nullptr) {
		root->height = 1 + root->right->height;
	}

	// update height for only left child
	else if (root->right == nullptr && root->left != nullptr) {
		root->height = 1 + root->left->height;
	}

	// update height for 2 children
	else if (root->left != nullptr && root->right != nullptr) {
		root->height = 1 + max(root->left->height, root->right->height);
	}

}




int AVL::getBalanceFactor(TreeNode* root) {

	// if there is only a root, or no root, bf = 0
	if (root->right == nullptr && root->left == nullptr || root == nullptr) {
		return 0;
	}

	// if there is only values on right side of tree, bf = -(height of right subtree)
	else if (root->left == nullptr && root->right != nullptr) {
		return root->right->height * -1;
	}

	// if there is only values on left side of tree, bf = (height of left subtree)
	else if (root->right == nullptr && root->left != nullptr) {
		return root->left->height;
	}

	// if there is values on both sides of the tree, bf = (height of left subtree) - (height of right subtree)
	else {
		return (root->left->height - root->right->height);
	}
}




// allows main to access function
void AVL::insertHelper(int ufID, string name)
{
	root = insert(root, ufID, name);
}

// used some code from class slides
// inserts a node, updates heights, calculates balance of tree, and returns the root node
AVL::TreeNode* AVL::insert(TreeNode* root, int ufID, string name)
{
	if (root == nullptr) {
		cout << "successful" << endl;
		return new TreeNode(ufID, name);
	}
	else if (ufID < root->ufID) {
		root->left = insert(root->left, ufID, name);
	}
	else if (ufID > root->ufID) {
		root->right = insert(root->right, ufID, name);
	}
	else {
		cout << "unsuccessful" << endl;
		return root;
	}


	heightCalculator(root); // updates height of root
	int balanceFactor = getBalanceFactor(root); // gets balance factor to check if rotations are needed

	if (balanceFactor < -1) {
		if (ufID < root->right->ufID) {
			root->right = rightRotate(root->right);
			root = leftRotate(root); // updates the root of the tree
			heightCalculator(root);	 // calculate height with root
		}
		else {
			root = leftRotate(root);
			heightCalculator(root);
		}
	}
	if (balanceFactor > 1) {
		if (ufID > root->left->ufID) {
			root->left = leftRotate(root->left);
			root = rightRotate(root);
			heightCalculator(root);
		}
		else {
			root = rightRotate(root);
			heightCalculator(root);
		}
	}
	return root;
}


// solved from stepik
// performs left rotation
AVL::TreeNode* AVL::leftRotate(AVL::TreeNode* node) {
	TreeNode* grandchild = node->right->left;
	TreeNode* newParent = node->right;
	newParent->left = node;
	node->right = grandchild;

	// update height of child of newParent
	heightCalculator(node);

	return newParent;
}

// solved from stepik
// performs right rotation
AVL::TreeNode* AVL::rightRotate(AVL::TreeNode* node) {
	TreeNode* grandchild = node->left->right;
	TreeNode* newParent = node->left;
	newParent->right = node;
	node->left = grandchild;

	heightCalculator(node);

	return newParent;
}




void AVL::removeIDhelper(int ufID) {
	removeID(root, ufID);
}

AVL::TreeNode* AVL::removeID(AVL::TreeNode* node, int ufID) {

	// recursion until we find the node that matches the input
	if (ufID < node->ufID) {
		node->left = removeID(node->left, ufID);
	}
	if (ufID > node->ufID) {
		node->right = removeID(node->right, ufID);
	}

	// check how many children the to-be-removed node has and rearrange nodes
	else {

		// no children
		if (node->left == nullptr && node->right == nullptr) {
			node = nullptr;
		}

		// one right child
		else if (node->left == nullptr && node->right != nullptr) {
			TreeNode* temp = node->right;
			node = node->right;
		}

		// one left child
		else if (node->right == nullptr && node->left != nullptr) {
			TreeNode* temp = node->left;
			node = node->left;
		}

		// 2 children
		else if (node->left != nullptr && node->right != nullptr) {
			TreeNode* temp = findInorderSuccessor(node->right); // find inorder successor by passing in right child
			node->ufID = temp->ufID;                            // replace node values with inorder successor values
			node->name = temp->name;
			node->right = removeID(node->right, temp->ufID);    // remove duplicate node
		}
	}

	return node;
}


AVL::TreeNode* AVL::findInorderSuccessor(TreeNode* node)
{
	while (node->left != nullptr) { // loops until lowest value child of passed in node is found
		node = node->left;
	}
	return node;

}






// https://cplusplus.com/reference/string/string/string/
// https://cplusplus.com/reference/string/string/append/
// adds leading zeroes back into ufID
void AVL::AddZeroes(TreeNode* root)
{
	string ufID = string(8 - to_string(root->ufID).size(), '0').append(to_string(root->ufID));
	cout << ufID << endl;
}


void AVL::searchNameHelper(string name)
{
	bool check = false;
	searchName(root, name, check);

	if (check == false) {
		cout << "unsuccessful" << endl;
	}

}


void AVL::searchName(AVL::TreeNode* root, string name, bool& check)
{
	bool alreadyPrinted = false;
	if (root == nullptr) {
		return;
	}
	if (root->name == name) {
		if (to_string(root->ufID).size() != 8) {		// if ufID had leading zeroes that were lost
			alreadyPrinted = true;
			check = true;
			AddZeroes(root);
		}
		if (!alreadyPrinted) {
			cout << root->ufID << endl;
			check = true;
		}
	}
	// recursively check the tree for the name
	if (root->left != nullptr) {
		searchName(root->left, name, check);
	}
	if (root->right != nullptr) {
		searchName(root->right, name, check);
	}
	return;
}





void AVL::searchIDhelper(int ufID)
{
	bool check = false;
	searchID(root, ufID, check);
	if (check == false) {
		cout << "unsuccessful" << endl;
	}
}

void AVL::searchID(AVL::TreeNode* root, int ufID, bool& check) {
	if (root == nullptr) {
		return;
	}
	if (root->ufID == ufID) {
		cout << root->name << endl;
		check = true;
	}
	if (root->ufID > ufID) {
		searchID(root->left, ufID, check);
	}
	if (root->ufID < ufID) {
		searchID(root->right, ufID, check);
	}
	return;
}





void AVL::printInorderHelper() {
	vector<string> names;
	printInorder(root, names);
	for (unsigned int i = 0; i < names.size(); i++) {
		if (i != 0) {
			cout << ", ";
		}
		cout << names[i];
	}
}

// prints left subtree -> root -> right subtree
void AVL::printInorder(AVL::TreeNode* root, vector<string>& names) {
	if (root == nullptr) {
		return;
	}
	if (root->left != nullptr) {
		printInorder(root->left, names);
	}
	names.push_back(root->name);            // names are pushed into vector to make comma formatting easier
	if (root->right != nullptr) {
		printInorder(root->right, names);
	}
}





void AVL::printPreorderHelper() {
	vector<string> names;
	printPreorder(root, names);
	for (unsigned int i = 0; i < names.size(); i++) {
		if (i != 0) {
			cout << ", ";
		}
		cout << names[i];
	}
}

// prints root -> left subtree -> right subtree
void AVL::printPreorder(TreeNode* root, vector<string>& names) {
	if (root == nullptr) {
		return;
	}
	names.push_back(root->name);
	if (root->left != nullptr) {
		printPreorder(root->left, names);
	}
	if (root->right != nullptr) {
		printPreorder(root->right, names);
	}
}





void AVL::printPostorderHelper() {
	vector<string> names;
	printPostorder(root, names);
	for (unsigned int i = 0; i < names.size(); i++) {
		if (i != 0) {
			cout << ", ";
		}
		cout << names[i];
	}

}

// prints left subtree -> right subtree -> root
void AVL::printPostorder(TreeNode* root, vector<string>& names) {
	if (root == nullptr) {
		return;
	}
	if (root->left != nullptr) {
		printPostorder(root->left, names);
	}
	if (root->right != nullptr) {
		printPostorder(root->right, names);
	}
	names.push_back(root->name);
}





void AVL::removeInorderHelper(int nth) {
	bool check = false;
	removeInorder(root, nth + 1, check);
	if (check) {
		cout << "successful" << endl;
	}
	else {
		cout << "unsuccessful" << endl;
	}
	countInorder = 0;
}

// uses same method from printInorder w/ count variable to find node
void AVL::removeInorder(AVL::TreeNode* node, int nth, bool &check) {

	if (countInorder < nth) {
		if (node->left != nullptr) {
			removeInorder(node->left, nth, check);
		}
		countInorder++;

		if (countInorder == nth) {
			check = true;
			removeID(root, node->ufID);
		}
		if (node->right != nullptr) {
			removeInorder(node->right, nth, check);
		}
	}
}





void AVL::printLevelCountHelper() {
	int levelCount = printLevelCount(root);
	cout << levelCount << endl;
}

int AVL::printLevelCount(TreeNode* root) {
	if (root == nullptr) {
		return 0;
	}
	return root->height; // uses the data from heightCalculator function
}