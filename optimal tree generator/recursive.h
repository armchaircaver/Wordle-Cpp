

#pragma once

//#include <unordered_map>
#include "robin_hood.h"
#include "wordlists.h"


typedef  robin_hood::unordered_flat_map <uint64_t, strvec_t> bypattern_t;

struct bestguess_t { double average = -1.0; std::string bestguess; };


double minavg(std::vector<std::string> & solutions);

std::string  pattern(std::string& solutionword, std::string& guess);
std::string  pattern(std::pair<std::string, int>& solution, std::string & guess);

std::vector< std::pair < double, std::string>> shortlist(int n, std::vector<std::string>& solutions);

double  avg(bypattern_t& distribution);

bestguess_t bestguess(strvec_t &solutions);

void clearpatterncache();

bypattern_t splitbypattern(std::string& guess, strvec_t& solutions);

uint64_t str2int(std::string s);

std::string int2str(uint64_t i);

int pattern2smallint(std::string s);


