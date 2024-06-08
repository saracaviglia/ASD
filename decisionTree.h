#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "list-array.h"
#include "string-utility.h"

using namespace std;

namespace decTree{

	enum Error {OK, FAIL};
	enum Operator {MAGGIORE, MINORE, UGUALE, MAGGIORE_UGUALE, MINORE_UGUALE, DIVERSO, SCONOSCIUTO};
	typedef string Label;
	const Label emptyLabel = "$#$#$";
	struct Elem{
		Label condition;	//Condizione per entrare nel nodo
		Label variable;		//Etichetta del nodo
	};
	struct node{
		Elem details;
		node* firstChild;	//Puntatore al primo figlio
		node* nextSibling;	//Puntatore al prossimo fratello
	};
	typedef node* decisionTree;
	const decisionTree emptyTree = nullptr;
	
	decisionTree createEmpty();
	bool isEmpty(const decisionTree&);
	bool member(const Label, const decisionTree&);
	Error addElem(const Label, const Label, const Label, decisionTree&);
	Error deleteElem(const Label, decisionTree&);
	Error setNode(const Label, const Label, decisionTree&);
	Label predictionBySteps(decisionTree&);
	Label prediction(decisionTree&);

}

//Funzioni fuori dal TDD
decTree::decisionTree readFromFile(string);
decTree::Label father(const decTree::Label, const decTree::decisionTree&);
list::List children(const decTree::Label, const decTree::decisionTree&);
void printTree(const decTree::decisionTree&);
void printVariables(const decTree::decisionTree&);
void printVariablesToInsert(const decTree::decisionTree&);
bool isANumber(decTree::Label);
int str2int(const string&);
decTree::Operator whichOperator(decTree::Label);
decTree::Label normalizeVariables(decTree::Label);
bool isEnd(decTree::decisionTree& t);
