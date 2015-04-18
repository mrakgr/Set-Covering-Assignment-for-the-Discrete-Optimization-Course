// The greedy algorithm repeatedly selects the set the best cost/size ratio and removes the elements in the result from all
// the existing sets. Good enough for 7, 3, 7, 7, 7, 7.

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <sstream>
#include <fstream>

std::string data_path = "C:\\Discrete Optimization Class\\setcover\\";
std::string data_file = "";

int num_set = 0, num_covers = 0;

using namespace std;

class Cover{
public:
	char tag;
	float set_cost;
	int index;
	vector<int> set;
	Cover(){ tag = 0; }
	Cover(float t) { tag = 0;  set_cost = t; }
	int set_size_int(){
		return set.end() - set.begin();
	}
	float set_size(){
		float temp = set.end() - set.begin();
		if (temp == 0) return float(1e-6);
		return temp;
	}
};


Cover Union(Cover &A, Cover &B){
	
	Cover new_cover; // Warning: The new cover's index is uninitialized here.
	new_cover.set_cost = A.set_cost + B.set_cost;
	new_cover.set.resize(A.set.end() - A.set.begin() + B.set.end() - B.set.begin());
	
	vector<int>::iterator it;
	it = set_union(A.set.begin(), A.set.end(), B.set.begin(), B.set.end(), new_cover.set.begin());

	new_cover.set.resize(it - new_cover.set.begin());
	
	return new_cover;
}

Cover Difference(Cover &A, Cover &B){

	Cover new_cover = A;
	
	vector<int>::iterator it;
	it = set_difference(A.set.begin(), A.set.end(), B.set.begin(), B.set.end(), new_cover.set.begin());

	new_cover.set.resize(it - new_cover.set.begin());

	return new_cover;
}


Cover greedyFindBest(vector<Cover> &sets, Cover &B){
	sets[0] = Difference(sets[0], B);
	float top_result = sets[0].set_cost / sets[0].set_size();
	Cover top_cover = sets[0];
	for (int i = 1; i < num_covers; i++){
		sets[i] = Difference(sets[i], B);
		float cost = sets[i].set_cost / sets[i].set_size();
		if (top_result > cost){
			top_cover = sets[i];
			top_result = cost;
		}
	}
	return top_cover;
}

int main(int argc, char* argv[])
{
	if (argc > 1){
		data_file = argv[1];
	}

	vector<Cover> sets;

	{
		fstream file(data_path + data_file, ios::in);
		
		string line;
		getline(file, line);
		stringstream t(line);
		t >> num_set >> num_covers;

		sets = vector<Cover>(num_covers);
		
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
	
	Cover result(0);
		
	for (int i = 0; result.set_size_int() < num_set; i += 1){
		//copy(sets[i].set.begin(), sets[i].set.end(), ostream_iterator<int>(cout, " ")); cout << endl;
		Cover best = greedyFindBest(sets, result);
		result = Union(result, best);
		//copy(best.set.begin(), best.set.end(), ostream_iterator<int>(cout, " ")); cout << endl;
		//cout << best.index << endl;
		sets[best.index].tag = 1;
	}

	//copy(result.set.begin(), result.set.end(), ostream_iterator<int>(cout, " ")); cout << endl;

	cout << result.set_cost << " 0" << endl;
	for (int i = 0; i < num_covers; i++) cout << int(sets[i].tag) << " ";
	cout << endl;

	return 0;
}

