

#pragma once

//#include <unordered_map>
#include "robin_hood.h"
#include "wordlists.h"


typedef  robin_hood::unordered_flat_map <uint64_t, strvec_t> bypattern_t;

struct bestguess_t { double average = -1.0; std::string bestguess; };

struct stats_t {
	uint64_t patterncache_hits = 0;
	uint64_t patterncache_misses = 0;
	uint64_t smallest_cutoff_hits=0;
	uint64_t cutoff_hits=0;
	uint64_t bestguesscache_hits=0;
};

stats_t export_stats();

std::string  pattern(std::string& solutionword, std::string& guess);

std::vector< std::pair < double, std::string>> shortlist(int n, std::vector<std::string>& solutions);

bestguess_t bestguess(strvec_t& solutions, bool printProgress = false);

void clearpatterncache();

bypattern_t splitbypattern(std::string& guess, strvec_t& solutions);

uint64_t str2int(std::string s);

std::string int2str(uint64_t i);



