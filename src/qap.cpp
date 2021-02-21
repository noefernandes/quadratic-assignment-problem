#include "qap.h"

//Tamanho da instancia
int n;
std::vector<int> opt;
int opt_cost;

Solution::Solution(){}

Solution::Solution(Matrix& dist, Matrix& flow){
	this->dist = dist;
	this->flow = flow;

	for(int i(0); i < n; i++){
		this->solution.push_back(i + 1);
	}

	int cost = 0;

	shuffle(this->solution.begin(), this->solution.end(), std::default_random_engine(seed));

	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			cost += this->flow[this->solution[i]-1][this->solution[j]-1] * this->dist[i][j];
		}
	}
	
	this->cost = cost;
}

Solution::Solution(Matrix& dist, Matrix& flow, std::vector<int> aux){
	this->dist = dist;
	this->flow = flow;
	this->solution.reserve(n);
	this->solution = aux;
	
	int cost = 0;

	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			cost += this->flow[this->solution[i]-1][this->solution[j]-1] * this->dist[i][j];
		}
	}
	
	this->cost = cost;
}

void Solution::operator=(Solution b){
	this->solution = b.solution;
	this->cost = b.cost;
	this->dist = b.dist;
	this->flow = b.flow;
}

//Testando se this domina b
bool Solution::operator>(Solution& b){
	
	return this->cost > b.cost; 
}

//Testando se this domina b
bool Solution::operator<(Solution& b){
	
	return this->cost < b.cost; 
}

bool Solution::operator==(Solution& b){
	return this->cost == b.cost;
}

int& Solution::operator[](int index){
	return this->solution[index];
}

void Solution::swap_solution(int& a, int& b){
	int temp = a;
	a = b;
	b = temp;

	int cost = 0;

	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			cost += this->flow[this->solution[i]-1][this->solution[j]-1] * this->dist[i][j];
		}
	}
	
	this->cost = cost;
}


/***********************************************************************************************/

Qap::Qap(){}

int Qap::get_inst_size(){ return n; }
int Qap::get_obj_qtd(){ return n; }
Matrix& Qap::get_dist_matrix(){ return dist; }
Matrix& Qap::get_flow_matrix(){ return flow; }

void Qap::read_instance(std::string instance_name, std::string solution_name){
	std::ifstream file;
	std::ifstream sol;

	file.open(instance_name.c_str());

	file >> n;
	
	dist.resize(n);
	flow.resize(n);

	
  	if(file.is_open()){
    	for(int i = 0; i < n; ++i){
			dist[i].resize(n);
			flow[i].resize(n);
		}

		for(int i = 0; i < n; ++i){
			for(int j = 0; j < n; ++j){
				file >> dist[i][j];
			}
		}

		for(int i = 0; i < n; ++i){
			for(int j = 0; j < n; ++j){
				file >> flow[i][j];
			}
		}

		sol.open(solution_name.c_str());
		sol >> n;

		sol >> opt_cost;

		opt.resize(n);

		for(int i = 0 ;i < n ; ++i){
			sol >> opt[i];

		}

	}
}

// Utilização da estrategia 2swap para permutar os vetores e gerar a vizinhança
void Qap::best_neighbor(Solution &curr_solution) //passar solucao por copia eh muito custoso... passar por referencia const; nao usar cur_solution para guardar o retorno da funcao...
{

	//Apenas para repassar o vetor solução pro vizinho
	std::vector<int> aux = curr_solution.solution;

	Solution neighbor(dist, flow, aux);
	neighbor.swap_solution(neighbor[0], neighbor[1]);

	
	int best_cost = neighbor.cost; 
	neighbor.swap_solution(neighbor[0], neighbor[1]);	

	for(int i = 0; i < n-1; i++){
		for (int j = i+1; j < n; ++j){
			
			neighbor.swap_solution(neighbor[i],neighbor[j]);

			if(neighbor.cost < best_cost){
				curr_solution = neighbor;
				best_cost = neighbor.cost;
			}

			neighbor.swap_solution(neighbor[i],neighbor[j]);
		}
	}
}

Solution Qap::local_search(Solution& curr_solution)
{
	
 	Solution best_solution(dist, flow, opt);
 	Solution candidate(dist, flow, opt);
 	candidate = curr_solution;
 	best_neighbor(candidate);

 	bool improve = true;

 	while(improve){
 		if (candidate < best_solution){

			best_solution = candidate;
 			curr_solution = candidate;

		}else if(candidate < curr_solution && candidate > best_solution){

 			curr_solution = candidate;

 		}else{

 			improve = false;
 		}

		candidate = curr_solution;
 		best_neighbor(candidate);
	}

	return best_solution;
}

int Qap::evaluate_move(int a, int b){
	return this->flow[a-1][b-1] * this->dist[a-1][b-1];
}


Solution Qap::constructive_phase(){
	std::vector<int> sol;
	std::vector<int> added;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(1, n);
 	int chosen = distrib(gen);
 	
 	added.push_back(chosen);
 	sol.push_back(chosen);

 	
 	for (int i = 2; i <= n; i++) {
			
		std::vector<int> lrc;
			
		//Preenchendo a lrc
		for(int cont = 0; cont < n; cont++) {
			
			int cand_cost = 99999999;
			int candidate = -1;
			
			// Encontrando melhor candidado (primeiro melhor, segundo melhor...)
			for (int possible_cand = 1; possible_cand <= n; possible_cand++) {
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


	Solution solution(dist, flow, sol);

	return solution;
}

Solution Qap::grasp(){
	Solution solution;
	int best_cost = 9999999;

	int it = 0;
	while(it < 100){
		
		Solution curr = constructive_phase();
		local_search(curr);

		if(curr.cost < best_cost){
			best_cost = curr.cost;
			solution = curr;
		}
		
		it++;
	}

	std::cout << solution.cost << "\n";

	return solution;
}