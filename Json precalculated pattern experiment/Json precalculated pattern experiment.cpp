#include "json.hpp"
#include <fstream>
#include <iostream>
#include <unordered_set>

class Patterns {
public:
	std::unordered_map<std::string, std::unordered_set<std::string>> solution_patterns;

	Patterns() {
		std::ifstream input("precalculated_patterns.json");
		nlohmann::json j;

		input >> j;

		solution_patterns = j;
		std::cout << solution_patterns.size() << " solution_patterns loaded\n";
	}

}; 

int main() {
	//std::ifstream input("precalculated_patterns.json");
	//nlohmann::json j;

	//input >> j;
	
	//std::unordered_map<std::string, std::unordered_set<std::string>> m = j; 

	Patterns sp;
	std::unordered_map<std::string, std::unordered_set<std::string>> m = sp.solution_patterns;

	std::cout << m.size() << " items in map\n";

	int count = 0;
	for (auto x : m) {
		std::cout << x.first << " ";
		std::cout << x.second.size() << "\n";
		for (auto y : x.second)
			std::cout << y << ",";
		std::cout << "\n";

		count++;
		if (count == 10) break;
	}
}