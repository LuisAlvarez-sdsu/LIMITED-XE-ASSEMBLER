// Assignment #2 LXE Assembler
// CS530-05-Spr2026
// Team Memebers: Luis Alvarez, Jazmin Gallegos
// LXE.h

#ifndef LXE_H
#define LXE_H

// Standard C++ libraries required for assembler functionality
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>
#include <string>

using namespace std;

// DATA STRUCTURES
// Represents a single line of assembly code after parsing.
// Used throughout Pass 1 and Pass 2 for processing.
struct Line 
{
    int address = 0;      // Memory address assigned during PASS 1
    string label;         // Optional label field (symbol definition)
    string opcode;        // Instruction or assembler directive
    string operand;       // Operand (symbol, constant, register, etc.)
};

// Represents a literal entry such as =C'EOF' or =X'05'
struct Literal 
{
    string name;          // Literal string as written in source code
    int address = -1;     // Assigned during LTORG or END processing
    int length = 0;       // Byte length of literal
};

// GLOBAL TABLES
extern map<string, int> OPTAB;
extern map<string, int> OPTAB2;
extern map<string, int> REG;
extern vector<Literal> LITTAB;

// FUNCTIONS
int literalLength(string lit);
string literalObjectCode(string lit);
void addLiteral(string lit);
void assignLiterals(int &locctr, vector<Line> &lines, int pos);

Line parseLine(string line);

void pass1(string filename, vector<Line> &lines,
           map<string,int> &SYMTAB,
           int &startAddr, int &programLength);

void pass2(string filename, vector<Line> &lines,
           map<string,int> &SYMTAB);

#endif