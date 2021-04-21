#ifndef QAP_H
#define QAP_H

#include <iostream>
#include <chrono>
#include <random>
#include <fstream>
#include <vector>
#include <algorithm>

using Matrix = std::vector<std::vector<int>>;

struct Solution{

	Solution(Matrix& dist, Matrix& flow);
	Solution(Matrix& dist, Matrix& flow, std::vector<int> aux);
	Solution();

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	
	//Testa se as soluções tem os mesmo valores de custos
	bool operator==(Solution& b);
	//Realiza atribuição de soluções
	void operator=(Solution b);
	//Verifica se b é dominada pela solução atual
	bool operator>(Solution& b);
	bool operator<(Solution& b);
	int& operator[](int index);
    //Faz uma troca no vetor solução
    void swap_solution(int& a, int& b);

    int size;
	//Alocação escolhida
	std::vector<int> solution;
	//custo
	int cost;
	//Matrix de distancia
	Matrix dist;
	//Matrix de fluxo
	Matrix flow;
};


class Qap{
	public:
		Qap();

		void read_instance(std::string instance_string);
		int get_inst_size();
		int get_obj_qtd();
		Matrix& get_dist_matrix();
		Matrix& get_flow_matrix();

		int n;
		std::vector<int> known_best_solution;
		int known_best_cost;
		Matrix dist;
		Matrix flow;
		Solution best_solution;
		std::string instance_string;
};

#endif