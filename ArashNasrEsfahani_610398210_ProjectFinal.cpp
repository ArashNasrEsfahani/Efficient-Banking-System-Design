#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <iomanip>
#include <fstream>

using namespace std;
const int ALPHABET_SIZE = 27;
enum COLOR { RED, BLACK };

class Node {
public:
int id;
string first;
string last;
int balance;
COLOR color;
Node *left, *right, *parent;

Node(int i, string f, string l, int b) : id(i) {
    first = f;
    last = l;
    balance = b;
	parent = left = right = NULL;
	color = RED;
}

Node *uncle() {

	if (parent == NULL or parent->parent == NULL)
		return NULL;

	if (parent->isOnLeft())
		return parent->parent->right;

	else
		return parent->parent->left;
}

bool isOnLeft() { return this == parent->left; }

Node *sibling() {

	if (parent == NULL)
		return NULL;

	if (isOnLeft())
		return parent->right;

	return parent->left;
}

void moveDown(Node *nParent) {
	if (parent != NULL) {
		if (isOnLeft()) {
		parent->left = nParent;
		} 
		else {
		parent->right = nParent;
		}
	}

	nParent->parent = parent;
	parent = nParent;
}

bool hasRedChild() {
	return (left != NULL and left->color == RED) or
		(right != NULL and right->color == RED);
}

};



