// Assignment #2 LXE Assembler
// CS530-05-Spr2026
// Team Memebers: Luis Alvarez, Jazmin Gallegos
// LXE.CPP

#include "lxe.h"

using namespace std;

// OPCODE TABLES
// OPTAB: format 3/4 instructions (standard SIC/XE ops)
map<string, int> OPTAB = {
    {"LDA", 0x00}, {"STA", 0x0C}, {"LDX", 0x04},
    {"STL", 0x14}, {"LDB", 0x68},
    {"ADD", 0x18}, {"SUB", 0x1C}, {"MUL", 0x20},
    {"DIV", 0x24}, {"COMP", 0x28},
    {"J", 0x3C}, {"JEQ", 0x30}, {"JLT", 0x38},
    {"JSUB", 0x48}, {"RSUB", 0x4C},
    {"TD", 0xE0}, {"RD", 0xD8}, {"WD", 0xDC},
    {"TIX", 0x2C}
};

// OPTAB2: format 2 (register-based instructions)
map<string, int> OPTAB2 = {
    {"CLEAR", 0xB4}, {"TIXR", 0xB8}
};

// Register encoding table used in format 2
map<string, int> REG = {
    {"A", 0}, {"X", 1}, {"L", 2},
    {"B", 3}, {"S", 4}, {"T", 5}, {"F", 6}
};

vector<Literal> LITTAB;       // global literal table

// LITERAL HANDLING
// Compute byte length of a literal
// C'EOF' => 3 bytes, X'F1' => 1 byte
int literalLength(string lit) {
    if (lit[1] == 'C') return lit.length() - 4;
    if (lit[1] == 'X') return (lit.length() - 4) / 2;
    return 0;
}

// Convert literal into machine-readable hex object code
string literalObjectCode(string lit) {
    string obj = "";

    if (lit[1] == 'C') {
        // Convert each character into ASCII hex
        for (int i = 3; i < lit.length() - 1; i++) {
            stringstream ss;
            ss << hex << uppercase << setw(2) << setfill('0')
               << (int)lit[i];
            obj += ss.str();
        }
    }
    else if (lit[1] == 'X') {
        obj = lit.substr(3, lit.length() - 4);
    }

    return obj;
}

// Add literal to table (prevents duplicates)
void addLiteral(string lit) {
    for (auto &l : LITTAB)
        if (l.name == lit) return;

    Literal l;
    l.name = lit;
    l.length = literalLength(lit);
    LITTAB.push_back(l);
}

// Assign addresses to all pending literals
// Triggered by LTORG or END directive
void assignLiterals(int &locctr, vector<Line> &lines, int pos) {
    vector<Line> newLines;

    for (auto &l : LITTAB) {
        if (l.address == -1) {
            l.address = locctr;

            // Create pseudo instruction for literal placement
            Line litLine;
            litLine.address = locctr;
            litLine.label = "*";      // marks literal line in listing
            litLine.opcode = l.name;

            newLines.push_back(litLine);
            locctr += l.length;
        }
    }

    // Insert literals into program stream
    lines.insert(lines.begin() + pos, newLines.begin(), newLines.end());
}

// PARSER
// Splits assembly line into label/opcode/operand
// Handles formats with or without label
Line parseLine(string line) {
    Line l;

    // Ignore blank lines and comments
    if (line.empty() || line[0] == '.') return l;

    stringstream ss(line);
    vector<string> tokens;
    string temp;

    while (ss >> temp) tokens.push_back(temp);

    // 1-field line (only opcode)
    if (tokens.size() == 1)
        l.opcode = tokens[0];

    // 2-field line
    else if (tokens.size() == 2) {
        if (line[0] == ' ' || line[0] == '\t') {
            l.opcode = tokens[0];
            l.operand = tokens[1];
        } else {
            l.label = tokens[0];
            l.opcode = tokens[1];
        }
    }

    // 3-field line (label opcode operand)
    else if (tokens.size() >= 3) {
        l.label = tokens[0];
        l.opcode = tokens[1];
        l.operand = tokens[2];
    }

    return l;
}

