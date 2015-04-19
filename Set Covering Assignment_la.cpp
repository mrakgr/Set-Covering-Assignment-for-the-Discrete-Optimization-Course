/*
The set covering assignment using the Late Acceptance Metaheuristic.
Works worse that the greedy approximation algorithm. Uses random flips as moves.
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <boost\dynamic_bitset.hpp>

#include <sstream>
#include <fstream>

std::string data_path = "C:\\Discrete Optimization Class\\setcover\\";
std::string data_file = "";

int num_set = 0, num_covers = 0;

using namespace std;

class Cover{
private:
	void __init(){
	}
public:
	float set_cost;
	int index;
	vector<int> set;
	Cover(){ __init(); }
	Cover(float t) { __init();  set_cost = t; }
	int set_size_int(){
		return set.end() - set.begin();
	}
};

vector<Cover> sets;

class LA{
private:
	float calculateCost(){
		float cost = 0;
		auto idx = solution.find_first();
		while (idx != solution.npos){
			cost += sets[idx].set_cost;
			idx = solution.find_next(idx);
		}
		auto elements_not_covered = num_set - elements_covered_total;
		cost += elements_not_covered*penalty;
		return cost;
	}
	const int penalty = 1; // Problem specific. As a rule of thumb the whole empty solution should have a cost 3x that of the aproximation algorithm.
public:
	const int acceptance_list_size = 30000;
	unsigned int acceptance_list_index;
	vector<float> acceptance_list;

	float best_solution_cost;
	boost::dynamic_bitset<> best_solution;
	float solution_cost;
	boost::dynamic_bitset<> solution;
	int elements_covered_total;
	vector<int> elements_covered;
	LA(){
		acceptance_list.resize(acceptance_list_size, num_set*penalty);
		acceptance_list_index = 0;

		best_solution.resize(num_covers);
		solution.resize(num_covers);
		elements_covered.resize(num_set);
		elements_covered_total = 0;
		solution_cost = calculateCost();
		best_solution_cost = solution_cost;
	}

	void AddSet(int idx){
		for (int i = 0; i < sets[idx].set_size_int(); i++){
			auto t = sets[idx].set[i];
			elements_covered[t]++;
			if (elements_covered[t] == 1) {
				elements_covered_total++;
				solution_cost -= penalty;
			}
		}
		solution_cost += sets[idx].set_cost;
	}
	void RemoveSet(int idx){
		for (int i = 0; i < sets[idx].set_size_int(); i++){
			auto t = sets[idx].set[i];
			elements_covered[t]--;
			if (elements_covered[t] == 0) {
				elements_covered_total--;
				solution_cost += penalty;
			}
		}
		solution_cost -= sets[idx].set_cost;
	}
	void FlipSet(int idx){
		solution.flip(idx);
		bool val = solution[idx];
		if (val == 1) AddSet(idx);
		else RemoveSet(idx);
		if (solution_cost < best_solution_cost){
			best_solution_cost = solution_cost;
			best_solution = solution;
			cout << best_solution_cost << endl;
		}
	}
	float getAcceptanceFirst(){
		return acceptance_list[acceptance_list_index % acceptance_list_size];
	}
	void setAcceptanceFirst(float cost){
		acceptance_list[acceptance_list_index % acceptance_list_size] = cost;
		acceptance_list_index++;
	}
};



int main(int argc, char* argv[])
{
	if (argc > 1){
		data_file = argv[1];
	}

	{
		fstream file(data_path + data_file, ios::in);
		
		string line;
		getline(file, line);
		stringstream t(line);
		t >> num_set >> num_covers;

		sets.resize(num_covers);
		
		for (int i = 0; getline(file, line); i++){
			stringstream s(line);
			s >> sets[i].set_cost;
			int number;
			for (int q = 0; s >> number; q++){
				sets[i].set.push_back(number);
			}
			sort(sets[i].set.begin(), sets[i].set.end());
			sets[i].index = i;
		}
	}
	
	LA late;
	auto prev_sol = late.solution_cost;

	mt19937 gen;
	uniform_int_distribution<int> dist(0, num_covers - 1);
	for (int i = 0; i < 100000000; i++){
		auto t = dist(gen);
		late.FlipSet(t);
		auto sol = late.solution_cost;

		//cout << late.best_solution_cost << endl;

		if (sol < prev_sol || sol < late.getAcceptanceFirst()){
			//cout << sol << endl;
			late.setAcceptanceFirst(sol);
			prev_sol = sol;
		}
		else{
			late.FlipSet(t);
		}
	}

	//cout << late.best_solution_cost << " 0" << endl;
	//cout << late.best_solution << endl;

	return 0;
}

