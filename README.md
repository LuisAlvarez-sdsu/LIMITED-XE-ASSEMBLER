# CS530 Spring 2025 - Project #2  
## Limited XE Assembler (`lxe`)

**Team Members**  
- [Luis Alvarez], RedID: [RedID]
- [Jazmin Gallegos], RedID: [828116114] 

---

## Overview

This project implements a limited two-pass assembler for the XE variant of the SIC/XE architecture.

The assembler reads one or more SIC/XE assembly source files from the command line and generates, for each input file:

a listing file with extension .l
a symbol table file with extension .st

The program performs the standard two-pass assembler process:

Pass 1
parses the source program
assigns addresses using a location counter (LOCCTR)
builds the symbol table (SYMTAB)
detects and stores literals in the literal table (LITTAB)
Pass 2
resolves symbols and literals
generates object code for supported instructions and directives
writes formatted listing output

This project does not generate an object program file, as specified by the assignment.

The executable name is:
lxe

Features Implemented
The assembler supports the following major features:

Multiple input source files from the command line
Two-pass assembly design
Symbol table generation (SYMTAB)
Literal table support (LITTAB)
Literal placement using LTORG and END
Listing file generation (.l)
Symbol table file generation (.st)
SIC/XE instruction support for implemented format 2, 3, and 4 instructions
Addressing mode handling for:
immediate addressing (#)
indirect addressing (@)
simple addressing
indexed addressing (,X)
Extended format instructions using +opcode
Support for assembler directives:
START
END
WORD
BYTE
RESW
RESB
BASE
LTORG
Input

The program accepts one or more SIC/XE source files as command-line arguments.

Example:

./lxe io.sic calc.sic main.sic

If no input files are provided, the program prints an error message and terminates.

Output

For each input source file, the assembler generates:

filename.l
A listing file containing addresses, labels, opcodes, operands, and generated object code
filename.st
A symbol table file containing symbols, assigned addresses, and literal information

Example:

Input:

./lxe calc.sic

Output files:

calc.l
calc.st
File Description
Source Files
lxe.cpp - main program logic, Pass 1, Pass 2, and output generation
lxe.h - structure definitions, global tables, and function declarations
Build File
Makefile - compiles the project and creates the executable lxe
Test Files
calc.sic - tests arithmetic instructions and symbol handling
io.sic - tests loops, I/O instructions, byte constants, and branching
main.sic - tests literals, LTORG, and format 4 instructions
Output Files
*.l - listing files
*.st - symbol table files
Data Structures Used

The assembler uses the following primary data structures:

Line

Stores one parsed line of source code, including:

address
label
opcode
operand
Literal

Stores one literal, including:

literal name
assigned address
length
SYMTAB

Implemented as:

map<string, int>

Stores label-to-address mappings.

LITTAB

Implemented as:

vector<Literal>

Stores literals encountered during Pass 1.

OPTAB

Stores format 3/4 opcode values.

OPTAB2

Stores format 2 opcode values.

REG

Stores register encoding values for format 2 instructions.

Program Design

The assembler is organized into the following major functions:

parseLine()
Parses a source line into label, opcode, and operand fields
pass1()
Builds the symbol table, assigns addresses, and collects literals
pass2()
Generates object code and listing output
addLiteral()
Adds literals to the literal table
assignLiterals()
Assigns addresses to pending literals during LTORG or END
literalLength()
Computes the byte length of a literal
literalObjectCode()
Converts a literal into hexadecimal object code
writeSymtab()
Writes the symbol table and literal table to the .st file
How to Compile

Use make to compile the project:

make

This creates the executable:

lxe

To remove generated files and rebuild:

make clean
make
How to Run

Run the assembler with one or more source files:

./lxe file1.sic file2.sic

Example:

./lxe io.sic calc.sic main.sic
Example Test Cases
1. Arithmetic Program

calc.sic

tests LDA, ADD, STA, RSUB
tests symbol resolution
tests WORD and RESW
2. I/O Program

io.sic

tests looping and branching
tests TD, RD, WD, TIX, JLT
tests byte constants and labels
3. Literal / Format 4 Program

main.sic

tests literals such as =C'HI'
tests LTORG
tests format 4 instruction handling with +JSUB
Limitations

This assembler is limited to the project requirements and currently has the following limitations:

EQU and ORG are not implemented
Object program files are not generated
Opcode support is limited to the instructions implemented in the project
Error handling is partial for some invalid input cases
Duplicate symbol detection and undefined symbol handling may be limited depending on the input
Error Handling

The program currently handles the following cases:

no input files provided
missing or invalid source files
empty or invalid files

The assembler attempts to continue processing when possible so that output files can still be generated for valid portions of the input.