// PASS 1 (SYMBOL TABLE + ADDRESS ASSIGNMENT)
// This will do:
// Build SYMTAB
// Assign LOCCTR addresses
// Collect literals
// Determine program length
void pass1(string filename, vector<Line> &lines,
           map<string,int> &SYMTAB,
           int &startAddr, int &programLength,
           string &programName) {

    ifstream file(filename);
    string line;
    int locctr = 0;

    while (getline(file, line)) {
        if (line.empty() || line[0] == '.') continue;

        Line l = parseLine(line);

        // START directive defines program entry address
        if (l.opcode == "START") {
            startAddr = stoi(l.operand, nullptr, 16);
            locctr = startAddr;
            l.address = locctr;
            programName = l.label;
        } else {
            l.address = locctr;

            // Add label to symbol table
            if (!l.label.empty())
                SYMTAB[l.label] = locctr;

            string op = l.opcode;
            bool format4 = false;

            // Detect format 4 instructions (+ opcode)
            if (!op.empty() && op[0] == '+') {
                format4 = true;
                op = op.substr(1);
            }

            // Detect literals during parsing
            if (!l.operand.empty() && l.operand[0] == '=')
                addLiteral(l.operand);

            // Update LOCCTR based on instruction type
            if (OPTAB.count(op))
                locctr += format4 ? 4 : 3;
            else if (OPTAB2.count(op))
                locctr += 2;
            else if (op == "WORD")
                locctr += 3;
            else if (op == "RESW")
                locctr += 3 * stoi(l.operand);
            else if (op == "RESB")
                locctr += stoi(l.operand);
            else if (op == "BYTE")
                locctr += (l.operand[0]=='C') ?
                          l.operand.length()-3 :
                          (l.operand.length()-3)/2;

            // LTORG forces immediate literal placement
            else if (op == "LTORG") {
                lines.push_back(l);
                assignLiterals(locctr, lines, lines.size());
                continue;
            }
        }

        lines.push_back(l);
    }

    // Final literal placement at END
    assignLiterals(locctr, lines, lines.size());

    // Compute program length for SYMTAB output
    programLength = locctr - startAddr;
}

// PASS 2 (OBJECT CODE GENERATION)
// This  will do:
// Generate machine code
// Resolve symbols/literals
// Apply addressing modes
void pass2(string filename, vector<Line> &lines,
           map<string,int> &SYMTAB) {

    ofstream out(filename.substr(0, filename.find('.')) + ".l");

    int base = 0;

    for (auto &l : lines) {
        string obj = "";
        string op = l.opcode;

        // Literal output line (* =C'EOF')
        if (l.label == "*") {
            obj = literalObjectCode(op);
        }

        // RSUB has fixed object code
        else if (op == "RSUB") {
            obj = "4F0000";
        }

        // FORMAT 3 / 4 instruction handling
        else if (OPTAB.count(op) || (op[0]=='+' && OPTAB.count(op.substr(1)))) {

            bool format4 = false;

            if (op[0] == '+') {
                format4 = true;
                op = op.substr(1);
            }

            int opcode = OPTAB[op];

            // Addressing mode flags
            int n=1,i_flag=1,x=0,b=0,p=0,e=format4;

            string operand = l.operand;

            // Immediate addressing (#)
            if (!operand.empty() && operand[0] == '#') {
                n=0; i_flag=1;
                operand = operand.substr(1);
            }

            // Indirect addressing (@)
            else if (!operand.empty() && operand[0] == '@') {
                n=1; i_flag=0;
                operand = operand.substr(1);
            }

            // Indexed addressing (,X)
            if (operand.find(",X") != string::npos) {
                x=1;
                operand = operand.substr(0, operand.find(",X"));
            }

            // Resolve literal to address
            if (!operand.empty() && operand[0] == '=') {
                for (auto &lit : LITTAB)
                    if (lit.name == operand)
                        operand = to_string(lit.address);
            }

            int target = operand.empty() ? 0 : stoi(operand);
            int disp;

            // PC-relative or BASE-relative calculation
            if (format4) {
                disp = target;
            } else {
                int nextAddr = l.address + 3;
                disp = target - nextAddr;

                if (disp >= -2048 && disp <= 2047)
                    p = 1;
                else {
                    disp = target - base;
                    b = 1;
                }
            }

            int code = (opcode & 0xFC) | (n<<1) | i_flag;
            int flags = (x<<3)|(b<<2)|(p<<1)|e;

            stringstream ss;
            ss << hex << uppercase << setfill('0');

            if (format4)
                ss << setw(2) << code << flags << setw(5) << (disp & 0xFFFFF);
            else
                ss << setw(2) << code << flags << setw(3) << (disp & 0xFFF);

            obj = ss.str();
        }

        // FORMAT 2 instructions register-based
        else if (OPTAB2.count(op)) {
            int opcode = OPTAB2[op];
            string r1, r2;
            stringstream ss(l.operand);
            getline(ss, r1, ',');
            getline(ss, r2);

            stringstream outcode;
            outcode << hex << uppercase << setw(2) << setfill('0') << opcode
                    << REG[r1] << (r2.empty()?0:REG[r2]);

            obj = outcode.str();
        }

        // BYTE directive handling
        else if (op == "BYTE") {
            if (l.operand[0] == 'C') {
                for (int i = 2; i < l.operand.size()-1; i++) {
                    stringstream ss;
                    ss << hex << uppercase << setw(2) << setfill('0')
                       << (int)l.operand[i];
                    obj += ss.str();
                }
            } else {
                obj = l.operand.substr(2, l.operand.size()-3);
            }
        }

        // WORD directive handling
        else if (op == "WORD") {
            stringstream ss;
            ss << hex << uppercase << setw(6) << setfill('0')
               << stoi(l.operand);
            obj = ss.str();
        }

        // BASE directive sets base register
        else if (op == "BASE") {
            base = SYMTAB[l.operand];
        }

        // Final listing file output format
        out << setw(4) << setfill('0') << hex << uppercase << l.address << "    "
            << setw(8) << left << l.label
            << setw(8) << left << l.opcode
            << setw(10) << left << l.operand
            << obj << endl;
    }

    out.close();
}

