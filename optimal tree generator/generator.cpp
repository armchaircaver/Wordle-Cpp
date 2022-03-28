#include "wordlists.h"
#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include "recursive.h"
#include <map>
#include <chrono>

std::string BGY(std::string p) {
    std::string x = p;
    std::replace(x.begin(), x.end(), '.', 'B');
    return x;
}

//-----------------------------------------------------------------------------------  

int totalsolsfound = 0;
int totalguesses = 0;
std::string lastpatternprinted = "?????";

//-----------------------------------------------------------------------------------  

void traversetree(int depth, std::string& startguess, strvec_t& inputlist) {

    //printf("traverse %zd\n", inputlist.size());

    bypattern_t  pg = splitbypattern(startguess, inputlist);
    
    printf(" %s ", startguess.c_str());

    for (auto x : pg) {

        if (depth == 1)
            clearpatterncache();

        std::string p = int2str(x.first); 

 
        if (lastpatternprinted == "GGGGG") {
            std::string printstr = std::string(13 * (depth - 1) + 6, ' ') + BGY(p) + std::to_string(depth);
            if (p == "GGGGG") printstr += "\n";
            printf("%s", printstr.c_str());
        }
        else {
            std::string printstr = BGY(p) + std::to_string(depth);
            if (p == "GGGGG") printstr += "\n";
            printf("%s", printstr.c_str());
        }
        lastpatternprinted = p;


        if (p == "GGGGG") {
            totalsolsfound += 1;
            totalguesses += depth;
        }
        else {
            bestguess_t bg = bestguess(pg[str2int(p)]);
            traversetree(depth + 1, bg.bestguess, pg[str2int(p)]);
        }
    }
}

/*
std::vector<std::string> solutions;
std::vector<std::string> alloptions;
std::vector< std::pair<std::string, int> > alloptions_bits;
std::vector< std::pair<std::string, int> > solutions_bits;
*/
int main() {

    //std::string startguess = "roate";
    //static std::vector < std::string>  tests = { "aback", "abase", "abate", "abbey", "abbot", "abhor", "abide", "abled", "abode", "abort", "about", "above", "abuse", "abyss" };
    //traverse(1, startguess, tests);
    /*
    static std::vector < std::string>  tests = { "fizzy", "hippy", "jiffy", "kinky", "minim", "ninny", "nymph", "piggy", "pinky", "pygmy", "vying", "whiff", "whiny", "wimpy" };
    std::string next = bestguess(tests);
    printf("next = %s\n", next.c_str());
    */
/*
    for (int i = 0; i < sizeof(solutions_c) / sizeof(solutions_c[0]); i++) {
        solutions.push_back(solutions_c[i]);
        int bitmap = 0;
        int j = 0;
        while (solutions_c[i][j]) {
            bitmap |= 1<< (solutions_c[i][j] - 'a');
            j++;
        }
        solutions_bits.push_back(std::make_pair(std::string(solutions_c[i]), bitmap));
        //printf(" %s % X\n", std::string(solutions_c[i]).c_str(), bitmap);
    }
    
    printf("solutions vector initialised, %zd items\n", solutions.size());
   
    for (int i = 0; i < sizeof(alloptions_c) / sizeof(alloptions_c[0]); i++) {
        alloptions.push_back(alloptions_c[i]);
        int bitmap = 0;
        int j = 0;
        while (alloptions_c[i][j]) {
            bitmap |= 1<< (alloptions_c[i][j] - 'a');
            j++;
        }
        alloptions_bits.push_back(std::make_pair(alloptions_c[i], bitmap));
    }
    printf("alloptions vector initialised, %zd items\n", alloptions.size());
 
 */
    /*
    int count = 0;
    for (int i=0; i< alloptions.size(); i++)
        for (int j = 0; j < solutions.size();j++) {
            std::string p1 = pattern(solutions[j], alloptions[i]);
            std::string p2 = pattern(solutions_bits[j], alloptions[i]);
            count++;
            if (p1 != p2) {
                printf("%s %s %s %s \n", p1.c_str(), p2.c_str(), solutions[j].c_str(), alloptions[i].c_str());
                exit(0);
            }

        }
    printf("pattern algorithms verified for %d cases\n",count);
    */

    PrimaryWords primarywords;
    strvec_t solutions = primarywords.solutions;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
   
    std::string startguess = "salet";
    traversetree(1, startguess, primarywords.solutions);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();

    printf("total solutions found %d\n", totalsolsfound);
    printf("total guesses %d\n", totalguesses);
    printf("average = %f\n", (double)totalguesses / (double)totalsolsfound);
    printf("%lld seconds\n", elapsed);
}