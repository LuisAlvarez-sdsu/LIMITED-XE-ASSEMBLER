/*
 * CS530 - Project 2: Limited XE Assembler
 * Team Members: [Your Names + RedIDs]
 *
 * Compile: g++ -o lxe lxe.cpp
 * Run: ./lxe file1.sic file2.sic
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>

using namespace std;

// OPTAB for debug of the file creation it structred (intructions, OP code)
map<string, string> OPTAB = {
    {"LDA", "00"}, {"STA", "0C"}, {"LDX", "04"},
    {"ADD", "18"}, {"SUB", "1C"}, {"MUL", "20"},
    {"DIV", "24"}, {"COMP", "28"}, {"J", "3C"},
    {"JEQ", "30"}, {"JLT", "38"}, {"JSUB", "48"},
    {"RSUB", "4C"}
};

struct Line {
    int address;
    string label, opcode, operand;
};

// split line (needs work) 
Line parseLine(string line) {
    stringstream ss(line);
    Line l;
    l.label = l.opcode = l.operand = "";

    ss >> l.label;
    if (OPTAB.count(l.label) || l.label == "START") {
        l.opcode = l.label;
        l.label = "";
        ss >> l.operand;
    } else {
        ss >> l.opcode >> l.operand;
    }

    return l;
}

// PASS 1
void pass1(string filename, vector<Line> &lines, map<string, int> &SYMTAB, int &startAddr) {
    ifstream file(filename);
    string line;
    int locctr = 0;

    while (getline(file, line)) {
        if (line.empty() || line[0] == '.') continue;

        Line l = parseLine(line);

        if (l.opcode == "START") {
            startAddr = stoi(l.operand, nullptr, 16);
            locctr = startAddr;
            l.address = locctr;
        } else {
            l.address = locctr;

            if (!l.label.empty()) {
                SYMTAB[l.label] = locctr;
            }

            if (OPTAB.count(l.opcode)) {
                locctr += 3;
            } else if (l.opcode == "WORD") {
                locctr += 3;
            } else if (l.opcode == "RESW") {
                locctr += 3 * stoi(l.operand);
            } else if (l.opcode == "RESB") {
                locctr += stoi(l.operand);
            } else if (l.opcode == "BYTE") {
                locctr += (l.operand.length() - 3); // rough
            }
        }

        lines.push_back(l);
    }

    file.close();
}

// PASS 2
void pass2(string filename, vector<Line> &lines, map<string, int> &SYMTAB) {
    string outFile = filename.substr(0, filename.find('.')) + ".l";
    ofstream out(outFile);

    for (auto &l : lines) {
        string objcode = "";

        if (OPTAB.count(l.opcode)) {
            string opcode = OPTAB[l.opcode];
            int addr = SYMTAB.count(l.operand) ? SYMTAB[l.operand] : 0;

            stringstream ss;
            ss << opcode << setw(4) << setfill('0') << hex << addr;
            objcode = ss.str();
        } else if (l.opcode == "WORD") {
            stringstream ss;
            ss << setw(6) << setfill('0') << hex << stoi(l.operand);
            objcode = ss.str();
        } else if (l.opcode == "BYTE") {
            objcode = l.operand;
        }

        out << setw(4) << setfill('0') << hex << l.address << "  "
            << setw(8) << left << l.label
            << setw(8) << l.opcode
            << setw(10) << l.operand
            << objcode << endl;
    }

    out.close();
}

// Write SYMTAB
void writeSymtab(string filename, map<string, int> &SYMTAB) {
    string outFile = filename.substr(0, filename.find('.')) + ".st";
    ofstream out(outFile);

    for (auto &sym : SYMTAB) {
        out << setw(10) << sym.first << " "
            << setw(4) << setfill('0') << hex << sym.second << endl;
    }

    out.close();
}

// MAIN
int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Error: No input files provided. Exiting.\n";
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        string filename = argv[i];

        vector<Line> lines;
        map<string, int> SYMTAB;
        int startAddr = 0;

        pass1(filename, lines, SYMTAB, startAddr);
        pass2(filename, lines, SYMTAB);
        writeSymtab(filename, SYMTAB);

        cout << "Processed: " << filename << endl;
    }

    return 0;
}
