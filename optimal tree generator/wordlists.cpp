#include "wordlists.h"

PrimaryWords::PrimaryWords()
{

    for (int i = 0; i < sizeof(solutions_c) / sizeof(solutions_c[0]); i++)
        solutions.push_back(solutions_c[i]);

    printf("solutions vector initialised, %zd items\n", solutions.size());

    for (int i = 0; i < sizeof(alloptions_c) / sizeof(alloptions_c[0]); i++)
        alloptions.push_back(alloptions_c[i]);

    printf("alloptions vector initialised, %zd items\n", alloptions.size());

}
