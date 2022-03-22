
#include <unordered_map>

#pragma once
typedef std::vector<std::string> strvec_t;

typedef  std::unordered_map < std::string, strvec_t> bypattern_t;

double minavg(std::vector<std::string> & solutions);
std::string  pattern(std::string& solutionword, std::string& guess);
std::vector< std::pair < double, std::string>> shortlist(int n, std::vector<std::string>& solutions);
double  avg(bypattern_t& distribution);
double minavg(std::vector<std::string> &solutions);
std::string bestguess(strvec_t &solutions);
void clearpatterncache();