void writeSymtab(string filename,
                 map<string,int> &SYMTAB,
                 int startAddr,
                 int programLength,
                 string programName) {

    string outFile = filename.substr(0, filename.find('.')) + ".st";
    ofstream out(outFile);

    if (!out) {
        cerr << "Error: Could not create SYMTAB file for " << filename << endl;
        return;
    }

    // Extract program name (use filename without extension)
    string progName = programName;
    if (progName.empty())
        progName = filename.substr(0, filename.find('.')); // fallback

    // HEADER
    out << "CSect   Symbol  Value   LENGTH  Flags:\n";
    out << "--------------------------------------\n";

    // Control section (program)
    out << left << setw(8) << progName
        << setw(8) << ""
        << setw(8) << uppercase << hex << setfill('0') << startAddr
        << "  "
        << setw(6) << programLength
        << "                  \n";

    // SYMBOL TABLE
    for (auto &entry : SYMTAB) {
        out << setw(8) << ""
            << setw(8) << entry.first
            << setw(8) << uppercase << hex << setfill('0') << entry.second
            << "          R\n";   // R = relocatable
    }

    // LITERAL TABLE
    out << "\nLiteral Table \n";
    out << "Name  Operand   Address  Length:\n";
    out << "--------------------------------\n";

    for (auto &lit : LITTAB) {

        string name = lit.name;

        // Remove '=' and type prefix for display
        // =C'EOF' → EOF
        string displayName = name.substr(3, name.length() - 4);

        string operand = literalObjectCode(name);

        out << setw(6) << left << displayName
            << setw(10) << operand
            << setw(8) << uppercase << hex << setfill('0') << lit.address
            << setw(6) << dec << lit.length
            << endl;
    }

    out.close();
}

// Main function processes command-line arguments and runs assembler passes
int main(int argc, char *argv[]) {
    string programName = "";
    // No input files case
    if (argc < 2) {
        cout << "Usage: lxe <file1.sic> <file2.sic> ...\n";
        cout << "Error: No input files provided. Program terminated.\n";
        return 1;
    }

    // Process each file independently
    for (int i = 1; i < argc; i++) {
        string filename = argv[i];

        // Check if file exists BEFORE running passes
        ifstream infile(filename);
        if (!infile) {
            cerr << "Error: Unable to open file \"" << filename << "\". Skipping.\n";
            continue;
        }
        infile.close();

        // Reset all structures per file
        vector<Line> lines;
        map<string, int> SYMTAB;
        int startAddr = 0;
        int programLength = 0;

        LITTAB.clear();

        cout << "Assembling: " << filename << "...\n";

        // Run assembler passes
        pass1(filename, lines, SYMTAB, startAddr, programLength, programName);

        // If file had no valid lines, skip
        if (lines.empty()) {
            cerr << "Warning: File \"" << filename << "\" is empty or invalid. Skipping.\n";
            continue;
        }

        pass2(filename, lines, SYMTAB);
        writeSymtab(filename, SYMTAB, startAddr, programLength, programName);

        cout << "Output files generated:\n";
        cout << "  - " << filename.substr(0, filename.find('.')) << ".l\n";
        cout << "  - " << filename.substr(0, filename.find('.')) << ".st\n";
        cout << "Done.\n\n";
    }

    return 0;
}
