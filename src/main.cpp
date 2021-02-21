#include <iostream>
#include "qap.h"

int main(int argc, char *argv[]){
	Qap qap;
	
	std::string instance = argv[1];
	std::string solution = argv[2];
	qap.read_instance("data/" + instance, "data/" + solution);

	qap.grasp();
}