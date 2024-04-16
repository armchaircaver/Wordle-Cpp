// pattern.cpp : Defines the exported functions for the DLL application.
//

#include <stdint.h>
#include <string.h>
#include "pch.h"
#include <iostream>

#define DLLEXPORT extern "C" __declspec(dllexport)

DLLEXPORT void pattern(const char* solutionword, const char* guess, int& result) {
    char list_solutionword[5]{};
    for (int i = 0; i < 5; i++) list_solutionword[i] = solutionword[i];

    const int dot = 0, Y = 1, G = 2;

    int p[5] = { dot,dot,dot,dot,dot };


    // find greens first, and remove the letter from the solution
    // so that we don't get false yellows for a repeated letter in the guess word
    for (int i = 0; i < 5; i++)
        if (solutionword[i] == guess[i]) {
            p[i] = G;
            list_solutionword[i] = '.';
        }

    // find yellows, removing a matched letter from the solution
    // so that we don't get further false yellows for a repeated letter in the guess word
    for (int i = 0; i < 5; i++) {
        // if guess[i] is in list_solutionword - this is a bottleneck
        char* pch = strchr(list_solutionword, guess[i]);

        if (pch != NULL)
            // if (strchr(list_solutionword.c_str(), guess[i]))
            if (p[i] != G) {
                p[i] = Y;
                list_solutionword[pch - list_solutionword] = '.';
            }
    }
    result = 0;
    for (int i = 0; i < 5; i++)
        result += p[i] << (2 * i);

    //std::cout << solutionword << "," << guess << "," << p << "\n";
}


DLLEXPORT void patterns(const char* solutions, const char* guess, const uint64_t numsolutions, uint64_t* results) {
    char sol[5]{};
    int res;
    for (int i = 0; i < numsolutions * 5; i += 5) {
        for (int j = 0; j < 5; j++) sol[j] = solutions[i + j];
        pattern(sol, guess, res);
        results[i / 5] = res;
    }
}



