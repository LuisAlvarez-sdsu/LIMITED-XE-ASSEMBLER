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
