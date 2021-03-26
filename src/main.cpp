#include <iostream>
#include "backtracking.h"

int main(int argc, char *argv[]){
	Qap qap;
	Backtracking backtracking;
	
	std::string instance = argv[1];
	std::string solution = argv[2];
	qap.read_instance("data/" + instance, "data/" + solution);

	//qap.grasp();
	backtracking.solve(qap);

	return 0;
}