#pragma once

#include <vector>
#include <string>
#include "robin_hood.h"

typedef std::vector<std::string> strvec_t;


class PrimaryWords {

public:
    strvec_t solutions;
    strvec_t alloptions;
    PrimaryWords(); 
};


extern PrimaryWords primarywords;
