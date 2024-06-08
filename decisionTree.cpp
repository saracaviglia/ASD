#include "decisionTree.h"

using namespace decTree;

//Alcune funzioni sono prese dai laboratori del gruppo 45, i cui membri sono Sara Caviglia (5163676), Martina Malcuit (5260784) e Lorenzo Mian (5222582).

//Funzione presa dal laboratorio 7 del gruppo 45
decisionTree getNode(const Label l, const decisionTree& t){
	if(isEmpty(t) || l == emptyLabel) return emptyTree;
	if(t->details.variable == l) return t;
	decisionTree aux = t->firstChild;
	if(aux != emptyTree && getNode(l, aux) != emptyTree) return getNode(l, aux);
	aux = t->nextSibling;
	if(aux != emptyTree && getNode(l, aux) != emptyTree) return getNode(l, aux);
	return emptyTree;
}

/**********FUNZIONI CHE NON FANNO PARTE DEL TDD**********/
//Funzione presa dal laboratorio 7
decTree::decisionTree readFromStream(istream& str){
    decisionTree t = createEmpty(); 
    string line;
    Label rootLabel, fatherLabel, childLabel, condition;
    getline(str, line);  
    istringstream instream;            
    instream.clear();
    instream.str(line);
    instream >> rootLabel; 
    removeBlanksAndLower(rootLabel);            
    addElem(emptyLabel, rootLabel, emptyLabel, t); 
    getline(str, line);                
    instream.clear();
    instream.str(line);
    while (!str.eof()){        
		instream >> fatherLabel; 
        removeBlanksAndLower(fatherLabel); 
        while (!instream.eof()){            
            instream >> childLabel;              
            removeBlanksAndLower(childLabel);  
            instream >> condition;              
            removeBlanksAndLower(condition); 
            addElem(fatherLabel, childLabel, condition, t); 
        }
        getline(str, line);  
        instream.clear();
        instream.str(line);
    }
    str.clear();
    return t;
}

//Funzione presa dal laboratorio 7
decTree::decisionTree readFromFile(string nome_file){
	ifstream ifs(nome_file.c_str());
	if(!ifs){
		cout << "\nErrore apertura file, verificare di avere inserito un nome corretto\n";
		return createEmpty();
	}
	return readFromStream(ifs);
}

//Funzione presa dal laboratorio 7 del gruppo 45 che permette di trovare il padre di un nodo data la sua label
Label father(const Label l, const decisionTree& t){
	Label label = emptyLabel;
	if(isEmpty(t)) return emptyLabel;
	list::List a = children(t->details.variable, t);
	for(int i = 0; i < a.size; i++){
		if(a.list[i] == l) label = t->details.variable;
	}
	if(label != emptyLabel) return label;
	decisionTree aux = t->firstChild;
	if(aux != emptyTree && label == emptyLabel) label = father(l, aux);
	if(label != emptyLabel) return label;
	aux = t->nextSibling;
	if(aux != emptyTree && label == emptyLabel) label = father(l, aux);
	if(label != emptyLabel) return label;
	return emptyLabel;
}

//Funzione presa dal laboratorio 7 del gruppo 45 che permette di creare una lista con i figli di un dato nodo
list::List children(const Label l, const decisionTree& t){
	list::List lst = list::createEmpty();
	if(!member(l, t)) return lst;
	decisionTree father = getNode(l, t);
	if(father->firstChild != emptyTree) list::addBack(father->firstChild->details.variable, lst);
	else return lst;
	father = father->firstChild;
	while(father->nextSibling != emptyTree){
		list::addBack(father->nextSibling->details.variable, lst);
		father = father->nextSibling;
	}
	return lst;
}

//Funzione presa dal laboratorio 7 che visita l'albero e inserisce i vari nodi all'interno di una lista
void visit(const decisionTree& t, list::List& a){
	if (isEmpty(t)) return;
	list::addBack(t->details.variable, a); 
	decisionTree aux = t->firstChild;
	while (!isEmpty(aux)) {
		visit(aux, a);
		aux = aux->nextSibling;
	}
}