class RBTree {
Node *root;

void leftRotate(Node *x) {
	Node *newParent = x->right;

	if (x == root)
		root = newParent;

	x->moveDown(newParent);

	x->right = newParent->left;

	if (newParent->left != NULL)
		newParent->left->parent = x;

	newParent->left = x;
}

void rightRotate(Node *x) {

	Node *newParent = x->left;

	if (x == root)
		root = newParent;

	x->moveDown(newParent);

	x->left = newParent->right;

	if (newParent->right != NULL)
		newParent->right->parent = x;

	newParent->right = x;
}

void swapColors(Node *x1, Node *x2) {
	COLOR temp;
	temp = x1->color;
	x1->color = x2->color;
	x2->color = temp;
}

void swapValues(Node *x, Node *y) {
	int tempId;
    int tempBalance;
    string tempFirst;
    string tempLast;

	tempId = x->id;
    tempBalance = x->balance;
    tempFirst = x->first;
    tempLast = x->last;
	x->id = y->id;
    x->balance = y->balance;
    x->first = y->first;
    x->last = y->last;
	y->id = tempId;
    y->balance = tempBalance;
    y->first = tempFirst;
    y->last = tempLast;
}

void fixDoubleRed(Node *x) {
	if (x == root) {
		x->color = BLACK;
		return;
	}

	Node *parent = x->parent, *grandparent = parent->parent,
		*uncle = x->uncle();

	if (parent->color != BLACK) {
		if (uncle != NULL && uncle->color == RED) {
			parent->color = BLACK;
			uncle->color = BLACK;
			grandparent->color = RED;
			fixDoubleRed(grandparent);
		} 

	else {

			if (parent->isOnLeft()) {
				if (x->isOnLeft()) {
					swapColors(parent, grandparent);
				}
				else {
					leftRotate(parent);
					swapColors(x, grandparent);
				}

				rightRotate(grandparent);
				} 
			else {
				if (x->isOnLeft()) {

					rightRotate(parent);
					swapColors(x, grandparent);
				} 
				else {
					swapColors(parent, grandparent);
				}
				leftRotate(grandparent);
			}
		}
	}
}

Node *successor(Node *x) {
	Node *temp = x;

	while (temp->left != NULL)
		temp = temp->left;

	return temp;
}

Node *treeReplace(Node *x) {

	if (x->left != NULL and x->right != NULL)
		return successor(x->right);

	if (x->left == NULL and x->right == NULL)
		return NULL;

	if (x->left != NULL)
		return x->left;
	else
		return x->right;
}

void deleteNode(Node *x) {
	Node *y = treeReplace(x);

	bool yxBlack = ((y == NULL || y->color == BLACK) && (x->color == BLACK));
	Node *parent = x->parent;

	if (y == NULL) {
		if (x == root) {
			root = NULL;
		}
		else {
			if (yxBlack) {
				fixDoubleBlack(x);
			}
			else {
				if (x->sibling() != NULL)
					x->sibling()->color = RED;
			}

			if (x->isOnLeft()) {
				parent->left = NULL;
			}
			else {
				parent->right = NULL;
			}
		}
		delete x;
		return;
	}

	if (x->left == NULL || x->right == NULL) {
		if (x == root) {
			x->id = y->id;
			x->left = x->right = NULL;
			delete y;
		} 
		else {
			if (x->isOnLeft()) {
				parent->left = y;
			} 
			else {
				parent->right = y;
			}
			delete x;
			y->parent = parent;
			if (yxBlack) {
				fixDoubleBlack(y);
			} 
			else {
				y->color = BLACK;
			}
		}
		return;
	}

	swapValues(y, x);
	deleteNode(y);
}

void fixDoubleBlack(Node *x) {
	if (x == root)
		return;

	Node *sibling = x->sibling(), *parent = x->parent;
	if (sibling == NULL) {
		fixDoubleBlack(parent);
	} 
	else{
		if (sibling->color == RED) {

			parent->color = RED;
			sibling->color = BLACK;
			if (sibling->isOnLeft()) {

			rightRotate(parent);
			} 
			else {
				leftRotate(parent);
			}

			fixDoubleBlack(x);
		}
		else {

			if (sibling->hasRedChild()) {

			if (sibling->left != NULL and sibling->left->color == RED) {
				if (sibling->isOnLeft()) {
					sibling->left->color = sibling->color;
					sibling->color = parent->color;
					rightRotate(parent);

				}
				else {
					sibling->left->color = parent->color;
					rightRotate(sibling);
					leftRotate(parent);
				}
			} 
			else {
				if (sibling->isOnLeft()) {
					sibling->right->color = parent->color;
					leftRotate(sibling);
					rightRotate(parent);
				} else {
					sibling->right->color = sibling->color;
					sibling->color = parent->color;
					leftRotate(parent);
				}
			}
			parent->color = BLACK;
			} 
			else {
				sibling->color = RED;
				if (parent->color == BLACK)
					fixDoubleBlack(parent);
				else
					parent->color = BLACK;
			}
		}
	}
}


void levelOrderMinusSearch(Node *x) {

	bool flag = false;
	if (x == NULL)
	    return;

	queue<Node *> q;
	Node *curr;

	q.push(x);

	while (!q.empty()) {

	curr = q.front();
	q.pop();

	if(curr->balance < 0){
		cout << curr->first << " " << curr->last << " " <<  setfill('0') << setw(5) << curr->id << " " << curr->balance << endl;
		flag = true;
	}

	if (curr->left != NULL)
		q.push(curr->left);
	if (curr->right != NULL)
		q.push(curr->right);
	}

	if(flag == false){ cout << "No Account With Negetive Assets Exist!" << endl;}
}


void levelOrderGt(Node *x, int gt) {
	bool flag = false;
	if (x == NULL)
	return;

	queue<Node *> q;
	Node *curr;

	q.push(x);

	while (!q.empty()) {

	curr = q.front();
	q.pop();

	if(curr->balance >= gt){
		cout << curr->first << " " << curr->last << " " <<  setfill('0') << setw(5) << curr->id << " " << curr->balance << endl;
		flag = true;
	}

	if (curr->left != NULL)
		q.push(curr->left);
	if (curr->right != NULL)
		q.push(curr->right);
	}

	if(flag == false){ cout << "No Account With The Given Assets Exist!" << endl;}
}


void postorderMinusDelete(Node *x, int& mcnt, priority_queue <int>& v) {

	if (x == NULL)
		return;

	postorderMinusDelete(x->left, mcnt, v);
	postorderMinusDelete(x->right, mcnt, v);

	if (x->balance < 0){
		v.push((-1)* (x->id));
		deleteNode(x);
		mcnt ++;
	}
}

void postorderFindRichest(Node *x, vector <vector<string>>& v, int& max) {

	if (x == NULL)
		return;
	
	postorderFindRichest(x->left, v, max);
	postorderFindRichest(x->right, v, max);

	if (x->balance > max){
		max = x->balance;
		v.clear();
		vector <string> richestTemp = {x->first, x->last, to_string(x->id), to_string(x->balance)};
		v.push_back(richestTemp);
	}
	else if (x->balance == max){
		vector <string> richestTemp = {x->first, x->last, to_string(x->id), to_string(x->balance)};
		v.push_back(richestTemp);
	}
}

public:

RBTree() { root = NULL; }

Node *getRoot() { return root; }

Node *search(int n) {

	Node *temp = root;

	while (temp != NULL) {
	if (n < temp->id) {
		if (temp->left == NULL)
			break;
		else
			temp = temp->left;
	} 
	else if (n == temp->id) {
		break;
	} 
	else {
		if (temp->right == NULL)
			break;
		else
			temp = temp->right;
	}
	}

	return temp;
}

Node *searchWithNull(int n) {

	Node *temp = root;
	bool fnd = false;

	while (temp != NULL) {
		if (n < temp->id) {
			if (temp->left == NULL)
				break;
			else
				temp = temp->left;
		} 
		else if (n == temp->id) {
			fnd = true;
			break;
		} 
		else {
			if (temp->right == NULL)
				break;
			else
				temp = temp->right;
		}
	}

	if (fnd ==true)
		return temp;
	
	else
		return NULL;
}

Node *change(int n, int chng) {

	Node *temp = root;
	bool fnd = false;

	while (temp != NULL) {

		if (n < temp->id) {
			if (temp->left == NULL)
				break;
			else
				temp = temp->left;
		} 
		else if (n == temp->id) {
			fnd = true;
			temp->balance += chng;
			break;
		} 
		else {
			if (temp->right == NULL)
				break;
			else
				temp = temp->right;
		}
	}

	if (fnd ==true)
		return temp;
	
	else
		return NULL;
}

void insert(int n, string f, string l, int b) {

	Node *newNode = new Node(n, f, l, b);

	if (root == NULL) {
		newNode->color = BLACK;
		root = newNode;
	} 

	else {
		Node *temp = search(n);

		if (temp->id == n) {
			return;
	}

	newNode->parent = temp;

	if (n < temp->id)
		temp->left = newNode;
	else
		temp->right = newNode;

	fixDoubleRed(newNode);
	}
}

void deleteById(int n) {
	if (root == NULL)
		return;

	Node *x = search(n), *y;

	if (x->id != n) {
		cout << "No Account Found With The Id: " << n << endl;
		return;
	}

	deleteNode(x);
}

int deleteMinus(priority_queue <int>& v) {
	int mcnt = 0;

	if (root == NULL)
		cout << "Tree is empty" << endl;
	else
		postorderMinusDelete(root, mcnt, v);

	return mcnt;
}

void searchMinus() {

	if (root == NULL)
		cout << "Database Is Empty!" << endl;
	else
		levelOrderMinusSearch(root);
}

void searchGt(int gt) {

	if (root == NULL)
		cout << "Database Is Empty!" << endl;
	else
		levelOrderGt(root, gt);
}

void findRichest(vector <vector<string>>& richest) {
	 
	int max = -2147483647 ;
	if (root == NULL)
		cout << "Database Is Empty!" << endl;
	else
		postorderFindRichest(root, richest, max);
}


void saveLine(ofstream &fout, Node* x){

	string line;
	if (x == NULL)
		return;

	saveLine(fout, x->left);
	saveLine(fout, x->right);
	line = to_string(x->id) + " " + x->first +" " + x->last+ " "  + to_string(x->balance);
	fout << line << endl;
		
}

void saveDatabase(ofstream &fout) {

	if (root == NULL)
		cout << "Database Is Empty!" << endl;
	else
		saveLine(fout, root);
	
}


void loadLine(ifstream &fin, Node* x){

	string id, first, last, balance;

	while (!fin.eof( )){
		fin >> id >> first >> last >> balance;
		insert(stoi(id), first, last, stoi(balance));
	}		
}

void loadDatabase(ifstream &fin) {
	loadLine(fin, root);
}

};

