#include <fstream>
#include <iostream>
#include <unordered_set>
#include "json.hpp"
#include "../optimal tree generator/recursive.h"
#include "../optimal tree generator/wordlists.h"

class PatternCounts {
public:
	std::unordered_map<std::string, std::unordered_map<std::string, int>> solution_pattern_counts;

	PatternCounts() {
		std::ifstream input("precalculated_pattern_counts.json");
		nlohmann::json j;

		input >> j;

		solution_pattern_counts = j;
		std::cout << solution_pattern_counts.size() << " solution_pattern_counts loaded\n";
	}

};

PatternCounts solpats;


//PrimaryWords pw;

strvec_t patternmatch(std::string& p, strvec_t& inputlist) {
	// patternlist is a pattern from other people's posts of results, e.g "..YGG"
	// The best patterns to use are those with fewest white squares

	strvec_t outputlist;
	for (auto w : inputlist)
		if (solpats.solution_pattern_counts[w].find(p) != solpats.solution_pattern_counts[w].end())
		outputlist.push_back(w);

	return outputlist;
}
//----------------------------------------------------------------------------

strvec_t guessmatch(std::string& g, std::string& p, strvec_t& inputlist) {
	// g is a guess, p is the pattern returned by the guess
	// e.g.'abcde', 'Y....', 'acdxx', 'Y....'
	strvec_t outputlist;
	for (auto w : inputlist)
		if (pattern(w, g) == p)
			outputlist.push_back(w);
	return outputlist;
}

//-------------------------------------------------------------------------
// split a line into words
strvec_t split(std::string & lineInput) {
	strvec_t words;
	std::string str = "";

	for (auto x : lineInput)
		if (x == ' ') {
			if (str.size()) words.push_back(str);
			str.erase();
		}
		else
			str = str + x;
	// pick up the final word in the line
	if (str.size()) words.push_back(str);
	return words;
}
//----------------------------------------------------------------------------
strvec_t repeatmatch(std::string& p, strvec_t& inputlist, int repeatcount) {
	strvec_t outputlist;
	for (auto w : inputlist)
		if (solpats.solution_pattern_counts[w].find(p) != solpats.solution_pattern_counts[w].end())
			if (solpats.solution_pattern_counts[w][p] >= repeatcount)
				outputlist.push_back(w);

	return outputlist;
}
//----------------------------------------------------------------------------
void standardisepattern(std::string& pattern) {
	std::transform(pattern.begin(), pattern.end(), pattern.begin(), ::toupper);
	std::replace(pattern.begin(), pattern.end(), 'W', '.');
	std::replace(pattern.begin(), pattern.end(), 'B', '.');

}
//----------------------------------------------------------------------------

int main() {

	//PrimaryWords pw;
	strvec_t W = primarywords.solutions;


	std::string lineInput;
	while (getline(std::cin, lineInput)) {

		if (lineInput.size() == 0)
			continue;

		// split line into words
		strvec_t words = split(lineInput);

		if (words.size() == 1) {
			// assume it is a pattern

			standardisepattern(words[0]);

			W = patternmatch(words[0], W);
			printf("Pattern %s : %zd words\n", words[0].c_str(), W.size());
		}
		if (words.size() == 2) {
			// might be "guess pattern" or "pattern repeatcount"
			if (!words[1].empty() && std::all_of(words[1].begin(), words[1].end(), ::isdigit)) {

				// it is a "pattern repeatcount"
				standardisepattern(words[0]);

				int repeatcount = atol(words[1].c_str());
				printf("Pattern %s repeated %d times :", words[0].c_str(), repeatcount);
				W = repeatmatch(words[0], W, repeatcount);
				printf(" %zd words\n", W.size());
			}
			else {
				// it is a guess-pattern pair 

				standardisepattern(words[1]);

				std::transform(words[0].begin(), words[0].end(), words[0].begin(), ::tolower);


				W = guessmatch(words[0], words[1], W);
				printf("Guess %s %s : %zd words\n", words[0].c_str(), words[1].c_str(), W.size());
			}
		}
		if (W.size() < 200) {
			for (auto w : W)
				printf("%s,", w.c_str());
			printf("\n");
		}
	}
	printf("Calculating best guess, please wait...\n");
	bestguess_t g = bestguess(W, true);
	printf("best guess: %s, average to solve: %f\n", g.bestguess.c_str(), g.average);
}