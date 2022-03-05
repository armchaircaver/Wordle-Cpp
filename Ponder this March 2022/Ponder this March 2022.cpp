#include <cstdio>
#include <vector>
#include <string>
#include <unordered_map>

const int MAXPRIME = 100001;
int prime[MAXPRIME];

std::vector<std::string> solutions;


std::string  pattern(std::string& solutionword, std::string& guess) {
    std::string list_solutionword = solutionword;
    std::string p = ".....";

    // find greens first, and remove the letter from the solution
    // so that we don't get false yellows for a repeated letter in the guess word
    for (int i = 0; i < 5; i++) {
        if (solutionword[i] == guess[i]) {
            p[i] = 'G';
            list_solutionword[i] = '.';
        }
    }

    // find yellows, removing a matched letter from the solution
    // so that we don't get further false yellows for a repeated letter in the guess word
    for (int i = 0; i < 5; i++) {
        // if guess[i] is in list_solutionword - this is a bottleneck
        if (list_solutionword.find(guess[i]) != std::string::npos) {
        // if (strchr(list_solutionword.c_str(), guess[i])){

            if (p[i] != 'G') {
                p[i] = 'Y';
                list_solutionword[list_solutionword.find(guess[i])] = '.';
            }
        }
    }
    return p;
}


void SieveOfEratosthenes(int n) {

    for (int i = 0; i < MAXPRIME; i++)
        prime[i] = 1;

    prime[0] = 0;
    prime[1] = 0;
    for (int p = 2; p * p <= MAXPRIME; p++)
    {
        if (prime[p]) {
            for (int i = p * 2; i <= MAXPRIME; i += p)
                prime[i] = 0;
        }
    }
    for (int p = 10000; p < MAXPRIME; p++)
        if (prime[p])
            solutions.push_back(std::to_string(p));
}


int main() {
     SieveOfEratosthenes(MAXPRIME);
    printf("%zd primes\n", solutions.size());
 
    std::unordered_map<std::string, int> patternmap;
    int i = 0;
    for (std::string a : {"G", "Y", "."})
        for (std::string b : {"G", "Y", "."})
            for (std::string c : {"G", "Y", "."})
                for (std::string d : {"G", "Y", "."})
                    for (std::string e : {"G", "Y", "."}) {
                        //printf("%s\n", (a + b + c + d + e).c_str() );
                        
                        patternmap[a + b + c + d + e] = i;
                        i++;
                    }
    printf("%zd patternmap items\n", patternmap.size());

    uint64_t smallest = 1ULL << 63;

    for (auto guess : solutions) {
        //printf("analysing guess %s\n", guess.c_str());

        int collection[243]{0};
        for (int i = 0; i < 243; i++)
            collection[i] = 0;

        for (auto s : solutions) {
            std::string p = pattern(s, guess);
            int pm = patternmap[p];
            collection[pm]++;
        }
        int sum = 0;
        for (int i = 0; i < 243; i++)
            sum += collection[i];
        if (sum != solutions.size()) {
            printf("mismatch \n");
            exit(1);
        }
 
        uint64_t M = 0;
        for (int i = 0; i < 243; i++)
            M += (uint64_t)collection[i] * (uint64_t)collection[i];
        if (M <= smallest) {
            printf("%s, %lld, %f\n", guess.c_str(), M, float(M) / solutions.size());
            smallest = M;
        }
    }
}