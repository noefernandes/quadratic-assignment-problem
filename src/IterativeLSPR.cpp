#include "IterativeLSPR.h"
#include <bits/stdc++.h>
#include<chrono>
#include <algorithm>
#include <fstream>

using namespace std;
#define V 12

using namespace std::chrono;
static Qap qap_instance;
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();


void pertubation(Solution &solution,int k){	

	std::mt19937 mt(seed);
	std::uniform_int_distribution<int> linear_dis(0,solution.size-k);
	int init = linear_dis(mt);

	std::random_shuffle(solution.solution.begin()+init, solution.solution.begin()+init+k);
	int cost = 0;

	for(int i = 0; i < solution.size; i++){
		for(int j = 0; j < solution.size; j++){
			cost += qap_instance.flow[solution[i]-1][solution[j]-1] * qap_instance.dist[i][j];

		}
	}

	
	solution.cost = cost;
}

double calcDistanciamento ( )
{
	return ( fabs(qap_instance.best_solution.cost-qap_instance.known_best_cost)/qap_instance.known_best_cost );
}

void best_neighbor(Solution &cur_solution) //passar solucao por copia eh muito custoso... passar por referencia const; nao usar cur_solution para guardar o retorno da funcao...
{

	//Apenas para repassar o vetor solução pro vizinho
	vector<int> aux = cur_solution.solution;

	Solution neighbor(qap_instance.dist,qap_instance.flow,cur_solution.solution);
	neighbor.swap_solution(neighbor[0],neighbor[1]);

	
	neighbor.swap_solution(neighbor[0],neighbor[1]);	

	for(int i = 0; i < qap_instance.n-1; i++)
	{
		for (int j = i+1; j < qap_instance.n; ++j)
		{
			neighbor.swap_solution(neighbor[i],neighbor[j]);

			if(neighbor < cur_solution)
			{
				cur_solution = neighbor;
			}
			neighbor.swap_solution(neighbor[i],neighbor[j]);
		}
	}
}

int evaluate_move(int a, int b){
	return qap_instance.flow[a-1][b-1] * qap_instance.dist[a-1][b-1];
}

void constructive_phase(){
	std::vector<int> sol;
	std::vector<int> added;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(1, qap_instance.n);
 	int chosen = distrib(gen);
 	
 	added.push_back(chosen);
 	sol.push_back(chosen);

 	
 	for (int i = 2; i <= qap_instance.n; i++) {
			
		std::vector<int> lrc;
			
		//Preenchendo a lrc
		for(int cont = 0; cont < qap_instance.n; cont++) {
			
			int cand_cost = 99999999;
			int candidate = -1;
			
			// Encontrando melhor candidado (primeiro melhor, segundo melhor...)
			for (int possible_cand = 1; possible_cand <= qap_instance.n; possible_cand++) {
				if (possible_cand != added[i - 2]) {
					int possible_cand_cost = evaluate_move(added[i - 2], possible_cand);

					if (std::find(added.begin(), added.end(), possible_cand) == added.end() &&
						std::find(lrc.begin(), lrc.end(), possible_cand) == lrc.end() &&
						possible_cand_cost < cand_cost) {
						
						candidate = possible_cand;
						cand_cost = possible_cand_cost;
					}
				}
			}
			
			if (candidate != -1) {
				lrc.push_back(candidate);
			}
		}

		
		// Adicionando candidato aleatorio na solucao
		if (lrc.size() > 0) {
			std::uniform_real_distribution<double> dist(0.0, 1.0);
	    	std::mt19937 rng; 
	    	rng.seed(std::random_device{}());
	    	double per = dist(rng);

			int random = lrc[(int) (per * lrc.size())];
			sol.push_back(random);
			added.push_back(random);
		} else {
			std::cout << "LRC vazia.\n";
		}
	}


	Solution solution(qap_instance.dist,qap_instance.flow, sol);
	qap_instance.best_solution = solution;
}


