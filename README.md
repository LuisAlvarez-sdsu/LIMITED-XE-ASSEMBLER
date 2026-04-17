# CS530 Spring 2025 - Project #2  
## Limited XE Assembler (`lxe`)

**Team Members**  
- [Full Name], RedID: [RedID]
- [Full Name], RedID: [RedID] 

---

## Overview

This project implements a limited two-pass assembler for the XE variant of the SIC/XE machine architecture.  
The assembler reads SIC/XE assembly source files and generates:

- a **listing file** with extension `.l`
- a **symbol table file** with extension `.st`

The program does **not** generate object files.

The executable name is:

```bash

lxe

Moving forward the next steps we should take
- Add the support for opcode format 4, #operand, @operand, X
- add the format 2 instructions, register table, BASE/NOBASE
- implement true format 3 logic and do the pc placement, base relative displacement, range checking
- improving the lisitng file with line numbers and error messages on same line

I think we should follow this struct Line {
    int lineNumber;
    int address;
    string raw;
    string label;
    string opcode;
    string operand;
    string objectCode;
    string error;
    bool isCommentOnly;
};

and use:

parseLine()
pass1()
pass2()
assembleFormat2()
assembleFormat34()



OUTLINEEEE
lxe-project/
├── Makefile
├── README.md
├── main.cpp
├── assembler.h
├── assembler.cpp
├── util.h
├── util.cpp
└── test/
    └── .gitkeep

#include <iostream>
#include "assembler.h"

using namespace std;

int main(int argc, char* argv[]) {

    // TODO: check for no input files
    if (argc < 2) {
        cout << "Usage: ./lxe file1.sic file2.sic\n";
        return 1;
    }

    // TODO: process each file
    for (int i = 1; i < argc; i++) {
        cout << "Processing: " << argv[i] << endl;

        assembleFile(argv[i]);  // call your main assembler function
    }

    return 0;
}

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <string>
#include <vector>
#include <map>

using namespace std;

// Represents one line of source
struct Line {
    int lineNumber;
    int address;

    string label;
    string opcode;
    string operand;
    string raw;

    string objectCode;
    string error;
};

//  MAIN FUNCTION 
void assembleFile(string filename);

//  PASSES 
void pass1(string filename,
           vector<Line>& lines,
           map<string,int>& SYMTAB,
           int& startAddr,
           int& programLength);

void pass2(vector<Line>& lines,
           map<string,int>& SYMTAB);

// OUTPUT 
void writeListing(string filename, vector<Line>& lines);
void writeSymtab(string filename, map<string,int>& SYMTAB);

//  HELPERS 
Line parseLine(string line, int lineNumber);
bool isDirective(string op);
int byteLength(string operand);

#endif

#include "assembler.h"
#include "util.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

// 
// Main control function
// 
void assembleFile(string filename) {

    vector<Line> lines;
    map<string,int> SYMTAB;

    int startAddr = 0;
    int programLength = 0;

    // TODO: run pass 1
    pass1(filename, lines, SYMTAB, startAddr, programLength);

    // TODO: run pass 2
    pass2(lines, SYMTAB);

    // TODO: write output files
    writeListing(filename, lines);
    writeSymtab(filename, SYMTAB);
}

// 
// PASS 1
// 
void pass1(string filename,
           vector<Line>& lines,
           map<string,int>& SYMTAB,
           int& startAddr,
           int& programLength) {

    ifstream file(filename);

    // TODO: check file opened

    string raw;
    int locctr = 0;
    int lineNumber = 0;

    while (getline(file, raw)) {

        lineNumber++;

        // TODO: parse line
        Line l = parseLine(raw, lineNumber);

        // TODO: skip comments

        // TODO: handle START

        // TODO: assign address

        // TODO: add label to SYMTAB

        // TODO: update LOCCTR based on opcode/directive

        lines.push_back(l);
    }

    // TODO: compute program length
}

// 
// PASS 2
// 
void pass2(vector<Line>& lines,
           map<string,int>& SYMTAB) {

    // TODO: loop through lines

    // TODO: generate object code

    // TODO: handle WORD, BYTE

    // TODO: handle instructions

    // TODO: handle errors
}

// 
// OUTPUT FILES
// 
void writeListing(string filename, vector<Line>& lines) {

    // TODO: open .l file

    // TODO: print each line with:
    // address, label, opcode, operand, object code, errors
}

void writeSymtab(string filename, map<string,int>& SYMTAB) {

    // TODO: open .st file

    // TODO: print symbol and address
}

// 
// PARSER
// 
Line parseLine(string line, int lineNumber) {

    Line l;
    l.raw = line;
    l.lineNumber = lineNumber;

    // TODO:
    // - detect comment lines
    // - detect label vs no label
    // - split into label/opcode/operand

    return l;
}

// 
// HELPERS
// 
bool isDirective(string op) {

    // TODO: return true if:
    // START, END, WORD, RESW, RESB, BYTE, BASE, NOBASE

    return false;
}

int byteLength(string operand) {

    // TODO:
    // handle C'...' and X'...'
    // return number of bytes

    return 0;
}

#ifndef UTIL_H
#define UTIL_H

#include <string>
using namespace std;

string trim(string s);
string toUpper(string s);
bool isNumber(string s);
string intToHex(int value, int width);

#endif

#include "util.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cctype>

using namespace std;

string trim(string s) {
    // TODO: remove spaces from start and end
    return s;
}

string toUpper(string s) {
    // TODO: convert string to uppercase
    return s;
}

bool isNumber(string s) {
    // TODO: check if string is a number
    return false;
}

string intToHex(int value, int width) {
    // TODO: convert integer to hex string
    return "";
}

