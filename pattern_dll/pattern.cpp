// pattern.cpp : Defines the exported functions for the DLL application.
//

#include <stdint.h>
#include <string.h>

#define DLLEXPORT extern "C" __declspec(dllexport)

DLLEXPORT char* pattern(char* solutionword, char* guess) {
    char list_solutionword[5]{};
    for (int i=0;i<5;i++) list_solutionword[i] = solutionword[i]; 

    char p[6] = { '.','.','.','.','.', 0};

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
        char* pch = strchr(list_solutionword, guess[i]);

        if (pch != NULL)
            // if (strchr(list_solutionword.c_str(), guess[i]))
            if (p[i] != 'G') {
                p[i] = 'Y';
                list_solutionword[pch - list_solutionword] = '.';
            }

    return p;
}

}