struct TrieNode
{
	struct TrieNode *children[ALPHABET_SIZE];
	bool isEOW; //end of word
};


struct TrieNode *getNode(void){
	struct TrieNode *newNode = new TrieNode;

	newNode->isEOW = false;

	for (int i = 0; i < ALPHABET_SIZE; i++)
		newNode->children[i] = NULL;

	return newNode;
}


void insertTrie(struct TrieNode *root, string key){		
	struct TrieNode *currNode = root;

	for (int i = 0; i < key.length(); i++){	

		if(key[i] != 32){
			int index = key[i] - 'a';
			if (!currNode->children[index])
				currNode->children[index] = getNode();

			currNode = currNode->children[index];
		}

		else{
			int index = 26;
			if (!currNode->children[index])
				currNode->children[index] = getNode();

			currNode = currNode->children[index];

		}
	}

	currNode->isEOW = true;
}

bool searchTrie(struct TrieNode *root, string key){
	struct TrieNode *tempNode = root;

	for (int i = 0; i < key.length(); i++)
	{
		if(key[i] != 32){
			int index = key[i] - 'a';
			if (!tempNode->children[index])
				return false;

			tempNode = tempNode->children[index];
		}
		else{
			int index = 26;
			if (!tempNode->children[index])
				return false;

			tempNode = tempNode->children[index];
		}
	}
	return (tempNode->isEOW);
}

