#include <cstdio>
#include <vector>
#include <string>

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
        if (list_solutionword.find(guess[i]) != std::string::npos) {
            if (p[i] != 'G') {
                p[i] = 'Y';
                list_solutionword[list_solutionword.find(guess[i])] = '.';
            }
        }
    }
    return p;
}


int main() {
    std::string sol = "arise";
    std::string guess = "baice";
    printf("%s\n", pattern(sol, guess).c_str());

    sol = "abcde";  guess = "aaabf";
    printf("%s\n", pattern(sol, guess).c_str());

}