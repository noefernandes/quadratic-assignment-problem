#include "backtracking.h"
#include <bits/stdc++.h>
using namespace std;
#define V 12

static vector<int> solution;
static vector<int> best_solution;



int flow[][V] = {
        {0, 10,  0,  5,  1,  0,  1,  2,  2, 2, 2, 0},
		{10,  0,  1,  3,  2,  2,  2,  3, 2, 0, 2, 0},
 		{0,  1,  0, 10,  2,  0,  2,  5,  4, 5, 2, 2},
 		{5,  3, 10,  0,  1,  1,  5,  0,  0, 2, 1, 0},
 		{1,  2,  2,  1,  0,  3,  5,  5,  5, 1, 0, 3},
 		{0,  2,  0,  1,  3,  0,  2,  2,  1, 5, 0, 0},
 		{1,  2,  2,  5,  5,  2,  0,  6,  0, 1, 5, 5},
 		{2,  3,  5,  0,  5,  2,  6,  0,  5, 2, 10, 0},
 		{2,  2,  4,  0,  5,  1,  0,  5,  0, 0, 10, 5},
 		{2,  0,  5,  2,  1,  5,  1,  2,  0, 0, 0, 4},
 		{2,  2,  2,  1,  0,  0,  5, 10, 10,  0, 0, 5},
 		{0,  0,  2,  0,  3,  0,  5,  0,  5,  4,  5,  0}
};


void resolve_qap(int graph[][V], int flow[][V], vector<bool>& v, int currPos,
         int n, int count, int cost, int& ans){

    if (count == n && graph[currPos][0]) {
    	if(ans > cost + graph[currPos][0] * flow[currPos][0]){
    		best_solution = solution;
    	}
    	//cout << cost + graph[currPos][0] * flow[currPos][0] << "\n";

    	vector<int> temp = solution;
    	/*for(int i = 0; i < temp.size(); i++){
    		cout << temp[i] << " ";
    	}*/
  
        ans = min(ans, cost + graph[currPos][0] * flow[currPos][0]);
        //cout << "\nresposta: " << ans << "\n";
        return;
    }
  
    
    for (int i = 0; i < n; i++) {
        if (!v[i] && graph[currPos][i]) {
  
            // Marcando como visitado
            v[i] = true;
            solution.push_back(i);

            resolve_qap(graph, flow, v, i, n, count + 1,
                cost + graph[currPos][i] * flow[currPos][i], ans);
  
            // Marcando o i-esimo vertice como não visitado
            v[i] = false;
            solution.pop_back();
        }
    }
};

void Backtracking::solve(Qap qap){
	// numero de vertices
    int n = 12;
  
    int graph[][V] = {
        {0, 1, 2, 3, 4, 1, 2, 3, 4, 5, 2, 3},
		{1, 0, 1, 2, 3, 2, 1, 2, 3, 4, 3, 2},
		{2, 1, 0, 1, 2, 3, 2, 1, 2, 3, 4, 3},
		{3, 2, 1, 0, 1, 4, 3, 2, 1, 2, 5, 4},
		{4, 3, 2, 1, 0, 5, 4, 3, 2, 1, 6, 5},
		{1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 1, 2},
		{2, 1, 2, 3, 4, 1, 0, 1, 2, 3, 2, 1},
		{3, 2, 1, 2, 3, 2, 1, 0, 1, 2, 3, 2},
		{4, 3, 2, 1, 2, 3, 2, 1, 0, 1, 4, 3},
		{5, 4, 3, 2, 1, 4, 3, 2, 1, 0, 5, 4},
		{2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 0, 1},
		{3, 2, 3, 4, 5, 2, 1, 2, 3, 4, 1, 0}
    };
    
	vector<bool> v(n);
    for (int i = 0; i < n; i++)
        v[i] = false;
  
    // Marcando 0-esimo vertice como visitado
    v[0] = true;
    int ans = INT_MAX;

  	solution.push_back(0);

    resolve_qap(graph, flow, v, 0, n, 1, 0, ans);
	
    best_solution.push_back(0);
  	
  	cout << "reposta: ";
  	for(int i = 0; i < n; i++){
  		cout << best_solution[i] << " ";
  	}
    
    cout << "\ncusto: " << ans << "\n";


}