 #include "wordlists.h"
#include <algorithm>
#include <stdexcept>
#include "recursive.h"

double powers[1000];
int powersinit = 0;

robin_hood::unordered_flat_map< std::string,  std::string> pattern_cache;

PrimaryWords primarywords;

void clearpatterncache() {
    pattern_cache.clear();
 }

std::string  pattern(std::string& solutionword, std::string& guess) {

    //if (pattern_cache.find(guess+solutionword) != pattern_cache.end())
    //    return pattern_cache[guess + solutionword];

    std::string list_solutionword = solutionword;
    std::string p = ".....";
    //int solbitmap = primarywords.wordbitmap[solutionword];

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
        //if (((guess[i] - 'a') & solbitmap) == 0)
        //    continue;

        // if guess[i] is in list_solutionword - this is a bottleneck
        //if (list_solutionword.find(guess[i]) != std::string::npos)
        if (strchr(list_solutionword.c_str(), guess[i]))
        {
            p[i] = 'Y';
            list_solutionword[list_solutionword.find(guess[i])] = '.';
        }
    }
    //pattern_cache[guess+solutionword] = p;
    return p;
}

std::vector< std::pair < double, std::string>> shortlist(int n, std::vector<std::string> & inputsols) {

    // returns a list of pairs (squaresum, guess)
    // The list returned is the list for guesses with the smallest squaresum


    std::vector< std::pair < double, std::string>> slist;
 
    if (!powersinit) {
        powers[0] = 0.0;
        for (int i = 1; i < 1000; i++)
            powers[i] = powl((double)i, 1.1);
        powersinit = 1;
    }

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
            squaresum += powers[x]-powers[x-1];


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

double  avg(bypattern_t & distribution) {
    double numerator = 0.0;
    int denominator = 0;
    for (auto p : distribution) {

        denominator += (int)p.second.size();

        if (int2str(p.first) == "GGGGG")
            continue;

        if (p.second.size() == 1)
            numerator += 1.0;

        else if  (p.second.size() == 2)
            numerator += 3.0;

        else
            numerator += (minavg(p.second) + 1.0) * p.second.size();
    }
    //printf("average %f / %d\n", numerator, denominator);
    return numerator / (double)denominator;
}

//std::unordered_map<std::string, double> minavg_cache;
robin_hood::unordered_flat_map< std::string, double> minavg_cache;

double minavg(strvec_t& solutions) {
    bestguess_t bg = bestguess(solutions);
    return bg.average;
}
//------------------------------------------------------------------------------------------------------------------------------------
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
        res += s+",";
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
//------------------------------------------------------------------------------------------------------------------------------------

robin_hood::unordered_flat_map< std::string, bestguess_t> bestguess_cache;

bestguess_t bestguess(strvec_t &solutions) {
    // find the guess that produces the best average for the solutions

 
    if (solutions.size() == 1)
        //no need to search
        return bestguess_t{ 0.0, solutions[0] };
 
    if (solutions.size() == 2)
        //no need to search
        return bestguess_t{ 0.5, solutions[0] };

    // see if we have cached the result
    std::string cs = concat(solutions);
    if (bestguess_cache.find(cs) != bestguess_cache.end()) {
        //printf("bestguess cache used for %s\n", cs.c_str());
        return bestguess_cache[cs];
    }
    
    double bestavg = powf(10.0, 10);
    std::string bestguess = "?????";

    // first, consider guesses from words in the solution list
    std::string reserve_guess = "?????";

    if (solutions.size() < 25) {
        for (auto guess : solutions) {

            bypattern_t solsbypattern = splitbypattern(guess, solutions);
            /*
            for (auto sol : solutions) {
                std::string p = pattern(sol, guess);
                solsbypattern[p].push_back(sol);
            }
            */ 

            if (solsbypattern.size() == solutions.size())
                // can't do better than this
                return bestguess_t{ (float)(solutions.size()-1)/(float)solutions.size(), guess };

            if (solsbypattern.size() == solutions.size() - 1)
                reserve_guess = guess;

            double a = avg(solsbypattern);
            //printf("solution word guess %s, average %f\n", guess.c_str(), a);

            if (a < bestavg) {
                bestavg = a;
                bestguess = guess;
            }
        }

    }
    if (reserve_guess != "?????")
        // we have found the next best to a full spread, and can't do better by calling shortlist
        return bestguess_t{ 1.0, reserve_guess };

    std::vector< std::pair < double, std::string>> sl = shortlist(15, solutions);
    
    for (auto x : sl) {
        std::string guess = x.second;
        double cost = x.first;
        //printf(" %f, %s \n", cost, guess.c_str());
    }

    for (auto x : sl) {
        std::string guess = x.second;

        bypattern_t solsbypattern;
        for (auto sol : solutions) {
            std::string p = pattern(sol, guess);
            solsbypattern[str2int(p)].push_back(sol);
        }

        if (solsbypattern.size() == solutions.size())
            // can't do better than this
            return bestguess_t{ 1.0, guess };

        double a = avg(solsbypattern);
        //printf("shortlist guess %s, average %f\n", guess.c_str(), a);
        if (a < bestavg) {
            bestavg = a;
            bestguess = guess;
        }
 
        if (bestguess == "?????") {
            throw std::runtime_error("search: failed to find bestguess");

        }
    }

    bestguess_cache[cs] = bestguess_t{ bestavg, bestguess };
    return bestguess_t{ bestavg, bestguess };
}