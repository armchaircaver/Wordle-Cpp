

#pragma once

//#include <unordered_map>
#include "robin_hood.h"

typedef std::vector<std::string> strvec_t;

typedef  robin_hood::unordered_flat_map < std::string, strvec_t> bypattern_t;

struct bestguess_t { double average; std::string bestguess; };


double minavg(std::vector<std::string> & solutions);

std::string  pattern(std::string& solutionword, std::string& guess);

std::vector< std::pair < double, std::string>> shortlist(int n, std::vector<std::string>& solutions);

double  avg(bypattern_t& distribution);

bestguess_t bestguess(strvec_t &solutions);

void clearpatterncache();

bypattern_t splitbypattern(std::string& guess, strvec_t& solutions);
