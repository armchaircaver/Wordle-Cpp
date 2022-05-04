#include "wordlists.h"
#include <algorithm>
#include <stdexcept>
#include "recursive.h"
#include <iostream>

PrimaryWords primarywords;

class Estimates {
    // estimate[n] is an estimate of the number of guesses after the best first guess 
    // for a set of solution words of length n
public:
    double estimates[1000];

    Estimates() {
        estimates[0] = 0.0;
        estimates[1] = 0.00;
        estimates[2] = 1.00;
        estimates[3] = 2.55;

        for (int i = 4; i < 1000; i++)
            estimates[i] = powl((double)i, 1.1) - 1.0;
    }
};
//----------------------------------------------------------------------------------------------------

Estimates e;

robin_hood::unordered_flat_map< std::string,  std::string> pattern_cache;


stats_t stats;

stats_t export_stats() {
    return stats;
}


void clearpatterncache() {
    pattern_cache.clear();
 }


bypattern_t splitbypattern(std::string& guess, strvec_t& solutions) {
    bypattern_t solsbypattern;
    for (auto sol : solutions) {
        std::string p = pattern(sol, guess);
        solsbypattern[str2int(p)].push_back(sol);
    }
    return solsbypattern;

}
//------------------------------------------------------------------------------------------------------------------------------------
std::string concat(strvec_t v) {
    std::string res;
    for (auto s : v)
        res += s + ",";
    return res;
}

//------------------------------------------------------------------------------------------------------------------------------------
uint64_t str2int(std::string s) {
    uint64_t i = 0ull;
    memcpy(&i, s.c_str(), 5);
    return i;
}
std::string int2str(uint64_t i) {
    char c[6] = { '?','?','?','?','?', 0 };
    memcpy(&c, &i, 5);
    return std::string(c);
}
//------------------------------------------------------------------------------------------------------------------------------------
int pattern2smallint(std::string s) {
    int i = 0;
    int pos = 0;
    for (auto c : s) {
        i |= (c & 3) << pos;
        pos += 2;
    }
    return i;
}

std::string  pattern(std::string& solutionword, std::string& guess) {

    auto pc_find = pattern_cache.find(guess + solutionword);
    if (pc_find != pattern_cache.end()) {
        stats.patterncache_hits++;
        return pc_find->second;
    }

    std::string list_solutionword = solutionword;
    std::string p = ".....";

    // find greens first, and remove the letter from the solution
    // so that we don't get false yellows for a repeated letter in the guess word
    for (int i = 0; i < 5; i++)
        if (solutionword[i] == guess[i]) {
            p[i] = 'G';
            list_solutionword[i] = '.';
        }

    // find yellows, removing a matched letter from the solution
    // so that we don't get further false yellows for a repeated letter in the guess word
    for (int i = 0; i < 5; i++) {
        if (p[i] == 'G')
            continue;

        // if guess[i] is in list_solutionword - this is a bottleneck
        //if (list_solutionword.find(guess[i]) != std::string::npos)
        if (strchr(list_solutionword.c_str(), guess[i]))
        {
            p[i] = 'Y';
            list_solutionword[list_solutionword.find(guess[i])] = '.';
        }
    }
    stats.patterncache_misses++;
    pattern_cache[guess+solutionword] = p;
    return p;
}

std::vector< std::pair < double, std::string>> shortlist(int n, std::vector<std::string> & inputsols) {

    // returns a list of pairs (squaresum, guess)
    // The list returned is the list for guesses with the smallest squaresum


    std::vector< std::pair < double, std::string>> slist;
    /*
    if (!powersinit) {
        powers[0] = 0.0;
        for (int i = 1; i < 1000; i++)
            powers[i] = powl((double)i, 1.5);
        powersinit = 1;
    }
    */

    bypattern_t solsbypattern;

    //std::unordered_map < std::string, int> totalsbypattern;
    robin_hood::unordered_flat_map< int, int> totalsbypattern;

    //int totalsbypattern[1024] = { 0 };
    std::string p;
    for (auto guess : primarywords.alloptions) {

        double squaresum = 0.0;
        solsbypattern.clear();
        totalsbypattern.clear();
        //memset(totalsbypattern, 0, sizeof(totalsbypattern));

        for (auto sol : inputsols) {
            p = pattern(sol, guess);
            //int x = ++totalsbypattern[str2int(p)];
           int x = ++totalsbypattern[pattern2smallint(p)];

            // accumulate the sum of squares of totals for each pattern
            // incrementing the sum of squares : x * *2 - (x - 1) * *2 = 2 * x - 1
            // squaresum += 2 * x - 1

            // however, this seems to produce a better shortlist
            squaresum += e.estimates[x]-e.estimates[x-1];


            if (slist.size() == n && squaresum > slist.back().first)
                break;
        }
        // we have found a guess that distributes each solution in a different pattern
        // and one of the solutions is a guess 
        if (squaresum == inputsols.size() && std::find(inputsols.begin(), inputsols.end(), guess) != inputsols.end()) {
            slist.clear();
            slist.push_back(std::make_pair(squaresum, guess));
            return slist;
        }

        if (slist.size() < n or squaresum < slist.back().first) {
            slist.push_back(std::make_pair(squaresum, guess));
            std::sort(slist.begin(), slist.end());
            if (slist.size() > n)
                slist.pop_back();
        }
    }

    return slist;
}

