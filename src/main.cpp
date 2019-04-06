#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include "hash.h"
#include "dynamic_table.h"
#include "static_table.h"

void readLines(std::istream &is, std::vector<std::string> &out) {
	std::string line;
	while (is.good()) {
		std::getline(is, line);
		out.push_back(line);
	}
}

int main(int argc, const char **argv) {
	// Defaults
	const std::string &DEFAULT_FILENAME = "brands.txt";
	constexpr std::size_t DEFAULT_SIZE = 150;

	// Parameters
	const std::string fileName = argc > 1 ? argv[1] : DEFAULT_FILENAME;
	const std::size_t size = argc > 2 ? std::stoull(argv[2]) : DEFAULT_SIZE;

	// Read file into vector
	std::ifstream inputFile(fileName);
	std::vector<std::string> lines;
	readLines(inputFile, lines);
	inputFile.close();

	// Construct and populate table
	StaticHashTable<std::string, std::size_t> table(HashString, size);
	for (const std::string &line : lines) {
		table.put(line, line.size());
	}
	std::cout << "Collisions: " << table.getCollisions() << std::endl;
	return EXIT_SUCCESS;
}