//Funzione ausiliaria ricorsiva della printTree; la stampa avviene nel formato (Variabile, Condizione per entrare nel nodo)
void printTreeAux(const decisionTree& t, int depth){
	if (isEmpty(t)) return;
    for(int j = 0; j < depth; j++)
    	cout << "--";
    if(t->details.condition == emptyLabel) cout << t->details.variable << endl;
    else cout << "(" << t->details.variable << ", " << t->details.condition << ")" << endl;
    decisionTree aux = t->firstChild;
    while(!isEmpty(aux)){
    	printTreeAux(aux, depth + 1);
    	aux = aux->nextSibling;
    }
}

//Funzione principale della stampa che richiama la funzione printTreeAux	
void printTree(const decisionTree& t){
	printTreeAux(t, 0);
}

//Funzione che stampa le variabili dell'albero di decisione
void printVariables(const decisionTree& t){
	list::List a = list::createEmpty();
	list::List b = list::createEmpty();
	visit(t, a);
	for(int i = 0; i < a.size; i++){
		a.list[i] = normalizeVariables(a.list[i]);
		//Elimina i doppioni e non aggiunge i nodi etichettati con "end"	
		if(!list::member(a.list[i], b) && a.list[i] != "end") list::addBack(a.list[i], b); 
	}	
	cout << "Le variabili sono:\n";
	for(int i = 0; i < b.size; i++)	
		cout << i+1 << ". " << b.list[i] << endl;
}

//Variazione della printVariables che permette di stampare le variabili da inserire nella prediction
void printVariablesToInsert(const decisionTree& t){
	list::List a = list::createEmpty();
	list::List b = list::createEmpty();
	visit(t, a);
	for(int i = 0; i < a.size; i++){
		a.list[i] = normalizeVariables(a.list[i]);	
		if(!list::member(a.list[i], b) && a.list[i] != "end") list::addBack(a.list[i], b); 
	}	
	for(int i = 0; i < b.size; i++)	
		//Non stampa la variabile "rischio" perché è ciò che poi dovrò calcolare nella prediction
		if(b.list[i] != "rischio")
			cout << b.list[i] << endl;
}

//Funzione che restituisce il numero di variabili
int numVariables(const decisionTree& t){
	list::List a = list::createEmpty();
	list::List b = list::createEmpty();
	visit(t, a);
	for(int i = 0; i < a.size; i++){
		a.list[i] = normalizeVariables(a.list[i]);	
		if(!list::member(a.list[i], b) && a.list[i] != "end") list::addBack(a.list[i], b); 
	}	
	return b.size;
}

//Funzione che restituisce un array dinamico di variabili
list::List arrayVariables(const decisionTree& t){
	list::List a = list::createEmpty();
	list::List b = list::createEmpty();
	visit(t, a);
	for(int i = 0; i < a.size; i++){
		a.list[i] = normalizeVariables(a.list[i]);	
		if(!list::member(a.list[i], b) && a.list[i] != "end" && a.list[i] != "rischio") list::addBack(a.list[i], b); 
	}	
	return b;
}

//Funzione che restituisce true se l'etichetta è un numero
bool isANumber(Label insert){
	for(int i = 0; i < sizeof(insert); i++)
		if(!isdigit(insert[i])) return false;
	return true;
}

//Funzione presa dal laboratorio 4 per convertire le stringhe in int
int str2int(const string &s) {
	int n;
	istringstream itmp(s);
	itmp >> n;
	return n;
}

//Funzione che, data una condizione dell'albero di decisione, restituisce l'operatore che si trova all'inizio della stringa stessa
decTree::Operator whichOperator(Label value){
	if(value[0] == '=') return UGUALE;
	if(value[0] == '!' && value[0] == '=') return DIVERSO;
	if(value[0] == '<')
		if(value[1] == '=') return MINORE_UGUALE;
		else return MINORE;
	if(value[0] == '>')
		if(value[1] == '=') return MAGGIORE_UGUALE;
		else return MAGGIORE;
	return SCONOSCIUTO;
}

