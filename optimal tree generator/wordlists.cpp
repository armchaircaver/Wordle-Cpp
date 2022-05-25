#include "wordlists.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::vector<std::string> read_csv(std::string filename) {
    // Reads a single line CSV file into a vector of string

  
    // Create an input filestream
    std::ifstream myFile(filename);

    // Make sure the file is open
    if (!myFile.is_open()) throw std::runtime_error("Could not open file");

    // Helper vars
    std::string line, word;
 
    std::vector<std::string> result;

    // Read the first line
    if (myFile.good())
    {
        // Extract the first line in the file
        std::getline(myFile, line);

        // Create a stringstream from line
        std::stringstream ss(line);

        // Extract each word for the line
        while (std::getline(ss, word, ',')) {
             result.push_back(word);
        }
    }
 
    // Close file
    myFile.close();

    return result;
}

PrimaryWords::PrimaryWords() {

    if (solutions.size() == 0) {
 
        solutions = read_csv("solutions.txt");
        alloptions = read_csv("guesses.txt");

        alloptions.insert(alloptions.end(), solutions.begin(), solutions.end());

        printf("solutions vector initialised, %zd items\n", solutions.size());
        printf("alloptions vector initialised, %zd items\n", alloptions.size());
    }

}