void deleteTrie(struct TrieNode *root, string key){
	struct TrieNode *tempNode = root;

	for (int i = 0; i < key.length(); i++)
	{
		if(key[i] != 32){
			int index = key[i] - 'a';

			tempNode = tempNode->children[index];
		}
		else{
			int index = 26;

			tempNode = tempNode->children[index];
		}
	}

	tempNode->isEOW = false;
}

void commandExecute(RBTree tree){

	struct TrieNode *names = getNode();

	vector <vector<string>> richest;
	priority_queue <int> ids;
	ids.push(0);

    
	int acc_id;
	int val;
	int cnt = 0;

	string com; //command
	string f; //first name
	string l; //last name
	string n; //full name

    while(true){

		if((cnt != 0) && richest.size() == 0){
			tree.findRichest(richest);
		}

		if (!(std::cin >> com)) { cout<< "error" << endl;}

        if(com == "EXIT"){
			ofstream fout;
			fout.open("Database.txt");
			tree.saveDatabase(fout);
			fout.close();

			fout.open("Richest.txt");
			fout << richest.size() << endl;		
			for (int i = 0; i < richest.size(); i++) {
				string line = "";
        		for (int j = 0; j < 4; j++) {
					if (j == 0){
						line = richest[i][j];
					}
					else{
						line = line + " " + richest[i][j];
					}
      			}
				fout << line << endl;
			}		
			fout.close();

			priority_queue <int> idsTemp;
			fout.open("Ids.txt"); //contains cnt and ids
			int idSize = ids.size();
			fout << to_string(cnt) << endl;
			fout << ids.size() << endl;
			for (int i = 0; i < idSize ; i++){
				fout << (ids.top()*(-1)) << endl;
				idsTemp.push(ids.top());
				ids.pop();
			}
			ids = idsTemp;
			fout.close();
            break;
		}

        else if(com == "LOAD"){
 			ifstream fin;
 			fin.open("Database.txt");
			tree.loadDatabase(fin);
			fin.close();

			fin.open("Database.txt");
			string id2, first, last, balance2;
			while (!fin.eof( )){
				fin >> id2 >> first >> last >> balance2;
				n  = first + " " + last;
				insertTrie(names, n);
			}
			fin.close();

			fin.open("Ids.txt"); //contains cnt and ids
			ids = priority_queue <int>();
			string id, first1, last1, balance1;
			
			string scnt;
			fin >> scnt;
			cnt = stoi(scnt);

			string sidSize;
			int idSize;
			fin >> sidSize;
			idSize = stoi(sidSize);
			

			for (int i = 0; i < idSize ; i++){
				fin >> id;
				ids.push((-1)* stoi(id));
			}
			
			fin.close();

			fin.open("Richest.txt");
			string sizeOfRichest;
			fin >> sizeOfRichest;
			for(int i = 0; i < stoi(sizeOfRichest); i++){
				fin >> first1 >> last1  >> id >> balance1 ;
				vector <string> richestTemp = {first1, last1, id, balance1};
				richest.push_back(richestTemp);
			}
			fin.close();

			cout << "SUCCESS" << endl;
        }   

        else if(com == "SAVE"){
			ofstream fout;
			fout.open("Database.txt");
			tree.saveDatabase(fout);
			fout.close();

			fout.open("Richest.txt");
			fout << richest.size() << endl;		
			for (int i = 0; i < richest.size(); i++) {
				string line = "";
        		for (int j = 0; j < 4; j++) {
					if (j == 0){
						line = richest[i][j];
					}
					else{
						line = line + " " + richest[i][j];
					}
      			}
				fout << line << endl;
			}		
			fout.close();

			priority_queue <int> idsTemp;
			fout.open("Ids.txt");
			int idSize = ids.size();
			fout << to_string(cnt) << endl;
			fout << ids.size() << endl;
			for (int i = 0; i < idSize ; i++){
				fout << (ids.top()*(-1)) << endl;
				idsTemp.push(ids.top());
				ids.pop();
			}
			ids = idsTemp;
			fout.close();

			cout << "SUCCESS" << endl;
		}

        else if(com == "SEARCH"){

            if (!(std::cin >> com)) { cout<< "error" << endl;}

			if(com == "MINUS"){
				tree.searchMinus();
			}

			else if(com == "RICHEST"){
				bool flg = false;
				for (int i = 0; i < richest.size(); i++) {
        			for (int j = 0; j < 4; j++) {
						if (j == 2){
							cout << setfill('0') << setw(5) << stoi(richest[i][j]) << ' ';
						}
						else{
						cout <<richest[i][j] << ' ';
						flg = true;
						}
					}
					cout << endl;
				}
				if(flg == false) {cout << "Database Is Empty!" << endl;}
			}

			else if(com == "GT"){
				if (!(std::cin >> com)) { cout<< "error" << endl;}
				val = stoi(com);
				tree.searchGt(val);
				
			}

			else{
				acc_id = stoi(com);
				if (tree.searchWithNull(acc_id) != NULL) {
					Node *temp = tree.search(acc_id);
					cout << temp->first << " " << temp->last << " " << temp->balance << endl;
				}
				else
					cout << "An Account With This Account Number Doesn't Exist!" << endl;
			}    			
        }

        else if(com == "REGISTER"){

            if (!(std::cin >> com)) { cout<< "error" << endl;}
			f = com;
			if (!(std::cin >> com)) { cout<< "error" << endl;}
			l = com;
			if (!(std::cin >> com)) { cout<< "error" << endl;}
			val = stoi(com);
			n = f + " " + l;

			if(searchTrie(names, n) == false){
				tree.insert((ids.top())*(-1), f, l, val);
				insertTrie(names, n);
				

				if(cnt == 0){ //first customer
					vector <string> richestTemp = {f, l, to_string((ids.top())*(-1)), to_string(val)};
					richest.push_back(richestTemp);
				}
				else if(val > stoi(richest[0][3])){ 
					richest.clear(); 
					vector <string> richestTemp = {f, l, to_string((ids.top())*(-1)), to_string(val)};
					richest.push_back(richestTemp);     
				}
				else if(val == stoi(richest[0][3])){
					vector <string> richestTemp = {f, l, to_string((ids.top())*(-1)), to_string(val)};
					richest.push_back(richestTemp);
				}
				ids.pop();
				if(ids.empty()){
					cnt++;
					ids.push((-1) * cnt);
				}
				
				cout << "SUCCESS" << endl;
			}
			else
				cout << "An Account With This First And Last Name Already Exists!" << endl;
			
        }

        else if(com == "DELETE"){
            if (!(std::cin >> com)) { cout<< "error" << endl;}

			if(com == "MINUS"){
				cout << tree.deleteMinus(ids) << "_deleted." << endl;
			}
			else{
				acc_id = stoi(com);
				Node* temp3 = tree.searchWithNull(acc_id);
				if (temp3 != NULL) {
						n = temp3->first + " " + temp3->last;
						deleteTrie(names, n);

						for(int i = 0; i < richest.size(); i++){ //check to see if he should be deleted from richest
						if( (temp3->first == richest[i][0]) && (temp3->last == richest[i][1]) )
							richest.erase(richest.begin() + i);
						}

						tree.deleteById(acc_id);
						ids.push((-1) * acc_id);
						cout << "SUCCESS" << endl;

				}
				else
					cout << "An Account With This Account Number Doesn't Exist!" << endl;
				
			}
        }

        else if(com == "CHANGE"){
            if (!(std::cin >> com)) { cout<< "error" << endl;}
			acc_id = stoi(com);
			if (!(std::cin >> com)) { cout<< "error" << endl;}
			val = stoi(com);

			if (tree.searchWithNull(acc_id) != NULL) {
				Node *temp = tree.change(acc_id, val);
				cout << temp->first << " " << temp->last << " " <<  setfill('0') << setw(5) << temp->id << " " << ((temp->balance) - val) << " "<< temp->balance << endl;
				
				if(temp->balance > stoi(richest[0][3])){ 
					richest.clear(); 
					vector <string> richestTemp = {temp->first, temp->last, to_string(acc_id), to_string(temp->balance)};
					richest.push_back(richestTemp);     
				}
				else if(temp->balance == stoi(richest[0][3])){
					vector <string> richestTemp = {temp->first, temp->last, to_string(acc_id), to_string(temp->balance)};
					richest.push_back(richestTemp);
				}
			}
			else
				cout << "An Account With This Account Number Doesn't Exist!" << endl;

        }

        else if(com == "REWARD"){
            cout << "Due To Some Planning Problems, This Option Could Not Be Added In Time :)" << endl;
        }

		else
			cout << "ERROR: Unknown command" << endl;
    }
}



int main(){

    RBTree tree;
    commandExecute(tree);

    return 0;
}