Solution local_search(Solution cur_solution)
{
	
 	Solution best_solution(qap_instance.dist,qap_instance.flow);


  	bool improve = true;

 	Solution candidate(qap_instance.flow,qap_instance.dist,cur_solution.solution);
 	best_neighbor(candidate);



 	while(improve)
 	{
 		if (candidate < best_solution)
 		{
			best_solution = candidate;

 			cur_solution = candidate;

		}else if(candidate < cur_solution && candidate > best_solution)
 		{

 			cur_solution = candidate;
 		}
 		else
 		{

 			improve = false;
 		}
		candidate = cur_solution;
 		best_neighbor(candidate);
	}


	return best_solution;


}

//Calcula a distância simétrica, logo os movimentos serão guardados
std::vector<int> hamming_distance(Solution ini , Solution guide){

	std::vector<int> Moves;

	for (int i = 0 ; i < qap_instance.n ; ++i)
	{
		if( ini[i] != guide[i])
		{
			Moves.push_back(i);
		}
	}

	return Moves;
}
//Aplica o movimento a partir da solução inicial e da guia
//E pega o primeiro movimento da lista 
void move(Solution& ini, Solution& guide, std::vector<int>& Moves){
	for (int i = Moves[0]; i < qap_instance.n; ++i)
 			{
 				if(ini[Moves[0]] == guide[i]){
 					ini.swap_solution(ini[Moves[0]],guide[i]);
 				}
 			}
}


Solution path_relinking(){
	
	Solution start_solution(qap_instance.dist,qap_instance.flow);

 	Solution last_solution(qap_instance.dist,qap_instance.flow);

 	//Assumindo que o melhor por enquanto é a primeira solução
 	Solution best_solution(qap_instance.dist,qap_instance.flow);

 	// Candidato que ira representar o vizinho da solução
  	Solution candidate(qap_instance.dist,qap_instance.flow,start_solution.solution);

 	for (int i = 0; i < qap_instance.n; ++i)
 	{
 		if(candidate[i] != last_solution[i])
 		{
 			//Varrendo o vetor a partir do valor diferente entre a start e last, caso ache, ocorre a troca
 			for (int j = i; j < qap_instance.n; ++j)
 			{
 				if(candidate[j] == last_solution[i]){
 					candidate.swap_solution(candidate[i],candidate[j]);
 				}
 			}
 			if( best_solution > candidate){
 				best_solution = candidate;
 			}
 		}
 	}

 	// Ultimo dos vizinho sendo testado
 	if (best_solution > last_solution)
 	{
 		best_solution = last_solution;
 	}

 	return best_solution;
}

Solution iterative_local_search()
{

 	Solution start_solution(qap_instance.dist,qap_instance.flow);
	Solution ls_solution(qap_instance.dist,
		qap_instance.flow,local_search(start_solution).solution); 

	Solution best_solution(qap_instance.dist,qap_instance.flow, 
		ls_solution.solution); 
	
	int k = 1;


	Solution candidate(qap_instance.dist,qap_instance.flow
		, ls_solution.solution); 
	
	Solution ls_candidate(qap_instance.dist,qap_instance.flow
		, candidate.solution); 


	while(k < qap_instance.n)
	{
		
		if (ls_candidate < best_solution)
		{
			best_solution = ls_candidate;

		} else{

			k++;

		}
		pertubation(candidate,k);

		ls_candidate = local_search(candidate);
			//cout << ls_candidate.cost;


	}

	return best_solution;
	
}
// Iterative LS path relinking mixed
void solve(){
	Solution start_solution = iterative_local_search();
	Solution last_solution = iterative_local_search();




 	Solution best_solution(qap_instance.dist,qap_instance.flow);
 	//Variavel auxiliar para alterar o sentido do caminho
	bool turn = true;
	std::vector<int> Moves = hamming_distance(start_solution,last_solution); 

	//Guarda a melhor solução
	if(start_solution < last_solution)
	{
		best_solution = start_solution;
	
	}else{
		
		best_solution = last_solution;
	
	}

	//Será feito até que acabe o número de movimentos,ou seja, quando start e last forem iguais
	while(Moves.size() > 0){

		if(turn){
			move(start_solution,last_solution,Moves);

			//Guarda a melhor solução até então
 			if( best_solution > start_solution){
 				best_solution = start_solution;
 			}

 			Moves.erase(Moves.begin());
 			turn = false;

		}else{
			move(last_solution,start_solution,Moves);

			if( best_solution > last_solution){
 				best_solution = last_solution;
 			}

 			Moves.erase(Moves.begin());
 			turn = true;

		}

	}


	qap_instance.best_solution = best_solution;

}


