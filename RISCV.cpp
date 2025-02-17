#include <iostream>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>

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




int ReadMem(int pc, int datatype, int sign)
{

    int mem;
    if ((datatype == byte) && (sign==negative))
       mem=MemorySpace[pc]>>7?MemorySpace[pc]|(0xffffff00):MemorySpace[pc];

    else if (datatype == byte)
       mem=MemorySpace[pc];

    else if ((datatype==halfword) && (sign==negative))  
       mem= (MemorySpace[pc+1]>>7) ?  (MemorySpace[pc]|(MemorySpace[pc+1]<<8)|(0xffff0000)) : (MemorySpace[pc]|(MemorySpace[pc+1]<<8));

    else if (datatype == halfword)
       mem=MemorySpace[pc]|MemorySpace[pc+1]<<8; 

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

    CurrentInstr = ReadMem(pc, word, positive);
    #ifdef debug
    std::cout << "Current Instruction: " <<std::hex<< CurrentInstr << std::endl;
    #endif
}

// void Decode();
// void Execute();



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
        // Decode();
        // Execute();
        pc = pc + 4;

    }

}
