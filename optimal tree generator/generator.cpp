#include "wordlists.h"
#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include "recursive.h"
#include <map>
#include <chrono>
#include <iostream>

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


int main() {

    PrimaryWords primarywords;
 
   
    std::string startguess ;
    std::cout << "Enter starting word: ";
    std::cin >> startguess;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    traversetree(1, startguess, primarywords.solutions);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();

    printf("total solutions found %d\n", totalsolsfound);
    printf("total guesses %d\n", totalguesses);
    printf("average = %f\n", (double)totalguesses / (double)totalsolsfound);
    printf("%lld seconds\n", elapsed);
}