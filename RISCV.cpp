#include <iostream>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>

#define R 0x33
#define I 0x3|0x13
#define S 0x23
#define B 0x63
#define J 0x6f
#define U 0x37
#define word 4
#define halfword 2
#define byte 1
#define negative 1
#define positive 0

#define Verbose 1
#define Silent 0
#define DefaultFile "prog.mem"

uint8_t MemorySpace[1 << 16] = {};
uint32_t StartAddress = 0;
uint32_t StackAddress = 1 << 16;
uint32_t x[32] = {};
uint32_t pc=0;
int mode;
std::string MemoryImage;
int CurrentInstr;

uint8_t opcode;
uint8_t rd;
uint8_t funct3;
uint8_t rs1;
uint8_t rs2;
uint8_t funct7;
uint32_t imm;


    int ReadMem(int pc, int datatype)
    {

        int mem;
        if (datatype == byte)
        mem=MemorySpace[pc]>>7?MemorySpace[pc]|(0xffffff00):MemorySpace[pc];


        else if (datatype==halfword)  
        mem= (MemorySpace[pc+1]>>7) ?  (MemorySpace[pc]|(MemorySpace[pc+1]<<8)|(0xffff0000)) : (MemorySpace[pc]|(MemorySpace[pc+1]<<8));


        else if (datatype ==word)
        mem=MemorySpace[pc] | MemorySpace[pc+1]<<8 | MemorySpace[pc+2]<<16 | MemorySpace[pc+3]<<24;

        return mem; 

    }


    void StoreMem(int location, int bytes, int regst)
    {

        for(int i=0;i<bytes;i++)
        {
            MemorySpace[location+i]=regst>>(8*i); 
        }
        

    }


    void Fetch()
    {

        CurrentInstr = ReadMem(pc, word);
        #ifdef debug
        std::cout << "Current Instruction: " <<std::hex<< CurrentInstr << std::endl;
        #endif
    }

    void Decode()
    {

        opcode = CurrentInstr & 0x7f;
        if(opcode == R)
        {
            rd = (CurrentInstr >> 7) & 0x1f;
            funct3 = (CurrentInstr >> 12) & 0x7;
            rs1 = (CurrentInstr >> 15) & 0x1f;
            rs2 = (CurrentInstr >> 20) & 0x1f;
            funct7 = CurrentInstr >> 25;


            #ifdef debug
            std::cout << "rd: " <<std::hex<< (int)rd << std::endl;
            std::cout << "funct3: " <<std::hex<< (int)funct3 << std::endl;
            std::cout << "rs1: " <<std::hex<< (int)rs1 << std::endl;
            std::cout << "rs2: " <<std::hex<< (int)rs2 << std::endl;
            std::cout << "funct7: " <<std::hex<<(int) funct7 << std::endl;
            #endif
        }

        else if (opcode = I) // I type
        {

        rd = (CurrentInstr >> 7) & 0x1F;     
        funct3 = (CurrentInstr >> 12) & 0x7;   
        rs1 = (CurrentInstr >> 15) & 0x1F;     

        imm = (CurrentInstr >> 20) & 0xFFF;    
        if (imm & 0x800) {  
            imm |= 0xFFFFF000; //32 bits ext
        }

        }

        else if(opcode == S)
        {
            funct3 = (CurrentInstr>>12) & 0x7;
            rs1 = (CurrentInstr>>15) & 0x1f;
            rs2 = (CurrentInstr>>20) & 0x1f;
            imm = (CurrentInstr>>7 & 0x1f) | (( CurrentInstr >> 25 & 0x7f) <<5);
            imm = (imm>>11) ? imm | 0xffff0000 : imm;
            #ifdef debug
            std::cout << "funct3: " <<std::hex<< (int)funct3 << std::endl;
            std::cout << "rs1: " <<std::hex<< (int)rs1 << std::endl;
            std::cout << "rs2: " <<std::hex<< (int)rs2 << std::endl;
            std::cout << "imm: " <<std::hex<<imm << std::endl;
            #endif
        }

        else if (opcode == B) // B Type
        {
        imm = ((CurrentInstr >> 31) & 0x1) << 12 | ((CurrentInstr >> 7) & 0x1) << 11 |  ((CurrentInstr >> 25) & 0x3F) << 5 | ((CurrentInstr >> 8) & 0xF) << 1;
        rs1 = (CurrentInstr >> 15) & 0x1F;     
        rs2 = (CurrentInstr >> 20) & 0x1F;
        funct3 = (CurrentInstr >> 12) & 0x7;
        if (imm & 0x1000) 
        {
            imm |= 0xFFFFE000;  //32 bits ext
        }
        }

        else if (opcode == U )  // U type 
        {
        rd = (CurrentInstr >> 7) & 0x1F;
        imm = CurrentInstr & 0xFFFFF000;

        }

        else if(opcode == J ) // J Type
        {

        rd = (CurrentInstr >> 7) & 0x1F;
        imm = ((CurrentInstr >> 31) & 0x1) << 20 | ((CurrentInstr >> 21) & 0x3FF) << 1 | ((CurrentInstr >> 20) & 0x1) << 11 | ((CurrentInstr >> 12) & 0xFF) << 12;  

        if (imm & 0x100000) {
            imm |= 0xFFE00000;  // Extend the sign to 32 bits
        }


        }



    }
//void Execute();



    int main(int argc, char* argv[]) 
    {
        int ProgramSize = 0;

        switch(argc)
        {
            case 4: MemoryImage = argv[1];
                    StartAddress = std::stoi(argv[2]);
                    StackAddress = std::stoi(argv[3]);
                    // mode = std::stoi(argv[3]);
                    break;
            case 3: MemoryImage = argv[1];
                    mode = std::stoi(argv[2]);
                    break;
            case 1: MemoryImage = DefaultFile;

        }

        #ifdef debug
        std::cout << "StartAddress: " << StartAddress << "\n";
        std::cout << "StackAddress: " << StackAddress << "\n";
        #endif

        std::ifstream file(MemoryImage);
        if (!file) 
        {
            std::cerr << "Error: Could not open file!" << std::endl;
            return 0;
        }

        std::string line;
        while (std::getline(file, line)) 
        {
            std::istringstream stream(line);
            uint32_t address, value;
            char colon;
            std::string data;

            if (stream >> std::hex >> address >> colon >> data) 
            {
                #ifdef debug
                std::cout << "Address: 0x" << std::hex << address
                        << " => data: " << data << std::endl;
                #endif

                sscanf(data.c_str(), "%x", &value);
                ProgramSize += 4; 


                StoreMem(address, (data.size())/2, value );    //Storing into Memory Space

            }
        }
        file.close();

        std::cout << "ProgramSize: " << ProgramSize << "\n";

        #ifdef debug
        std::cout << "\nMemory Contents:\n";
        for (int i = 0; i < ProgramSize; i++) 
        {
            std::cout << std::hex << i << "  :" << std::hex << (int)MemorySpace[i] << std::endl;
        }
        #endif



        while(true)
        {
            Fetch();
            if(CurrentInstr==0)
            {
                break;
            }
            Decode();
            // Execute();
            pc = pc + 4;

        }

    }
