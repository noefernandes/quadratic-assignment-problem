#include "backtracking.h"
#include <bits/stdc++.h>
#include<chrono>
#include <algorithm>
#include <fstream>

using namespace std;
#define V 12

using namespace std::chrono;
static Qap qap_instance;


void resolve_qap(int current_cost, int current_solution_size, vector<int>& current_solution,
    vector<bool>& already_in_solution){

    if(current_solution_size == qap_instance.n){
        if(current_cost < qap_instance.best_solution.cost){
            qap_instance.best_solution.cost = current_cost;
            qap_instance.best_solution.solution = current_solution;
        }
    }

    else if(current_solution_size == 0){
        for (int i = 0; i < qap_instance.n; ++i)
        {
            current_solution[0] = i;
            already_in_solution[i] = true;

            resolve_qap(0,1,current_solution,already_in_solution);

            already_in_solution[i] = false;
        }
    }else{
        for (int i = 0; i < qap_instance.n; i++)
        {
            if(!already_in_solution[i]){
                current_solution[current_solution_size] = i;
                    already_in_solution[i] = true;

                    
                    //Calculando o acrescimo do novo custo
                    int cost_increase = 0;
                    for(int j = 0; j < current_solution_size; j++){
                        cost_increase += qap_instance.dist[j][current_solution_size]* qap_instance.flow[current_solution[j]][i]
                                        + qap_instance.dist[current_solution_size][j]*qap_instance.flow[i][current_solution[j]];
                    }

                    // Verificando o novo ramo gerado
                    resolve_qap(current_cost + cost_increase,current_solution_size+1
                        ,current_solution, already_in_solution);

                    // Removendo o elemento da solução e checando o par de qap_instance.distancia e fluxo
                    already_in_solution[i] = false;
            }
        }
    }
}

void Backtracking::solve(){
	// numero de vertices
  
	vector<bool> already_in_solution(qap_instance.n,false);
    vector<int> current_solution(qap_instance.n);

    //for (int i = 0; i < n; i++)
      //  v[i] = false;
  
    // Marcando 0-esimo vertice como visitado
    qap_instance.best_solution.cost = INT_MAX;


    //resolve_qap(qap_instance.dist, qap_instance.flow, v, 0, n, 1, 0, ans);
	//resolve_qap(qap_instance.dist,qap_instance.flow,v,0,0,n,1,0,ans);
    resolve_qap(0,0,current_solution,already_in_solution);
  	


}

void Backtracking::run_experiments(Qap qap){
    vector<double> v_tempo;
    double media = 0.0;
    qap_instance = qap;

    for (int i = 0; i < 30; i++)
    {
        auto start = high_resolution_clock::now();
        solve();
        auto end = high_resolution_clock::now();

        auto tempo = duration_cast<seconds>(end-start);
        media += tempo.count();
        v_tempo.push_back(tempo.count());
    }

    ofstream out("results.txt",ofstream::app);
    if (!out.is_open()) std::cout << "\nNão abriu o arquivo!\n";


    out << "Instância: " << qap_instance.instance_string <<"\n";
    out << "Custo ótimo já conhecido " << qap_instance.known_best_cost <<"\n";
    out << "Melhor solução já conhecida: ";
    for(int i = 0; i < qap_instance.n; i++){
        out << qap_instance.known_best_solution[i] << " ";
    }

    out << "\nTempo médio: "<< media/30 <<" segundos";
    out << "\nMenor tempo encontrado: " << *min_element(
        v_tempo.begin(),v_tempo.end()) <<" segundos";
    out << "\nMaior tempo encontrado: " << *max_element(
       v_tempo.begin(),v_tempo.end()) <<" segundos";
    out << "\nCusto encontrado: "<< qap_instance.best_solution.cost;
    out << "\nSolução encontrada: ";
    for(int i = 0; i < qap_instance.n; i++){
        out << qap_instance.best_solution.solution[i] << " ";
    }
    out <<"\n=======================================================\n";

}