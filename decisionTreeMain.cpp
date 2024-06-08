#include "decisionTree.h"

const string menu = "\nMENU:\n\n1. Lettura di albero di decisione da file\n2. Inserimento di un nodo etichettato labelFiglio attaccato a un padre etichettato labelPadre\n3. Cancellazione di un nodo dall'albero\n4. Modifica di un nodo dell'albero\n5. Visualizzazione dell'albero di decisione\n6. Stampa delle variabili dell'albero di decisione\n7. Effettua predizione inserendo i valori uno alla volta\n8. Effettua predizione inserendo tutti i valori all'inizio\n0. Uscita\np. Stampa il menu\n\nFornisci la tua scelta ---> ";

const string messaggio = "\nPremere '0' per uscire, 'p' per stampare il menu o qualsiasi valore da '1' a '8' per continuare le operazioni. ---> ";

int main(){
	try{
		decTree::decisionTree t = decTree::createEmpty();
		string nome_file, index;
		char ch;
		decTree::Label l1, l2, l, condition, prediction;
		int iniz = 0;
		cout << menu;
		while(true){
			cin >> ch;
			cin.ignore();
			if(ch == '0') break;
			switch(ch){
				case '1':
				cout << "Inserire il nome del file ---> ";
				cin >> nome_file;
				if(!cin) throw runtime_error("Errore inserimento nome file\n");
				t = readFromFile(nome_file);
				cout << messaggio;
				break;
				
				case '2':
				cout << "Inserire prima labelPadre, poi labelFiglio ---> ";
				cin >> l1 >> l2;
				cout << "Inserire, se necessaria, una condizione; altrimenti 'X' ---> ";
				cin >> condition;
				removeBlanksAndLower(l1);
				removeBlanksAndLower(l2);
				if(condition != "X"){
					decTree::addElem(l1, l2, condition, t);
					if(decTree::addElem(l1, l2, condition, t) == decTree::FAIL)
						cout << "\nNodo non aggiunto perché già presente o perché non esiste il padre.\n";
				} else
					decTree::addElem(l1, l2, decTree::emptyLabel, t);
					if(decTree::addElem(l1, l2, decTree::emptyLabel, t) == decTree::FAIL)
						cout << "\nNodo non aggiunto perché già presente o perché non esiste il padre.\n";
				cout << messaggio;
				break;
				
				case '3':
				cout << "Inserire il nodo da eliminare ---> ";
				cin >> l;
				if(decTree::deleteElem(l, t) == decTree::FAIL)
					cout << "\nNodo non cancellato (motivo: l'etichetta e' quella della radice e la radice ha figli, oppure l'etichetta non e' presente nell'albero)\n";
      			else 
         			cout << "\nNodo " << l << " cancellato\n";
				cout << messaggio;
				break;
				
				case '4':
				cout << "Inserire il nodo da modificare, poi il nuovo valore ---> ";
				cin >> l1 >> l2;
				removeBlanksAndLower(l1);
				removeBlanksAndLower(l2);
				decTree::setNode(l1, l2, t);
				if(decTree::setNode(l1, l2, t) == decTree::FAIL)
					cout << "\nNodo non modificato perché non presente nell'albero.\n";
				cout << messaggio;
				break;
				
				case '5':
				printTree(t);
				cout << messaggio;
				break;
				
				case '6':
				printVariables(t);
				cout << messaggio;
				break;
				
				case '7':
				prediction = decTree::predictionBySteps(t);
				if(prediction == "=a")
					cout << "Predizione: rischio alto\n\n";
				else if(prediction == "=b")
					cout << "Predizione: rischio basso\n\n";
				else if(prediction == "IMPOSSIBILE")
					cout << "I dati non sono stati inseriti correttamente\n\n";
				else cout << prediction << endl;
				cout << messaggio;
				break;
				
				case '8':
				prediction = decTree::prediction(t);
				if(prediction == "=a")
					cout << "Predizione: rischio alto\n\n";
				else if(prediction == "=b")
					cout << "Predizione: rischio basso\n\n";
				else if(prediction == "IMPOSSIBILE")
					cout << "I dati non sono stati inseriti correttamente\n\n";
				else cout << prediction << endl;
				cout << messaggio;
				break;
				
				case 'p':
				cout << menu;
				break;
				
				default:
				cout << "\nOpzione non valida\n";
				cout << menu;
			}
		}
		return 0;
	}
	catch(runtime_error& msg)
		{cerr << "Errore a runtime: " << msg.what() << endl;}
	catch(...)
		{cerr << "Eccezione non riconosciuta\n";}
}
