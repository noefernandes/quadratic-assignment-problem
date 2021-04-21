#ifndef ITERATIVELSPR_H_
#define ITERATIVELSPR_H_
#include "qap.h"

void pertubation(Solution &solution,int k);

Solution local_search( Solution start_solution );
Solution path_relinking();
void best_neighbor(Solution &cur_solution);
std::vector<int> hamming_distance(Solution ini , Solution guide);
void move(Solution& ini, Solution& guide, std::vector<int>& Moves);
void solve();
void best_neighbor(Solution &cur_solution);
Solution iterative_local_search();
void run(Qap qap);
#endif