#include "wordlists.h"
//#include <unordered_map>
#include "robin_hood.h"
#include <algorithm>
#include <stdexcept>
#include "recursive.h"

double powers[1000];
int powersinit = 0;

std::unordered_map< std::string, std::string> pattern_cache;

void clearpatterncache() {
    pattern_cache.clear();
 }

std::string  pattern(std::string& solutionword, std::string& guess) {

    //if (pattern_cache.find(guess+solutionword) != pattern_cache.end())
    //    return pattern_cache[guess + solutionword];

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
    for (int i = 0; i < 5; i++)
        // if guess[i] is in list_solutionword - this is a bottleneck
        if (list_solutionword.find(guess[i]) != std::string::npos)
            // if (strchr(list_solutionword.c_str(), guess[i]))
            if (p[i] != 'G') {
                p[i] = 'Y';
                list_solutionword[list_solutionword.find(guess[i])] = '.';
            }
    //pattern_cache[guess+solutionword] = p;
    return p;
}


std::vector< std::pair < double, std::string>> shortlist(int n, std::vector<std::string> & solutions) {

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
    robin_hood::unordered_flat_map< std::string, int> totalsbypattern;

    for (auto guess : alloptions) {

        double squaresum = 0.0;
        solsbypattern.clear();
        totalsbypattern.clear();

        for (auto sol : solutions) {
            std::string p = pattern(sol, guess);
            totalsbypattern[p] ++;

            // accumulate the sum of squares of totals for each pattern
            // incrementing the sum of squares : x * *2 - (x - 1) * *2 = 2 * x - 1
            // squaresum += 2 * totalsbypattern[p] - 1

            // however, this seems to produce a better shortlist
            int x = totalsbypattern[p];
            //squaresum += powers[x] - powers[x - 1];
            squaresum += powers[x]-powers[x-1];


            if (slist.size() == n && squaresum > slist.back().first)
                break;
        }
        // we have found a guess that distributes each solution in a different pattern
        // and one of the solutions is a guess 
        if (squaresum == solutions.size() && std::find(solutions.begin(), solutions.end(), guess) != solutions.end()) {
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

        if (p.first == "GGGGG")
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

    std::string strsolutions;
    for (auto s : solutions)
        strsolutions += s;
    if (minavg_cache.find(strsolutions) != minavg_cache.end())
        return minavg_cache[strsolutions];

    double  m = 10000000000000000000.0;

    std::vector< std::pair < double, std::string>> slist = shortlist(15, solutions);

    for (auto x : slist) {
        std::string guess = x.second;

        bypattern_t solsbypattern;
        for (auto sol : solutions) {
            std::string p = pattern(sol, guess);
            solsbypattern[p].push_back(sol);
        }
        double a = avg(solsbypattern);
        if (a < m) 
            m = a;
    }
    minavg_cache[strsolutions] = m;
    return m;
}
//------------------------------------------------------------------------------------------------------------------------------------
std::string bestguess(strvec_t &solutions) {
    // find the guess that produces the best average for the solutions

    //printf(" % zd alloptions\n", alloptions.size());
    //exit(0);

    if (solutions.size() <= 2) {
        //if verbose : print("no need to search")
        return solutions[0];
    }

    //starttime = perf_counter()
    double bestavg = powf(10.0, 10);
    std::string bestguess = "?????";

    // first, consider guesses from words in the solution list
    std::string reserve_guess = "?????";

    if (solutions.size() < 25) {
        for (auto guess : solutions) {

            bypattern_t solsbypattern;
            for (auto sol : solutions) {
                std::string p = pattern(sol, guess);
                solsbypattern[p].push_back(sol);
            }

            if (solsbypattern.size() == solutions.size())
                // can't do better than this
                return guess;

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
        return reserve_guess;

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
            solsbypattern[p].push_back(sol);
        }

        if (solsbypattern.size() == solutions.size())
            // can't do better than this
            return guess;

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

    return bestguess;
}