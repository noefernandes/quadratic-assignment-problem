#include <iostream>
#include "backtracking.h"
using namespace std;

int main(){
	int choice;
	cout << "Escolha a forma de execução:\n";
	cout << "1 - Executar uma por classe de instância(rou12, had12, esc16a e bur26a)\n";
	cout << "2 - Executar uma das instâncias na pasta data\n";
	cin >> choice;

	switch(choice){
		case 1:{
			std::vector<std::string> instances {"rou12", "had12", "esc16a","bur26a"};
			
			for (int i = 0; i < instances.size(); i++){
				Qap qap;
				Backtracking backtracking;
				cout<< instances[i];
				qap.read_instance(instances[i]);

				//qap.grasp();
				backtracking.run_experiments(qap);	

			
			}
		}
			break;

		case 2:{
			cout << "Digite o nome da instância desejada: ";
			string instance_string;
			cin >> instance_string;
			Qap qap;
			Backtracking backtracking;
	
			qap.read_instance(instance_string);

			//qap.grasp();
			backtracking.run_experiments(qap);
		}
			break;
		default:
			cout << "Número inválido!";
		
	}



	return 0;
}