double  avg(bypattern_t & distribution, double cutoff) {
    // find the average to solve the distribution
    // If the running average or smalest average exceeds the cutoff, abort the search

    // first, calculate the denominator
    int denominator = 0;
    for (auto p : distribution) 
        denominator += (int)p.second.size();

    double numerator_cutoff = cutoff * (double)denominator;

    // calculate the smallest the numerator can possibly be
    // if it is larger than the cutoff, we don't need to bother calculating the actual value
    double smallest_numerator = 0.0;
    for (auto p : distribution) {

        if (int2str(p.first) == "GGGGG")
            continue;

        if (p.second.size() == 1)
            smallest_numerator += 1.0;

        else
            smallest_numerator += 2.0 * p.second.size() - 1.0;
    }
    if (smallest_numerator > numerator_cutoff) {
        stats.smallest_cutoff_hits++;
        return smallest_numerator / (double)denominator;
    }

    // now calculate the numerator, aborting if it exceeds the cutoff
    double numerator = 0.0;
    for (auto p : distribution) {
 
        if (int2str(p.first) == "GGGGG")
            continue;

        if (p.second.size() == 1)
            numerator += 1.0;

        else if  (p.second.size() == 2)
            numerator += 3.0;

        else
            numerator += (bestguess(p.second).average + 1.0) * p.second.size();

        if (numerator > numerator_cutoff) {
            stats.cutoff_hits++;
            break;
        }
    }
    return numerator / (double)denominator;
}

//std::unordered_map<std::string, double> minavg_cache;
robin_hood::unordered_flat_map< std::string, double> minavg_cache;

//------------------------------------------------------------------------------------------------------------------------------------

robin_hood::unordered_flat_map< std::string, bestguess_t> bestguess_cache;

// from https://www.froglogic.com/blog/recursion-depth-counting/
class RecursionCounter
{
public:
    inline RecursionCounter() { ++cnt; /*printf("depth %d\n", cnt);*/ }
    inline ~RecursionCounter() {
        if (pattern_cache.size() > 100000 and cnt <= 2) {
            //printf("depth %d, clearing pattern cache\n", cnt);
            pattern_cache.clear();
        }
        --cnt; 
    }
    inline operator int() const { return cnt; }
private:
    static int cnt;
};
int RecursionCounter::cnt = 0;


bestguess_t bestguess(strvec_t &solutions, bool printProgress ) {
    // find the guess that produces the best average for the solutions

    RecursionCounter recursionDepth;
 
    if (solutions.size() == 1)
        //no need to search
        return bestguess_t{ 0.0, solutions[0] };
 
    if (solutions.size() == 2)
        //no need to search
        return bestguess_t{ 0.5, solutions[0] };

    // see if we have cached the result
    std::string cs = concat(solutions);
    if (bestguess_cache.find(cs) != bestguess_cache.end()) {
        stats.bestguesscache_hits++;
        return bestguess_cache[cs];
    }
    
    double bestavg = powf(10.0, 10);
    std::string bestguess = "?????";

    // first, consider guesses from words in the solution list
    std::string reserve_guess = "?????";

    if (solutions.size() < 25) {
        for (auto guess : solutions) {

            bypattern_t solsbypattern = splitbypattern(guess, solutions);
 
            if (solsbypattern.size() == solutions.size())
                // can't do better than this
                return bestguess_t{ (float)(solutions.size()-1)/(float)solutions.size(), guess };

            if (solsbypattern.size() == solutions.size() - 1)
                reserve_guess = guess;

            double a = avg(solsbypattern, bestavg);
            //printf("solution word guess %s, average %f\n", guess.c_str(), a);

            if (a < bestavg) {
                bestavg = a;
                bestguess = guess;
                if(printProgress) printf("best guess from solution words: '%s', average to solve: %f\n", guess.c_str(), a);
            }
        }

    }
    if (reserve_guess != "?????") {
        // we have found the next best to a full spread, and can't do better by calling shortlist
        bestguess_cache[cs] = bestguess_t{ 1.0, reserve_guess };
        return bestguess_t{ 1.0, reserve_guess };
    }

    std::vector< std::pair < double, std::string>> sl = shortlist(40, solutions);
    if (printProgress) {
        printf("shortlist:\n");
        for (auto w : sl)
            std::cout << w.second << " (" << w.first << ")" << ", ";
        std::cout << "\n";
    }
    
    for (auto x : sl) {
        std::string guess = x.second;

        bypattern_t solsbypattern;
        for (auto sol : solutions) {
            std::string p = pattern(sol, guess);
            solsbypattern[str2int(p)].push_back(sol);
        }

        if (solsbypattern.size() == solutions.size()) {
            // can't do better than this
            bestguess_cache[cs] = bestguess_t{ 1.0, guess };
            return bestguess_t{ 1.0, guess };
        }

        double a = avg(solsbypattern, bestavg);
        if (a < bestavg) {
            bestavg = a;
            bestguess = guess;
            if (printProgress) printf("best guess so far: %s, average to solve: %f\n", guess.c_str(), a);
        }
        else {
            if (printProgress) printf("'%s' not as good as '%s'\n", guess.c_str(), bestguess.c_str());
        }
 
        if (bestguess == "?????") {
            throw std::runtime_error("search: failed to find bestguess");

        }
    }

    bestguess_cache[cs] = bestguess_t{ bestavg, bestguess };
    return bestguess_t{ bestavg, bestguess };
}