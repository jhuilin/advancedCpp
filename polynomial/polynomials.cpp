#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

class Node {
	int coeficient;
	int power;
	Node* next;

	void parseTerm(string term, int &co, int& po) {
		int e = 0;
		int coeficient_positive = 1;
		int power_positive = 1;

		for (int i = 0; i < term.length(); i++) {
			if (e == 1) {
				if (term[i] == '-') {
					power_positive = -1;
				}
				else if (isdigit(term[i])) {
					po = po * 10 + (term[i] - '0');
				}
				else {
					cout << "invalid polynomial." << endl;
					exit(1);
				}
			}
			else if (term[i] == '-') {
				coeficient_positive = -1;
			}
			else if (isdigit(term[i])) {
				co = co * 10 + (term[i] - '0');
			}
			else if (term[i] == 'x') {
				if (co == 0) {
					co = 1;
				}
				if (i + 1 >= term.length()) {
					po = 1;
				}
			}
			else if(term[i] == '^'){
				e = 1;
			}
			else {
				cout << "invalid polynomial." << endl;
				exit(1);
			}
		}
		co *= coeficient_positive;
		po *= power_positive;
	}

public:
	Node() {
		coeficient = 0;
		power = 0;
		next = NULL;
	}

	Node(int c, int e, Node* n) {
		coeficient = c;
		power = e;
		next = n;
	}

	void addNode(string term) {
		int co = 0;
		int ex = 0;
		parseTerm(term, co, ex);
		Node* current = this;
		if (current) {
			while (current->next && current->next->power >= ex) {
				if (current->next->power == ex) {
					current->next->coeficient += co;
					return;
				}
				current = current->next;
			}
			if (current) {
				current->next = new Node(co, ex, current->next);
				return;
			}
		}
		next = new Node(co, ex, NULL);
	}

	void addNode(Node* newNode) {
		int co = newNode->coeficient;
		int ex = newNode->power;
		Node* current = this;
		if (current) {
			while (current->next && current->next->power >= ex) {
				if (current->next->power == ex) {
					current->next->coeficient += co;
					return;
				}
				current = current->next;
			}
			if (current) {
				current->next = new Node(co, ex, current->next);
				return;
			}
		}
		next = new Node(co, ex, NULL);
	}

	void print(ofstream &output) {
		int c = 0;
		Node* current = next;
		while (current) {
			if (c == 0) {
				c = 1;
				if (current->power == 1) {
					if (current->coeficient == 1) {
						output << "x";
					}
					else if (current->coeficient == -1) {
						output <<"-x";
					}
					else {
						output << to_string(current->coeficient) << "x";
					}
				}
				else if (current->power != 0) {
					if (current->coeficient == 1) {
						output << "x^" << to_string(current->power);
					}
					else if (current->coeficient == -1) {
						output << "-x^" << to_string(current->power);
					}
					else {
						output << to_string(current->coeficient) << "x^" << to_string(current->power);
					}
				}
				else {
					output << to_string(current->coeficient);
				}
			}
			else if (current->coeficient == 1) {
				c = 1;
				output << "+";
				if (current->power == 1) {
					output << "x";
				}
				else if (current->power != 0) {
					output << "x^" << to_string(current->power);
				}
				else{
					output << to_string(current->coeficient);
				}
			}
			else if (current->coeficient < 0) {
				c = 1;
				if (current->power == 0) {
					output << to_string(current->coeficient);
				}
				else {
					if (current->coeficient == -1) {
						output << "-";
					}
					else {
						output << to_string(current->coeficient);
					}
					if (current->power == 1) {
						output << "x";
					}
					else{
						output << "x^" << to_string(current->power);
					}
				}
			}
			else if(current->coeficient > 1){
				c = 1;
				output << "+";
				output << to_string(current->coeficient);
				if (current->power == 1) {
					output << "x";
				}
				else if (current->power != 0) {
					output << "x^" << to_string(current->power);
				}
			}
			current = current->next;
		}
	}

	Node* addition(Node* head) {
		Node* result = new Node();
		Node* current = result;
		Node* left = next;
		Node* right = head->next;
		Node* newNode = NULL;
		while (left || right) {
			if (left == NULL) {
				newNode = new Node(right->coeficient, right->power, NULL);
				right = right->next;
			}
			else if (right == NULL) {
				newNode = new Node(left->coeficient, left->power, NULL);
				left = left->next;
			}
			else if (right->power > left->power) {
				newNode = new Node(right->coeficient, right->power, NULL);
				right = right->next;
			}
			else if (left->power > right->power) {
				newNode = new Node(left->coeficient, left->power, NULL);
				left = left->next;
			}
			else if(left->power == right->power){
				if (left->coeficient + right->coeficient != 0) {
					newNode = new Node(left->coeficient + right->coeficient, left->power, NULL);
				}
				else {
					newNode = NULL;
				}
				right = right->next;
				left = left->next;
			}
			if (newNode) {
				current->next = newNode;
				current = current->next;
			}
		}
		return result;
	}

	Node* subtraction(Node* head) {
		Node* p2 = head->next;
		Node* newNode = new Node();
		while(p2 != NULL){
			Node* temp = new Node(p2->coeficient * -1, p2->power, NULL);
			newNode->addNode(temp);
			p2 = p2->next;
		}
		return addition(newNode);
	}

	Node* multiplication(Node* head) {
		Node* result = new Node();
		Node* left = next;
		Node* right = head->next;
		Node* newNode = NULL;
		while (left) {
			Node* right = head->next;
			while (right) {
				newNode = new Node(left->coeficient * right->coeficient, left->power + right->power, NULL);
				result->addNode(newNode);
				right = right->next;
			}
			left = left->next;
		}
		return result;
	}

	~Node() {
		delete next;
	}
};

int main() {
	ifstream infile("input.txt");
	ofstream output("output.txt");
	string line;
	Node* head1;
	Node* head2;
	Node* result;

	regex r("((-?)[\\d]*x((\\^(-?))[\\d]*)?)|((-?)[\\d]*)");
	smatch m;

	while (getline(infile, line)) {
		sregex_iterator currentrentMatch(line.begin(), line.end(), r);
		sregex_iterator lastMatch;
		head1 = new Node();
		while (currentrentMatch != lastMatch) {
			smatch m = *currentrentMatch;
			string term = m.str();
			if (term != "") {
				head1->addNode(term);
			}
			currentrentMatch++;
		}
		output << line << endl;
		head1->print(output);
		output << endl;
		if (getline(infile, line)) {
			sregex_iterator currentrentMatch(line.begin(), line.end(), r);
			sregex_iterator lastMatch;
			head2 = new Node();
			while (currentrentMatch != lastMatch) {
				smatch m = *currentrentMatch;
				string term = m.str();
				if (term != "") {
					head2->addNode(term);
				}
				currentrentMatch++;
			}

			output << line << endl;
			head2->print(output);
			output << endl;
			//add result
			output << "(";
			head1->print(output);
			output << ") + (";
			head2->print(output);
			output << ") = ";
			head1->addition(head2)->print(output);
			output << endl;
			//sub result
			output << "(";
			head1->print(output);
			output << ") - (";
			head2->print(output);
			output << ") = ";
			head1->subtraction(head2)->print(output);
			output << endl;
			//mul result
			output << "(";
			head1->print(output);
			output << ") * (";
			head2->print(output);
			output << ") = ";
			head1->multiplication(head2)->print(output);
			output << endl;
		}
		else {
			cout << "missing polynimial.\n";
		}
	}
	infile.close();
	output.close();
	return 0;
}