//Funzione che, data una variabile del tipo "var_num", restituisce solo il nome della variabile (ad esempio se l'input è "tipo_1", restituisce "tipo")
Label normalizeVariables(Label var){
	if(var.empty()) return emptyLabel;
	int pos = var.find("_", 0);
	var.erase(pos, sizeof(var));
	return var;
}

//Funzione che restituisce true se il nodo è una foglia
bool isEnd(decisionTree& t){
	return t->firstChild == emptyTree;
}

/**********FUNZIONI DEL TDD**********/
//La funzione crea un albero vuoto.
decisionTree decTree::createEmpty(){
	decisionTree t = emptyTree;
	return t;
}

//La funzione ritorna se l'albero è vuoto.
bool decTree::isEmpty(const decisionTree& t){
	return t == emptyTree;
}

//La funzione verifica se un elemento con etichetta l è già presente nell'albero.
bool decTree::member(const Label l, const decisionTree& t){
    list::List a = list::createEmpty();
    visit(t,a);
    for(int i=0; i<a.size; i++){
        if(a.list[i] == l) return true; 
    }
    return false;
}

//La funzione aggiunge un nuovo elemento nell'albero, "attacandolo" al nodo con etichetta labelPadre.
Error decTree::addElem(const Label labelPadre, const Label labelFiglio, const Label condizione, decisionTree& t){
	//Preparazione del nodo
	decisionTree aux = new node;
	aux->details.variable = labelFiglio;
	aux->details.condition = condizione;
	aux->firstChild = emptyTree;
	aux->nextSibling = emptyTree;
	//Controlli (verifichiamo se l'albero è vuoto, se labelPadre non è presente o se labelFiglio è già presente)
	if(labelPadre == emptyLabel && isEmpty(t)){ 
		t = aux;
		return OK;
	}
	if(labelPadre == emptyLabel && !isEmpty(t)) return FAIL;
    if (!member(labelPadre, t) || member(labelFiglio, t)) return FAIL;
    //Inserimento del nuovo nodo
    decisionTree father = getNode(labelPadre, t);
    if(father->firstChild != emptyTree){
    	decisionTree cur = father->firstChild;
    	while(!isEmpty(cur->nextSibling))
    		cur = cur->nextSibling;
    	cur->nextSibling = aux;
    } else father->firstChild = aux;
    return OK;
}

//La funzione elimina l'elemento etichettato con l.
Error decTree::deleteElem(const Label l, decisionTree& t){
	//Controlli (verifichiamo il nodo da eliminare è presente)
	if(!member(l, t)) return FAIL;
	//Prendo il nodo padre del nodo da eliminare
	decisionTree fatherTree = getNode(father(l, t), t);	
	//Rimozione del nodo in casi particolari
	if(isEmpty(fatherTree)){
		if(!list::isEmpty(children(l, t))) return FAIL; 
		else{t = emptyTree; return OK;}
	} else {
		//Rimozione del nodo in casi "normali"
		decisionTree nodeToRemove = getNode(l, t);
		decisionTree brother = nodeToRemove->nextSibling;
		decisionTree cur = nodeToRemove;
		nodeToRemove->nextSibling = nodeToRemove->firstChild;
		//Scorro i fratelli
		if(brother != emptyTree){
			while(cur->nextSibling != emptyTree){
				cur = cur->nextSibling;
			}
			cur->nextSibling = brother;
		}
		//Se il nodo da eliminare è il primo figlio
		if(nodeToRemove->details.variable == fatherTree->firstChild->details.variable) fatherTree->firstChild = nodeToRemove->nextSibling;
		//Se il nodo da eliminare non è il primo figlio
		else{
			cur = fatherTree->firstChild;
			while(cur->nextSibling->details.variable != nodeToRemove->details.variable){
				cur = cur->nextSibling;
			}
			cur->nextSibling = nodeToRemove->nextSibling;
		}
		//Eliminazione vera e propria del nodo
		delete nodeToRemove;
	}
	return OK;
}

