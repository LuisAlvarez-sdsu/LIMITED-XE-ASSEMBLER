#include <iostream>
#include <vector>
#include "assembler.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Error: No input files provided.\n";
        cout << "Usage: ./lxe file1.sic file2.sic ...\n";
        return 1;
    }

    Assembler assembler;

    for (int i = 1; i < argc; i++) {
        string filename = argv[i];
        cout << "Processing file: " << filename << endl;

        if (!assembler.assemble(filename)) {
            cout << "Failed to process: " << filename << endl;
        }
    }

    return 0;
