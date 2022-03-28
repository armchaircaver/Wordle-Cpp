#include <iostream>
#include <string.h>
#include <set>

// from https://github.com/alex1770/wordle/blob/main/wordle.cpp
// Ternary, L->H, 0=black, 1=yellow, 2=green
int score(std::string& test, std::string& hidden) {
    char t[5], h[5];
    memcpy(t, test.c_str(), 5);
    memcpy(h, hidden.c_str(), 5);
    int k, l, s = 0, w;

    // Greens
    for (k = 0, w = 1; k < 5; k++) {
        if (t[k] == h[k]) { t[k] = 254; h[k] = 255; s += 2 * w; }
        w *= 3;
    }

    // Yellows
    for (k = 0, w = 1; k < 5; k++) {
        for (l = 0; l < 5; l++)if (t[k] == h[l]) { s += w; h[l] = 255; break; }
        w *= 3;
    }

    //printf("%s %s %s\n",testwords[i].c_str(),hiddenwords[j].c_str(),decscore(s).c_str());

    return s;
}

uint64_t str2int(std::string s) {
    uint64_t i=0ull;
    memcpy(&i, s.c_str(), 5);
    return i;
}

std::string int2str( uint64_t i) {
    char c[6] = { '?','?','?','?','?', 0 };
    memcpy(&c, &i, 5);
    return std::string(c);
}

int pattern2smallint(std::string s) {
    int i = 0;
    int pos = 0;
    for (auto c : s) {
        i |= (c & 3)<<pos;
        pos += 2;
    }
    return i;
}

std::string print_binary(uint64_t number)
{
    std::string binstring = "";
    if (number >> 1)
        binstring = print_binary(number >> 1);
    if (number & 1)
        binstring += "1";
    else
        binstring+= "0";

    return binstring;
}

int main()
{
    std::set<int> smallints;

    for (std::string a : {"G", "Y", "."})
        for (std::string b : {"G", "Y", "."})
            for (std::string c : {"G", "Y", "."})
                for (std::string d : {"G", "Y", "."})
                    for (std::string e : {"G", "Y", "."}) {
                        std::string p = a + b + c + d + e;
                        uint64_t i = str2int(p);
                        smallints.insert(pattern2smallint(p));
                        printf("%s %llX %s %d\n", p.c_str(), i, int2str(i).c_str(),  pattern2smallint(p) );
                    }
    printf("%zd items in smallints set\n", smallints.size()  );
    return 0;
}

