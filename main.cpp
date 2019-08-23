#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

using namespace std;
typedef unsigned short uint16_t;

enum specialRegisters
{
    IP = 7,
    SP,
    BP
};

enum InstructionSet
{
    BR,
    ADD,
    LD,
    ST,
    JSR,
    AND,
    LDR,
    STR,
    RTI,
    NOT,
    LDI,
    STI,
    JMP,
    RESERVED, //1101
    LEA,
    TRAP
};

enum BranchInstructions
{
    BRn,
    BRz,
    BRp,
    BRzp,
    BRnp,
    BRnz,
    BRnzp,
};

enum REGISTERS
{
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    R8
};

map<string, InstructionSet> instructionMap;
map<string, REGISTERS> registerMap;
map<string, BranchInstructions> branchMap;
vector<uint16_t> instructions;

uint16_t registers[10];
uint16_t memory[1 << 16];

void init()
{
    instructionMap.insert({"BR", BR});
    instructionMap.insert({"BRp", BR});
    instructionMap.insert({"BRn", BR});
    instructionMap.insert({"BRz", BR});
    instructionMap.insert({"BRnp", BR});
    instructionMap.insert({"BRnz", BR});
    instructionMap.insert({"BRzp", BR});
    instructionMap.insert({"BRnzp", BR});
    instructionMap.insert({"ADD", ADD});
    instructionMap.insert({"LD", LD});
    instructionMap.insert({"ST", ST});
    instructionMap.insert({"JSR", JSR});
    instructionMap.insert({"AND", AND});
    instructionMap.insert({"LDR", LDR});
    instructionMap.insert({"STR", STR});
    instructionMap.insert({"RTI", RTI});
    instructionMap.insert({"NOT", NOT});
    instructionMap.insert({"LDI", LDI});
    instructionMap.insert({"STI", STI});
    instructionMap.insert({"JMP", JMP});
    instructionMap.insert({"LEA", LEA});
    instructionMap.insert({"TRAP", TRAP});

    registerMap.insert({"R1", R1});
    registerMap.insert({"R2", R2});
    registerMap.insert({"R3", R3});
    registerMap.insert({"R4", R4});
    registerMap.insert({"R5", R5});
    registerMap.insert({"R6", R6});
    registerMap.insert({"R7", R7});
    registerMap.insert({"R8", R8});

    branchMap.insert({"BRn", BRn});
    branchMap.insert({"BRz", BRz});
    branchMap.insert({"BRp", BRp});
    branchMap.insert({"BRnz", BRnz});
    branchMap.insert({"BRnp", BRnp});
    branchMap.insert({"BRzp", BRzp});
    branchMap.insert({"BRnzp", BRnzp});
}

void byteCode(vector<string> &tokens)
{
    InstructionSet opCode = instructionMap[tokens[0]];
    switch (opCode)
    {
    case ADD:
    {
        uint16_t instruction = 0;
        instruction += ADD << 12;

        REGISTERS des = registerMap[tokens[1]];
        REGISTERS sr1 = registerMap[tokens[2]];

        instruction += des << 9;
        instruction += sr1 << 6;

        if (tokens[3][0] == '#')
        {
            instruction += 1 << 5;
            int imm = stoi(tokens[3].substr(1, tokens[3].size()));
            if (imm < 0)
            {
                unsigned char tmp = 0;
                tmp += -imm;
                unsigned char mask = 255 >> 3;
                tmp = mask & (~tmp + 1);
                instruction += tmp;
            }
            else
            {
                instruction += imm;
            }
        }
        else
        {
            REGISTERS sr2 = registerMap[tokens[3]];
            instruction += sr2;
        }

        instructions.push_back(instruction);

        break;
    }
    case BR:
    {
        string label = tokens[1];
        if (tokens[0] == "BR")
        {
        }
        else
        {
            BranchInstructions branchInst = branchMap[tokens[0]];
            switch (branchInst)
            {
            case BRn:
            {
                break;
            }
            case BRz:
            {
                break;
            }
            case BRp:
            {
                break;
            }
            case BRnp:
            {
                break;
            }
            case BRnz:
            {
                break;
            }
            case BRzp:
            {
                break;
            }
            case BRnzp:
            {
                break;
            }

            default:
                break;
            }
        }

        break;
    }
    default:
        break;
    }
}

int main()
{
    init();
    uint16_t a;
    std::ifstream file("sample.asm");
    if (file.is_open())
    {
        std::string line;
        while (getline(file, line))
        {
            vector<string> tokens;
            string token = "";
            for (int i = 0; i < line.size(); i++)
            {

                char curchar = line[i];

                if (curchar == '\t' || curchar == ' ' || curchar == ',')
                {
                    if (token != "")
                    {
                        tokens.push_back(token);
                        token = "";
                    }
                    continue;
                }
                else
                {
                    token += curchar;
                }
            }
            if (token != "")
            {
                tokens.push_back(token);
            }
            byteCode(tokens);
        }
        file.close();
    }

    ofstream out("numbers", ios::out | ios::binary);
    if (!out)
    {
        cout << "Cannot open file.";
        return 1;
    }

    out.write((char *)&instructions[0], instructions.size() * sizeof(uint16_t));

    out.close();

    return 0;
}