void run(Qap qap){
    vector<double> v_tempo;
    double media = 0.0;
    vector<double> v_tempo2;
    double media2 = 0.0;

    qap_instance = qap;


    for (int i = 0; i < 30; i++)
    {
        auto start = high_resolution_clock::now();
        solve();
        auto end = high_resolution_clock::now();

        auto tempo = duration_cast<microseconds>(end-start);
        media += tempo.count();
        v_tempo.push_back(tempo.count());
    }

    ofstream out("results.txt",ofstream::app);
    if (!out.is_open()) std::cout << "\nNão abriu o arquivo!\n";

    out << "Busca local iterativa + path-relinking\n";

    out << "Instância: " << qap_instance.instance_string <<"\n";
    out << "Custo ótimo já conhecido " << qap_instance.known_best_cost <<"\n";
    out << "Melhor solução já conhecida: ";
    for(int i = 0; i < qap_instance.n; i++){
        out << qap_instance.known_best_solution[i] << " ";
    }

    out << "\nTempo médio: "<< media/30 <<"0."<<" segundos";
    out << "\nMenor tempo encontrado: " << "0."<< *min_element(
        v_tempo.begin(),v_tempo.end()) <<" segundos";
    out << "\nMaior tempo encontrado: " << "0."<<*max_element(
       v_tempo.begin(),v_tempo.end()) <<" segundos";
    out << "\nCusto encontrado: "<< qap_instance.best_solution.cost;
   out << "\nSolução encontrada ";
    for(int i = 0; i < qap_instance.n; i++){
        out << qap_instance.best_solution[i] << " ";
    }

    out << "\nDistância entre a melhor solução conhecida e a encontrada: ";
    out << calcDistanciamento();
    out <<"\n=======================================================\n";

    for (int i = 0; i < 30; i++)
    {
        auto start = high_resolution_clock::now();
        constructive_phase();
        auto end = high_resolution_clock::now();

        auto tempo = duration_cast<microseconds>(end-start);
        media2 += tempo.count();
        v_tempo2.push_back(tempo.count());
    }

    out << "Fase construtiva do grasp\n";

    out << "Instância: " << qap_instance.instance_string <<"\n";
    out << "Custo ótimo já conhecido " << qap_instance.known_best_cost <<"\n";
    out << "Melhor solução já conhecida: ";
    for(int i = 0; i < qap_instance.n; i++){
        out << qap_instance.known_best_solution[i] << " ";
    }

    out << "\nTempo médio: "<< media2/30 <<"0."<<" segundos";
    out << "\nMenor tempo encontrado: " << "0."<< *min_element(
        v_tempo2.begin(),v_tempo2.end()) <<" segundos";
    out << "\nMaior tempo encontrado: " << "0."<<*max_element(
       v_tempo2.begin(),v_tempo2.end()) <<" segundos";
    out << "\nCusto encontrado: "<< qap_instance.best_solution.cost;
   out << "\nSolução encontrada ";
    for(int i = 0; i < qap_instance.n; i++){
        out << qap_instance.best_solution[i] << " ";
    }

    out << "\nDistância entre a melhor solução conhecida e a encontrada: ";
    out << calcDistanciamento();
    out <<"\n=======================================================\n";

}