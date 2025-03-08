#include <iostream>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>
#include "defines.h"
#define UNSIGNED 0
#define SIGNED 1

#define StartAddress 0 
#define StackAddress 1<<16 

uint8_t MemorySpace[1 << 16] = {};
uint32_t x[32] = {};
int mode;
std::string MemoryImage;
int CurrentInstr;
bool BranchTaken;

uint8_t opcode;
uint8_t rd;
uint8_t funct3;
uint8_t rs1;
uint8_t rs2;
uint8_t funct7;
uint32_t imm;
uint32_t pc;





void Print(int mode)
{
    static const char* regNames[32] = {
        "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
        "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
        "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
        "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
    };


    if (mode == Verbose)
    {   
        std::cout << "Program Counter : 0x" << std::hex << pc << std::endl;
        std::cout << "Current Instruction : 0x" << std::hex << CurrentInstr << std::endl;

        for (int i = 0; i < 32; i++)
        {
            std::cout << std::dec << "x[" << i << "] (" 
                      << regNames[i] << ") = 0x" 
                      << std::hex << x[i] << std::endl;
        }
    }
    else
    {
        std::cout << "Program Counter : 0x" << std::hex << pc << std::endl;
        std::cout << "Current Instruction : 0x" << std::hex << pc << std::endl;

        for (int i = 0; i < 32; i++)
        {
            std::cout << std::dec << "x[" << i << "] (" 
                      << regNames[i] << ") =0x" 
                      << std::hex << x[i] << std::endl;
        }
    }
            std::cout << "---------------------------------------------" << std::endl;


}





    int ReadMem(int datatype, int sign, uint32_t location)
    {

        int mem;
        if ((datatype == byte) && (sign))
          mem = MemorySpace[location] >> 7 ? MemorySpace[location] | (0xffffff00) : MemorySpace[location];

        else if(datatype == byte)
          mem = MemorySpace[location];

        else if ((datatype == halfword) && (sign))  
          mem= (MemorySpace[location + 1]>>7) ?  (MemorySpace[location]|(MemorySpace[location+1]<<8)|(0xffff0000)) : (MemorySpace[location]|(MemorySpace[location+1]<<8));

        else if (datatype == halfword ) 
          mem= (MemorySpace[location]|(MemorySpace[location+1]<<8));

        else if (datatype == word)
          mem = MemorySpace[location] | MemorySpace[location+1]<<8 | MemorySpace[location+2]<<16 | MemorySpace[location+3]<<24;

        return mem; 

    }




    void StoreMem(uint32_t location, int bytes, uint32_t regst)
    {
        for(int i=0;i<bytes;i++)
        {
            MemorySpace[location+i]=regst>>(8*i); 
        }
        

    }

    void Fetch()
    {

        CurrentInstr = ReadMem(word, UNSIGNED, pc);
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


        }

        else if ((opcode == I) || (opcode == I_LOADS) || (opcode == ECALL)) 
        {

        rd = (CurrentInstr >> 7) & 0x1F;     
        funct3 = (CurrentInstr >> 12) & 0x7;   
        rs1 = (CurrentInstr >> 15) & 0x1F;     

        imm = (CurrentInstr >> 20) & 0xFFF;    
        if (imm & 0x800) 
        {  
            imm |= 0xFFFFF000; 
        }

        }

        else if(opcode == S)
        {

            funct3 = (CurrentInstr>>12) & 0x7;
            rs1 = (CurrentInstr>>15) & 0x1f;
            rs2 = (CurrentInstr>>20) & 0x1f;
            imm = (CurrentInstr>>7 & 0x1f) | (( CurrentInstr >> 25 & 0x7f) <<5);
            imm = (imm>>11) ? imm | 0xfffff000 : imm;
        }

        else if (opcode == B)
        {
        imm = ((CurrentInstr >> 31) & 0x1) << 12 | ((CurrentInstr >> 7) & 0x1) << 11 |  ((CurrentInstr >> 25) & 0x3F) << 5 | ((CurrentInstr >> 8) & 0xF) << 1;
        rs1 = (CurrentInstr >> 15) & 0x1F;     
        rs2 = (CurrentInstr >> 20) & 0x1F;
        funct3 = (CurrentInstr >> 12) & 0x7;
        if (imm & 0x1000) 
        {
            imm |= 0xFFFFE000; 
        }
        }

        else if (opcode == U_LUI || opcode == U_AUIPC )
        {
        rd = (CurrentInstr >> 7) & 0x1F;
        imm = CurrentInstr & 0xFFFFF000;

        }

        else if(opcode == J ) 
        {

        rd = (CurrentInstr >> 7) & 0x1F;
        imm = ((CurrentInstr >> 31) & 0x1) << 20 | ((CurrentInstr >> 21) & 0x3FF) << 1 | ((CurrentInstr >> 20) & 0x1) << 11 | ((CurrentInstr >> 12) & 0xFF) << 12;



        }
        #ifdef debug
        std::cout << "funct3: " <<std::dec<< (int)funct3 << std::endl;
        std::cout << "funct7: " <<std::dec<< (int)funct7 << std::endl;
        std::cout << "rs1: " <<std::dec<< (int)rs1 << std::endl;
        std::cout << "rs2: " <<std::dec<< (int)rs2 << std::endl;
        std::cout << "rd: " <<std::dec<< (int)rd << std::endl;
        std::cout << "imm: " <<std::dec<<imm << std::endl;
        #endif



    }

    void Execute()
    {
        switch(opcode)
        {
            case R: 
                    switch(funct3)
                    {
                        case 0b000:
                                    switch(funct7)
                                    {
                                        case 0b0000000:  x[rd] = x[rs1] + x[rs2]; break;                   // add
                                        case 0b0100000:  x[rd] = x[rs1] - x[rs2]; break;                   // sub
                                    }
                                    break;
                        case 0b001: x[rd] = x[rs1] << x[rs2]; break;                              // sll
                        case 0b010: x[rd] = (x[rs1] < x[rs2]) ? 1 : 0; break;                     // slt
                        case 0b011: x[rd] = (uint32_t)x[rs1] < (uint32_t)x[rs2] ? 1 : 0; break;   // sltu
                        case 0b100: x[rd] = x[rs1] ^ x[rs2]; break;                               // xor
                                    switch(funct7)
                                    {
                                        case 0b0000000:  x[rd] = x[rs1] >> x[rs2]; break;                  // srl
                                        case 0b0100000:  x[rd] = (int32_t)x[rs1] >> x[rs2]; break;         // sra
                                    }
                                    break;
                        case 0b110: x[rd] = x[rs1] | x[rs2]; break;                               // or
                        case 0b111: x[rd] = x[rs1] & x[rs2]; break;                               // and
                    }
                    break;
            
            case I:
                    switch(funct3)
                    {

                        case 0b000: x[rd] = x[rs1] + imm ; break;                                  // addi

                                                                      
                        case 0b010: x[rd] = (x[rs1] < imm) ? 1 : 0; break;                         // slti
                        case 0b011: x[rd] = (uint32_t)x[rs1] < (uint32_t)imm ? 1 : 0; break;       // sltiu
                        case 0b100: x[rd] = x[rs1] ^ imm; break;                                   // xori
                        case 0b110: x[rd] = x[rs1] | imm; break;                                   // ori
                        case 0b111: x[rd] = x[rs1] & imm; break;                                   // andi
                        case 0b001: x[rd] = x[rs1] << (imm & 0x1F); break;                         // slli
                        case 0b101:
                                    switch(funct7)
                                    {
                                        case 0b0000000: x[rd] = x[rs1] >> (imm & 0x1F); break;             // srli
                                        case 0b0100000: x[rd] = (int32_t)x[rs1] >> (imm & 0x1F); break;    // srai
                                    }
                                    break;
                        //case 0b000: x[rd] = pc + 4; pc = x[rs1] + imm; break;                    // jalr
                    }
                    break;
            case ECALL: std::cout<<"ECALL Detected"<<std::endl; 
                       switch(x[17])
                       {
                         case 63:   {
                            int fdr = x[10]; 
                            uint32_t addrr = x[11]; 
                            int sizer = x[12];
                            if (fdr == 0) 
                            { 
                                std::cin.read(reinterpret_cast<char*>(&MemorySpace[addrr]), sizer);
                            }
                            x[10] = std::cin.gcount(); 
                            #ifdef debug
                            for(int i=0; i<x[12]; i++)
                            {
                                std::cout<<"MemorySpace["<<std::dec<<(0x200 + i)<<"] = "<<std::dec<<static_cast<char>(MemorySpace[0x200+i])<<std::endl;
                            }
                            #endif
                            break;}
                        case 64: {
                            int fdw = x[10]; 
                            uint32_t addrw = x[11]; 
                            int sizew = x[12];
                            if (fdw == 1) 
                            { 
                                std::cout.write(reinterpret_cast<char*>(&MemorySpace[addrw]), sizew)<<std::endl;
                                std::cout.flush();
                            }
                            x[10] = sizew;
                            break;}
                        } break;

                
            
            case S:
                    switch(funct3)
                    {
                        case 0b000: std::cout<<"Store Byte detected"<<std::endl;
                                    StoreMem(x[rs1]+imm, byte, x[rs2]); break;                  // sb

                        case 0b001: std::cout<<"Store Half Word detected"<<std::endl;
                                    StoreMem(x[rs1]+imm, halfword, x[rs2]); break;              // sh

                        case 0b010: std::cout<<"Store Word detected"<<std::endl;
                                    StoreMem(x[rs1]+imm, word, x[rs2]);   break ;              // sw

                    }   
                    break;
            
            case B:
                    switch(funct3)
                    {
                        case 0b000: if (x[rs1] == x[rs2])                                           // beq
                                    {
                                        std::cout<<"BEQ detected"<<std::endl;
                                        BranchTaken = true;
                                        pc += imm;
                                    }   break;                                                        
                        case 0b001: if (x[rs1] != x[rs2])                                           // bne
                                    {
                                        std::cout<<"BNE detected"<<std::endl;
                                        BranchTaken = true;
                                        pc += imm; 
                                    }   break;                                                        
                        case 0b100: if ((int32_t)x[rs1] < (int32_t)x[rs2])                          // blt
                                    {   
                                        std::cout<<"BLT detected"<<std::endl;
                                        BranchTaken = true;
                                        pc += imm; 
                                    }   break;                                                            
                        case 0b101: if ((int32_t)x[rs1] >= (int32_t)x[rs2])                         // bge
                                    {
                                        std::cout<<"BGE detected"<<std::endl;
                                        BranchTaken = true;
                                        pc += imm; 
                                    }   break;                         
                        case 0b110: if (x[rs1] < x[rs2])                                            // bltu
                                    {
                                        std::cout<<"BLTU detected"<<std::endl;
                                        BranchTaken = true;
                                        pc += imm;    
                                    }   break;
                        case 0b111: if (x[rs1] >=x[rs2])                                            // bgeu
                                    {
                                        std::cout<<"BGEU detected"<<std::endl;
                                        BranchTaken = true;
                                        pc += imm; 
                                    }   break;     
                    } break;
            
            case U_LUI   :  std::cout<<" LUI Detected "<<std::endl;
                            x[rd] = imm; break; 

            case U_AUIPC :  std::cout<<" AUIPC Detected "<<std::endl;
                            x[rd] = pc + imm; break;

            
            case J       :  std::cout<<" JAL Detected "<<std::endl;
                            pc = pc + imm; 
                            x[rd] = pc + 4; break;
                            
            case I_LOADS : 
                            switch(funct3)
                                    { 
                                        case 0b000: std::cout<<"Load Byte Detected"<<std::endl;
                                                    x[rd] = ReadMem(byte, SIGNED, imm + x[rs1]); break;       // Load Byte

                                        case 0b001: std::cout<<"Load Half Word Detected"<<std::endl;
                                                    x[rd] = ReadMem(halfword, SIGNED,imm +  x[rs1]); break;   // Load Halfword 

                                        case 0b010: std::cout<<"Load Word Detected"<<std::endl; 
                                                    x[rd] = ReadMem(word, UNSIGNED, imm +  x[rs1]); break;    // Load Word

                                        case 0b100: std::cout<<"Load Byte Unsigned Detected"<<std::endl;
                                                    x[rd] = ReadMem(byte, UNSIGNED, imm + x[rs1] ); break;    // Load Byte Unsigned

                                        case 0b101: std::cout<<"Load HalfWord Unsigned Detected"<<std::endl;
                                                    x[rd] = ReadMem(halfword, UNSIGNED, imm + x[rs1]);break;  // Load Halfword Unsigned
                                    } break;
                            }
                x[0]=0;
                Print(mode);


    }




    int main(int argc, char* argv[]) 
    {
        int ProgramSize = 0;
        pc = StartAddress;
        x[2] = StackAddress;


        switch(argc)
        {
            case 5: MemoryImage = argv[1];
                    pc = std::stoi(argv[2]);
                    x[2] = std::stoi(argv[3]);
                    mode = std::stoi(argv[4]);
                    break;
            case 4: MemoryImage = argv[1];
                    pc = std::stoi(argv[2]);
                    x[2] = std::stoi(argv[3]);
                    break;
            case 2: MemoryImage = argv[1];
                    break;
            case 3: MemoryImage = argv[1];
                    mode = std::stoi(argv[2]);
                    break;
        }



        #ifdef debug
        std::cout << "StartAddress: " << StartAddress << "\n";
        std::cout << "StackAddress: " << StackAddress << "\n";
        std::cout << "Stack Pointer: "  << x[2] << "\n";
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
        
        std::cout<< "-----------------RISC-V Simulator---------------------------"<<std::endl;
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
            if(CurrentInstr == 0 || CurrentInstr == 0x00008067)
            {
                Print(mode);
                std::cout<<"------------------Simulation Ended here---------------------"<<std::endl;
                break;
            }
            Decode();
            Execute();

            if(!BranchTaken)
            {
                pc = pc + 4;

            }
            BranchTaken = false;


        }

    }
