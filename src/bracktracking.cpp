#include "backtracking.h"
#include <bits/stdc++.h>
using namespace std;
#define V 4

static vector<int> solution;
static vector<int> best_solution;

Matrix flow;

/*int flow[][V] = {
        { 0, 2, 3, 6 },
        { 2, 0, 5, 8 },
        { 3, 5, 0, 3 },
        { 6, 8, 3, 0 }
};*/


void backtracing_solve(Matrix graph, Matrix flow, vector<bool>& v, int currPos,
         int n, int count, int cost, int& ans){

    if (count == n && graph[currPos][0]) {
    	if(ans > cost + graph[currPos][0] * flow[currPos][0]){
    		best_solution = solution;
    	}
    	cout << cost + graph[currPos][0] * flow[currPos][0] << "\n";

    	vector<int> temp = solution;
    	for(int i = 0; i < temp.size(); i++){
    		cout << temp[i] << " ";
    	}
  
        ans = min(ans, cost + graph[currPos][0] * flow[currPos][0]);
        cout << "\nresposta: " << ans << "\n";
        return;
    }
  
    // BACKTRACKING STEP
    // Loop to traverse the adjacency list
    // of currPos node and increasing the count
    // by 1 and cost by graph[currPos][i] value
  
    for (int i = 0; i < n; i++) {
        if (!v[i] && graph[currPos][i]) {
  
            // Mark as visited
            v[i] = true;
            solution.push_back(i);
            backtracing_solve(graph, flow, v, i, n, count + 1,
                cost + graph[currPos][i] * flow[currPos][i], ans);
  
            // Mark ith node as unvisited
            v[i] = false;
            solution.pop_back();
        }
    }
};

void Backtracking::solve(Qap qap){
	// n is the number of nodes i.e. V
    //int n = 4;
    int n = qap.n;
  
    /*int graph[][V] = {
        { 0, 10, 15, 20 },
        { 10, 0, 35, 25 },
        { 15, 35, 0, 30 },
        { 20, 25, 30, 0 }
    };
    */
    Matrix graph;
    graph = qap.dist;
    flow = qap.flow;

	vector<bool> v(n);
    for (int i = 0; i < n; i++)
        v[i] = false;
  
    // Mark 0th node as visited
    v[0] = true;
    int ans = INT_MAX;

  	solution.push_back(0);


    backtracing_solve(graph, flow, v, 0, n, 1, 0, ans);
  		
    best_solution.push_back(0);
  	for(int i = 0; i < best_solution.size(); i++){
  		cout << best_solution[i] << " ";
  	}

    cout << "\n" << ans;
  
}