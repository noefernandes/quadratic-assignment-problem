#include "qap.h"

//Tamanho da instancia
int n;


Solution::Solution(){}

Solution::Solution(Matrix& dist, Matrix& flow){
	this->dist = dist;
	this->flow = flow;
	this->size = dist.size();
	for(int i(0); i < this->size; i++){
		this->solution.push_back(i + 1);
	}

	int cost = 0;

	shuffle(this->solution.begin(), this->solution.end(), std::default_random_engine(seed));

	for(int i = 0; i < this->size; i++){
		for(int j = 0; j < this->size; j++){
			cost += this->flow[this->solution[i]-1][this->solution[j]-1] * this->dist[i][j];
		}
	}
	
	this->cost = cost;
}

Solution::Solution(Matrix& dist, Matrix& flow, std::vector<int> aux){
	this->dist = dist;
	this->flow = flow;
	this->solution.reserve(solution.size());
	this->solution = aux;

	this->size = solution.size();
	
	int cost = 0;



	for(int i = 0; i < this->size; i++){
		for(int j = 0; j < this->size; j++){
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

	for(int i = 0; i < this->size; i++){
		for(int j = 0; j < this->size; j++){
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

void Qap::read_instance(std::string instance_string){
	std::ifstream file;
	std::ifstream sol;
	this->instance_string = instance_string;
	std::string instance_name = "data/" + instance_string + ".dat"; 
	std::string solution_name = "data/" + instance_string + ".sln";


	file.open(instance_name.c_str());

	file >> n;
	this->n = n;
	
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

		sol >> known_best_cost;

		known_best_solution.resize(n);

		for(int i = 0 ;i < n ; ++i){
			sol >> known_best_solution[i];

		}

	}
}




