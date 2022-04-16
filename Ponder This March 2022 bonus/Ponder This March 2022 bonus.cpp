#include <cstdio>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

const int MAXPRIME = 10000001;
int prime[10000001];

std::vector<std::string> solutions;


std::string  pattern(std::string& solutionword, std::string& guess) {
    std::string list_solutionword = solutionword;
    std::string p = ".......";

    // find greens first, and remove the letter from the solution
    // so that we don't get false yellows for a repeated letter in the guess word
    for (int i = 0; i < 7; i++) {
        if (solutionword[i] == guess[i]) {
            p[i] = 'G';
            list_solutionword[i] = '.';
        }
    }

    // find yellows, removing a matched letter from the solution
    // so that we don't get further false yellows for a repeated letter in the guess word
    for (int i = 0; i < 7; i++) {
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
    for (int p = 1000000; p < MAXPRIME; p++)
        if (prime[p])
            solutions.push_back(std::to_string(p));
}


int main() {

    SieveOfEratosthenes(MAXPRIME);
    printf("%zd primes\n", solutions.size());
    //for (auto  s : solutions)
    //    printf("%s\n", s.c_str() );

    std::unordered_map<std::string, int> patternmap;
    int i = 0;
    for (std::string a : {"G", "Y", "."})
        for (std::string b : {"G", "Y", "."})
            for (std::string c : {"G", "Y", "."})
                for (std::string d : {"G", "Y", "."})
                    for (std::string e : {"G", "Y", "."})
                        for (std::string f : {"G", "Y", "."})
                            for (std::string g : {"G", "Y", "."})
                            {
                                printf("%s\n", (a + b + c + d + e + f + g).c_str());
                                patternmap[a + b + c + d + e + f + g] = i;
                                i++;
                            }
    printf("%zd patternmap items\n", patternmap.size());

    const int THREEp7 = 3 * 3 * 3 * 3 * 3 * 3 * 3;
    std::unordered_map<std::string, int[THREEp7]> guessmap;

    for (auto guess : solutions) {
 
        if (guess != "1584269")
            continue;
        
        int numUniqeChars = (int)std::unordered_set<char>(std::begin(guess), std::end(guess)).size();
        if (numUniqeChars < 7)
            continue;

        printf("collecting guess %s\n", guess.c_str());

        int collection[THREEp7]{ 0 };
        for (int i = 0; i < THREEp7; i++)
            collection[i] = 0;

        for (auto s : solutions) {
            std::string p = pattern(s, guess);
            int pm = patternmap[p];
            collection[pm]++;
        }
        // not sure why I can't assign the whole array at once
        for (int i = 0; i < THREEp7; i++)
            guessmap[guess][i] = collection[i];
    }
    for (auto x : solutions) {
        int numUniqeChars = (int)std::unordered_set<char>(std::begin(x), std::end(x)).size();
        if (numUniqeChars < 7)
            continue;

        if (x != "1584269")
            continue;

        
        int sum = 0;
        for (int i = 0; i < THREEp7; i++)
            sum += guessmap[x][i];
        if (sum != solutions.size()) {
            printf("mismatch 1\n");
            exit(1);
        }
    }
    printf(" no mismatches\n");

    uint64_t smallest = 1ULL << 63;
    for (auto guess : solutions) {
        if (guess != "1584269")
            continue;

        int numUniqeChars = (int)std::unordered_set<char>(std::begin(guess), std::end(guess)).size();
        if (numUniqeChars < 7)
            continue;

        uint64_t M = 0;
        for (int i = 0; i < THREEp7; i++)
            M += (uint64_t)guessmap[guess][i] * (uint64_t)guessmap[guess][i];
        if (M < smallest) {
            printf("%s, %lld / %lld =  %f\n", guess.c_str(), M,solutions.size(),  double(M) / solutions.size());
            smallest = M;
        }
    }
}