#include "wordlists.h"
#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include "recursive.h"
#include <map>



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

void  traverse(int depth, std::string& startguess, strvec_t& inputlist) {


    std::map < std::string, strvec_t>  pg;
    for (auto sol : inputlist) {
        std::string p = pattern(sol, startguess);
        pg[p].push_back(sol);
    }


    printf(" %s ", startguess.c_str());

    for (auto x : pg) {

        if (depth == 1)
            clearpatterncache();

        std::string p = x.first;

        //if depth == 1 :
        //    clearcaches()

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
            std::string g2 = bestguess(pg[p]);
            traverse(depth + 1, g2, pg[p]);
        }
    }
}

int main() {
    //std::string startguess = "roate";
    //static std::vector < std::string>  tests = { "aback", "abase", "abate", "abbey", "abbot", "abhor", "abide", "abled", "abode", "abort", "about", "above", "abuse", "abyss" };
    //traverse(1, startguess, tests);
    /*
    static std::vector < std::string>  tests = { "fizzy", "hippy", "jiffy", "kinky", "minim", "ninny", "nymph", "piggy", "pinky", "pygmy", "vying", "whiff", "whiny", "wimpy" };
    std::string next = bestguess(tests);
    printf("next = %s\n", next.c_str());
    */

    std::string startguess = "salet";
    traverse(1, startguess, solutions);

    printf("total solutions found %d\n", totalsolsfound);
    printf("total guesses %d\n", totalguesses);
    printf("average = %f\n", (double)totalguesses / (double)totalsolsfound);
}