#include <iostream>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>
#include "defines.h"
#define StartAddress 0 
#define StackAddress 1<<16 

uint8_t MemorySpace[1 << 16] = {};
uint32_t x[32] = {};
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
uint32_t pc;





    void Print(uint32_t pc, int mode)
    {
            if(mode==Verbose)
            {
                std::cout<<std::hex<<pc<<std::endl;
                for(int i=0; i<32;i++)
                {
                    std::cout<<std::dec<<"x["<<i<<"]"<<std::hex<<x[i]<<std::endl; 
                }

                std::cout<<std::hex<<CurrentInstr<<std::endl;
            }

            else 
            {
                std::cout<<std::hex<<pc<<std::endl;
                for(int i=0; i<32;i++)
                {
                    std::cout<<std::dec<<"x["<<i<<"]"<<std::hex<<x[i]<<std::endl; 
                }
    
            }

    }


    int ReadMem(uint32_t pc, int datatype)
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


    void Fetch(uint32_t pc)
    {

        CurrentInstr = ReadMem(pc, word);
        #ifdef debug
        std::cout << "Current Instruction: " <<std::hex<< CurrentInstr << std::endl;
        #endif
    }

    void Decode(uint32_t pc)
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
            std::cout << "rd: " <<std::dec<< (int)rd << std::endl;
            std::cout << "funct3: " <<std::hex<< (int)funct3 << std::endl;
            std::cout << "rs1: " <<std::dec<< (int)rs1 << std::endl;
            std::cout << "rs2: " <<std::dec<< (int)rs2 << std::endl;
            std::cout << "funct7: " <<std::hex<<(int) funct7 << std::endl;
            #endif
        }

        else if (opcode == I) 
        {

        rd = (CurrentInstr >> 7) & 0x1F;     
        funct3 = (CurrentInstr >> 12) & 0x7;   
        rs1 = (CurrentInstr >> 15) & 0x1F;     

        imm = (CurrentInstr >> 20) & 0xFFF;    
        if (imm & 0x800) 
        {  
            imm |= 0xFFFFF000; 
        }
        #ifdef debug
        std::cout<<"IMM: "<<std::hex<<imm<<std::endl;
        std::cout << "rd: " <<std::dec<< (int)rd << std::endl;
        std::cout << "funct3: " <<std::hex<< (int)funct3 << std::endl;
        std::cout << "rs1: " <<std::dec<< (int)rs1 << std::endl;
        #endif

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

        else if (opcode == U )
        {
        rd = (CurrentInstr >> 7) & 0x1F;
        imm = CurrentInstr & 0xFFFFF000;

        }

        else if(opcode == J ) 
        {

        rd = (CurrentInstr >> 7) & 0x1F;
        imm = ((CurrentInstr >> 31) & 0x1) << 20 | ((CurrentInstr >> 21) & 0x3FF) << 1 | ((CurrentInstr >> 20) & 0x1) << 11 | ((CurrentInstr >> 12) & 0xFF) << 12;  

        if (imm & 0x100000) {
        }


        }



    }

    void Execute(uint32_t pc)
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
                    case 0b001:  x[rd] = x[rs1] << x[rs2]; break;                              // sll
                    case 0b010:  x[rd] = (x[rs1] < x[rs2]) ? 1 : 0; break;                     // slt
                    case 0b011:  x[rd] = (uint32_t)x[rs1] < (uint32_t)x[rs2] ? 1 : 0; break;   // sltu
                    case 0b100:  x[rd] = x[rs1] ^ x[rs2]; break;                               // xor
                        switch(funct7)
                        {
                            case 0b0000000:  x[rd] = x[rs1] >> x[rs2]; break;                  // srl
                            case 0b0100000:  x[rd] = (int32_t)x[rs1] >> x[rs2]; break;         // sra
                        }
                        break;
                    case 0b110:  x[rd] = x[rs1] | x[rs2]; break;                               // or
                    case 0b111:  x[rd] = x[rs1] & x[rs2]; break;                               // and
                }
                break;
            
            case I:
                switch(funct3)
                {

                    case 0b000: std::cout<<"x["<<rd<<"]"<<x[rd]<<std::endl;
                                std::cout<<"x["<<rs1<<"]"<<x[rs1]<<std::endl;
                                std::cout<<"imm: "<<std::hex<<imm<<std::endl;
                    x[rd] = x[rs1] + imm;   // addi

                                break;                                   
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
                    //case 0b000: x[rd] = pc + 4; pc = x[rs1] + imm; break;                      // jalr
                }
                break;
            
            case S:
                switch(funct3)
                {
                    case 0b000: std::cout<<"KArthik"<<std::endl;
                                StoreMem(rs2+imm, 1, rs1);
                                #ifdef debug
                                std::cout<<"MemorySpace["<<(rs2+imm)<<"]"<<MemorySpace[rs2+imm]<<std::endl; 
                                #endif
                                break;                  // sb
                    case 0b001: StoreMem(rs2+imm, 2, rs1) ;
                                #ifdef debug
                                std::cout<<"MemorySpace["<<(rs2+imm)<<"]"<<MemorySpace[rs2+imm]<<std::endl; 
                                #endif 
                                break;   // sh
                    case 0b010: std::cout<<"KArthik"<<std::endl;
                                StoreMem(rs2+imm, 4, rs1);
                                #ifdef debug
                                std::cout<<"MemorySpace["<<(rs2+imm)<<"]"<<MemorySpace[rs2+imm]<<std::endl; 
                                #endif 
                                break;              // sw
                }
                break;
            
            case B:
                switch(funct3)
                {
                    case 0b000: if (x[rs1] == x[rs2]) pc += imm; break;                         // beq
                    case 0b001: if (x[rs1] != x[rs2]) pc += imm; break;                         // bne
                    case 0b100: if (x[rs1] < x[rs2]) pc += imm; break;                          // blt
                    case 0b101: if (x[rs1] >= x[rs2]) pc += imm; break;                         // bge
                    case 0b110: if ((uint32_t)x[rs1] < (uint32_t)x[rs2]) pc += imm; break;      // bltu
                    case 0b111: if ((uint32_t)x[rs1] >= (uint32_t)x[rs2]) pc += imm; break;     // bgeu
                }
                break;
            
            case U:
                switch(opcode)
                {
                    // case LUI: x[rd] = imm << 12; break;                                         // Load Upper Immediate
                    // case AUIPC: x[rd] = pc + (imm << 12); break;                                // Add Upper Immediate to PC
                }
                break;
            
            case J: break;
                }
                Print(pc, mode);

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

        std::cout << "ProgramSize: " << ProgramSize << "\n";

        #ifdef debug
        std::cout << "\nMemory Contents:\n";
        for (int i = 0; i < ProgramSize; i++) 
        {
            std::cout << std::hex << i << "  :" << std::hex << (int)MemorySpace[i] << std::endl;
            // if((i%4 )== 0){
            // int check = ReadMem(i, 4);
            // std::cout<<"check: "<<std::hex<<check<<std::endl;
        //}
        }
        #endif



        while(true)
        {
            Fetch(pc);
            Decode(pc);
            if(CurrentInstr == 0)
            {
                Print(pc, mode);
                break;
            }
            Execute(pc);

            pc = pc + 4;


        }

    }