//La funzione permette di cambiare l'etichetta di un nodo esistente
Error decTree::setNode(const Label variableToChange, const Label newVariable, decisionTree& t){
	//Controlli (verifichiamo che l'albero non sia vuoto e che l'etichetta da modificare esista nell'albero)
	if(isEmpty(t) || !member(variableToChange, t)) return FAIL;
	//Prendo il nodo e ne cambio l'etichetta
	decisionTree aux = getNode(variableToChange, t);
	aux->details.variable = newVariable;
	return OK;
}

//La funzione permette di effettuare una predizione chiedendo i dati uno alla volta.
Label decTree::predictionBySteps(decisionTree& t){
	if(isEmpty(t)) return emptyLabel;
	decisionTree aux = t;
	Label insert, cond = aux->firstChild->details.condition;
	Label error = "IMPOSSIBILE", message = "La predizione non può avere luogo in quanto esiste un nodo per il quale non c'è un arco percorribile\n";
	int insertInt, valueInt;
	Operator op;
	if(isEnd(aux->firstChild)) return aux->firstChild->details.condition;
	while(!isEnd(aux->firstChild)){
		//Chiedo di inserire i dati per una data variabile
		cout << "Inserire il valore per la variabile " << normalizeVariables(aux->details.variable) << ": ";
		cin >> insert;
		//Determino l'operatore della condizione necessaria per entrare nel nodo primo figlio
		op = whichOperator(aux->firstChild->details.condition);
		//Elimino l'operatore dalla stringa per poter effettuare il confronto
		if(op == UGUALE || op == MINORE || op == MAGGIORE) cond = (aux->firstChild->details.condition).substr(1, (aux->firstChild->details.condition).size());
		else if(op == DIVERSO || op == MINORE_UGUALE || op == MAGGIORE_UGUALE) cond = (aux->firstChild->details.condition).substr(2, (aux->firstChild->details.condition).size());
		//Trasformo la stringa inserita e la condizione, ormai priva di operatore, in valori numerici
		insertInt = str2int(insert);
		valueInt = str2int(cond);
		//Ho casistiche diverse in base all'operatore della condizione
		switch(op){
			case UGUALE:
				//Se sono entrambi valori numerici
				if(isANumber(insert) && isANumber(cond)){
					if(insertInt == valueInt){
						//Se la condizione è verificata, passo al primo figlio e richiamo ricorsivamente la funzione
						aux = aux->firstChild;
						return predictionBySteps(aux);
					} else if(aux->firstChild->nextSibling != emptyTree)
						//Se la condizione non è verificata ma il primo figlio ha un fratello, mi sposto sul fratello per vedere se la sua condizione si verifica
						aux = aux->firstChild->nextSibling;
					//Se nessun fratello ha una condizione che si verifica, ritorno un messaggio di errore
					else return message;
				//Se sono entrambe stringhe (il funzionamento interno è uguale a quello sopra)
				} else if(!isANumber(insert) && !isANumber(cond)){
					if(insert == cond){
						aux = aux->firstChild;
						return predictionBySteps(aux);
					} else if(aux->firstChild->nextSibling != emptyTree)
						aux = aux->firstChild->nextSibling;
					else return message;
				} else return error;
			break;
			case DIVERSO:
				//Stesso funzionamento del case UGUALE
				if(isANumber(insert) && isANumber(cond)){
					if(insertInt != valueInt){
						aux = aux->firstChild;
						return predictionBySteps(aux);
					} else if(aux->firstChild->nextSibling != emptyTree)
						aux = aux->firstChild->nextSibling;
					else return message;
				} else if(!isANumber(insert) && !isANumber(cond)){
					if(insert != cond){
						aux = aux->firstChild;
						return predictionBySteps(aux);
					} else if(aux->firstChild->nextSibling != emptyTree)
						aux = aux->firstChild->nextSibling;
					else return message;
				} else return error;
			break;
			//Da qui in avanti, devo avere per forza dei valori numerici; in tutti i casi, se la condizione si verifica passo al primo figlio, altrimenti provo con un fratello, oppure mando un messaggio di errore
			case MINORE:
				if(insertInt < valueInt){
					aux = aux->firstChild;
					return predictionBySteps(aux);
				} else if(aux->firstChild->nextSibling != emptyTree)
					aux = aux->firstChild->nextSibling;
				else return message;
			break;
			case MAGGIORE:
				if(insertInt > valueInt){
					aux = aux->firstChild;
					return predictionBySteps(aux);
				} else if(aux->firstChild->nextSibling != emptyTree)
					aux = aux->firstChild->nextSibling;
				else return message;
			break;
			case MINORE_UGUALE:
				if(insertInt <= valueInt){
					aux = aux->firstChild;
					return predictionBySteps(aux);
				} else if(aux->firstChild->nextSibling != emptyTree)
					aux = aux->firstChild->nextSibling;
				else return message;
			break;
			case MAGGIORE_UGUALE:
				if(insertInt >= valueInt){
					aux = aux->firstChild;
					return predictionBySteps(aux);
				} else if(aux->firstChild->nextSibling != emptyTree)
					aux = aux->firstChild->nextSibling;
				else return message; 
			break;
			default:
			break;
		}
	}
	//Chiamata ricorsiva sui fratelli
	while(!isEnd(aux)){
		return predictionBySteps(aux);
		aux = aux->nextSibling;
	}	
	//Messaggio di errore se non ho trovato nessun arco percorribile
	return message;
}

Label predictionAux(decisionTree& t, list::List var, list::List cond, int i){
	//La prima parte della funzione è identica alla prima parte della predictionBySteps
	Operator op;
	Label condToEnter;
	Label error = "IMPOSSIBILE", message = "La predizione non può avere luogo in quanto esiste un nodo per il quale non c'è un arco percorribile.\n";
	int insertInt, valueInt, k = 0;
	decisionTree aux = t;
	if(isEnd(aux->firstChild) && (aux->firstChild->details.condition == cond.list[k])) return aux->firstChild->details.condition;
	op = whichOperator(aux->firstChild->details.condition);
	if(op == UGUALE || op == MINORE || op == MAGGIORE) condToEnter = (aux->firstChild->details.condition).substr(1, (aux->firstChild->details.condition).size());
	else if(op == DIVERSO || op == MINORE_UGUALE || op == MAGGIORE_UGUALE) condToEnter = (aux->firstChild->details.condition).substr(2, (aux->firstChild->details.condition).size());
	insertInt = str2int(cond.list[i]);
	valueInt = str2int(condToEnter);
	//Anche lo switch è come nella predictionBySteps, con la differenza che analizzo dei valori memorizzati in un array
	switch(op){
		case UGUALE:
			if(isANumber(cond.list[i]) && isANumber(condToEnter)){
				if(insertInt == valueInt){
					aux = aux->firstChild;
				} else if(aux->firstChild->nextSibling != emptyTree){
					aux = aux->firstChild->nextSibling;
					while(normalizeVariables(aux->details.condition) != cond.list[k] && !isEmpty(aux->nextSibling))
						aux = aux->nextSibling;
				} else return message;
			} else if(!isANumber(cond.list[i]) && !isANumber(condToEnter)){
				if(cond.list[i] == condToEnter){
					aux = aux->firstChild;
				} else if(aux->firstChild->nextSibling != emptyTree){
					aux = aux->firstChild->nextSibling;
					while(normalizeVariables(aux->details.condition) != cond.list[k] && !isEmpty(aux->nextSibling))
						aux = aux->nextSibling;
				} else return message;
			} else return error;
		break;
		case DIVERSO:
			if(isANumber(cond.list[i]) && isANumber(condToEnter)){
				if(insertInt != valueInt){
					aux = aux->firstChild;
				} else if(aux->firstChild->nextSibling != emptyTree){
					aux = aux->firstChild->nextSibling;
					while(normalizeVariables(aux->details.condition) != cond.list[k] && !isEmpty(aux->nextSibling))
						aux = aux->nextSibling;
				} else return message;
			} else if(!isANumber(cond.list[i]) && !isANumber(condToEnter)){
				if(cond.list[i] != condToEnter){
					aux = aux->firstChild;
				} else if(aux->firstChild->nextSibling != emptyTree){
					aux = aux->firstChild->nextSibling;
					while(normalizeVariables(aux->details.condition) != cond.list[k] && !isEmpty(aux->nextSibling))
						aux = aux->nextSibling;
				} else return message;
			} else return error;
		break;
		case MINORE:
			if(insertInt < valueInt){
				aux = aux->firstChild;
			} else if(aux->firstChild->nextSibling != emptyTree){
				aux = aux->firstChild->nextSibling;
				while(normalizeVariables(aux->details.condition) != cond.list[k] && !isEmpty(aux->nextSibling))
					aux = aux->nextSibling;
			} else return message;
		break;
		case MAGGIORE:
			if(insertInt > valueInt){
				aux = aux->firstChild;
			} else if(aux->firstChild->nextSibling != emptyTree){
				aux = aux->firstChild->nextSibling;
				while(normalizeVariables(aux->details.condition) != cond.list[k] && !isEmpty(aux->nextSibling))
					aux = aux->nextSibling;
			} else return message;
		break;
		case MINORE_UGUALE:
			if(insertInt <= valueInt){
				aux = aux->firstChild;
			} else if(aux->firstChild->nextSibling != emptyTree){
				aux = aux->firstChild->nextSibling;
				while(normalizeVariables(aux->details.condition) != cond.list[k] && !isEmpty(aux->nextSibling))
					aux = aux->nextSibling;
			} else return message;
		break;
		case MAGGIORE_UGUALE:
			if(insertInt >= valueInt){
				aux = aux->firstChild;
			} else if(aux->firstChild->nextSibling != emptyTree){
				aux = aux->firstChild->nextSibling;
				while(normalizeVariables(aux->details.condition) != cond.list[k] && !isEmpty(aux->nextSibling))
					aux = aux->nextSibling;
			} else return message;
		break;
		default:
		break;
	}
	while(var.list[k] != normalizeVariables(aux->details.variable)) k++;
	while(!isEnd(aux->firstChild)){
		return predictionAux(aux, var, cond, k);
		aux = aux->nextSibling;
	}
	//Messaggio di errore se non ho trovato nessun arco percorribile
	return aux->firstChild->details.condition;
}

//La funzione permette di effettuare una predizione chiedendo i dati tutti all'inizio.
Label decTree::prediction(decisionTree& t){
	if(isEmpty(t)) return emptyLabel;
	//Creo un array dinamico con le variabili e uno vuoto che conterrà le condizioni
	list::List var = list::createEmpty();
	var = arrayVariables(t);
	list::List cond = list::createEmpty();
	Label insert, input;
	int i = 0, pos = 0;
	//Con un ciclo chiedo le coppie (Variabile, Condizione)
	cout << "Inserire le coppie (Variabile, Condizione) nel formato 'Variabile:Condizione', con le variabili nel seguente ordine:\n";
	printVariablesToInsert(t);
	cout << "Scrivere 'STOP' per terminare l'inserimento:\n";
	do{
		cin >> insert;
		if(insert != "STOP"){
			removeBlanksAndLower(insert);
			pos = insert.find(":", 0);
			insert.erase(0, pos+1);
			list::addBack(insert, cond);
		}
	} while(insert != "STOP");
	//Richiamo la funzione ausiliaria ricorsiva
	return predictionAux(t, var, cond, 0);
}
