#include <iostream>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <cstring>
#include "defines.h"
#define UNSIGNED 0
#define SIGNED 1

#define StartAddress 0 
#define StackAddress 1<<16 

uint8_t MemorySpace[1 << 16] = {};
uint32_t x[32] = {};
float f[32] = {};
int mode;
std::string MemoryImage;
int CurrentInstr;
bool BranchTaken;
int debug;


uint8_t opcode;
uint8_t rd;
uint8_t funct3;
uint8_t rs1;
uint8_t rs2;
uint8_t funct7;
uint32_t imm;
uint32_t pc;





void Print()
{
    static const char* regNames[32] = {
        "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
        "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
        "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
        "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
    };


        std::cout << "Current Instruction : 0x" << std::hex << CurrentInstr << std::endl;

        for (int i = 0; i < 32; i++)
        {
            std::cout << std::dec << "x[" << i << "] (" 
                      << regNames[i] << ") = 0x" 
                      << std::hex << x[i] << std::endl;
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
        rd = (CurrentInstr >> 7) & 0x1f;
        funct3 = (CurrentInstr >> 12) & 0x7;
        rs1 = (CurrentInstr >> 15) & 0x1f;
        rs2 = (CurrentInstr >> 20) & 0x1f;
        funct7 = CurrentInstr >> 25;

        if ((opcode == I) || (opcode == I_LOADS) || (opcode == ECALL) || (opcode == I_JALR))
        {

        imm = (CurrentInstr >> 20) & 0xFFF;    
        if (imm & 0x800) 
        {  
            imm |= 0xFFFFF000; 
        }

        }

        else if(opcode == S)
        {

            imm = (CurrentInstr>>7 & 0x1f) | (( CurrentInstr >> 25 & 0x7f) <<5);
            imm = (imm>>11) ? imm | 0xfffff000 : imm;
        }

        else if (opcode == B)
        {
        imm = ((CurrentInstr >> 31) & 0x1) << 12 | ((CurrentInstr >> 7) & 0x1) << 11 |  ((CurrentInstr >> 25) & 0x3F) << 5 | ((CurrentInstr >> 8) & 0xF) << 1;
        if (imm & 0x1000) 
        {
            imm |= 0xFFFFE000; 
        }
        }

        else if (opcode == U_LUI || opcode == U_AUIPC )
        {
        imm = CurrentInstr & 0xFFFFF000;

        }

        else if(opcode == J ) 
        {

        imm = ((CurrentInstr >> 31) & 0x1) << 20 | ((CurrentInstr >> 21) & 0x3FF) << 1 | ((CurrentInstr >> 20) & 0x1) << 11 | ((CurrentInstr >> 12) & 0xFF) << 12;
        imm = (imm>>19) ? (imm | 0xFFF00000) : imm;

        }
        
        else if(opcode == FLW)
        { 
            imm = (CurrentInstr >> 20) & 0xfff;
            if(imm & 0x800) imm |= 0xfffff000;
        }        
        else if(opcode == FSW)
        { 
            imm = ((CurrentInstr >> 7) & 0x1f) | ((CurrentInstr >> 25) << 5);
            if(imm & 0x800) imm |= 0xfffff000;
        }
        
        else if(opcode == FPA) 
        {
            funct7 = (CurrentInstr >> 25) & 0x7f;
        }
    
        if(debug)
        {
            std::cout << "funct3: " <<std::dec<< (int)funct3 << std::endl;
            std::cout << "funct7: " <<std::dec<< (int)funct7 << std::endl;
            std::cout << "rs1: " <<std::dec<< (int)rs1 << std::endl;
            std::cout << "rs2: " <<std::dec<< (int)rs2 << std::endl;
            std::cout << "rd: " <<std::dec<< (int)rd << std::endl;
            std::cout << "imm: " <<std::hex<<imm << std::endl;
        }



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
                                        case 0b0000000:  if(debug) std::cout<<"ADD Detected"<<std::endl;   
                                                         x[rd] = x[rs1] + x[rs2]; break;                   
                                        case 0b0100000:  if(debug) std::cout<<"SUB Detected"<<std::endl;
                                                         x[rd] = x[rs1] - x[rs2]; break;                   
                                    }
                                    break;
                        case 0b001: if(debug) std::cout<<"Shift Left Detected"<<std::endl;
                                    x[rd] = x[rs1] << x[rs2]; break;                             

                        case 0b010: if(debug) std::cout<<"Shift Less Than Detected"<<std::endl;
                                    x[rd] = (x[rs1] < x[rs2]) ? 1 : 0; break;                    

                        case 0b011: if(debug) std::cout<<"Shift Less Than Unsigned Detected"<<std::endl;
                                    x[rd] = (uint32_t)x[rs1] < (uint32_t)x[rs2] ? 1 : 0; break;   

                        case 0b100: if(debug) std::cout<<"XOR Detected"<<std::endl;
                                    x[rd] = x[rs1] ^ x[rs2]; break;                               

                                    switch(funct7)
                                    {
                                        case 0b0000000:  if(debug) std::cout<<"Shift Right Detected"<<std::endl;
                                                         x[rd] = x[rs1] >> x[rs2]; break;             
                                        case 0b0100000:  if(debug) std::cout<<"Shift Right Arithmetic Detected"<<std::endl;
                                                         x[rd] = (int32_t)x[rs1] >> x[rs2]; break;         
                                    }
                                    break;
                        case 0b110: if(debug) std::cout<<"OR Detected"<<std::endl;
                                    x[rd] = x[rs1] | x[rs2]; break;                               
                        case 0b111: if(debug) std::cout<<"AND Detected"<<std::endl;
                                    x[rd] = x[rs1] & x[rs2]; break;                           
                    }
                    break;
            
            case I:
                    switch(funct3)
                    {

                        case 0b000: if(debug) std::cout<<"ADDI Detected"<<std::endl;
                                    x[rd] = x[rs1] + imm ; break;                                  

                                                                      
                        case 0b010: if(debug) std::cout<<"Set Less Than Immediate Detected"<<std::endl;
                                    x[rd] = ((int32_t)x[rs1] < (int32_t)imm) ? 1 : 0; break;                                   

                        case 0b011: if(debug) std::cout<<"Set Less Than Unsigned Immediate Detected"<<std::endl;
                                    x[rd] = (uint32_t)x[rs1] < (uint32_t)imm ? 1 : 0; break;                        

                        case 0b100: if(debug) std::cout<<"XORI Detected"<<std::endl;
                                    x[rd] = x[rs1] ^ imm; break;                                   

                        case 0b110: if(debug) std::cout<<"ORI Detected"<<std::endl;
                                    x[rd] = x[rs1] | imm; break;                                

                        case 0b111: if(debug) std::cout<<"ANDI Detected"<<std::endl;
                                    x[rd] = x[rs1] & imm; break;                                

                        case 0b001: if(debug) std::cout<<"SLLI Detected"<<std::endl;
                                    x[rd] = x[rs1] << (imm & 0x1F); break;                      

                        case 0b101:
                                    switch(funct7)
                                    {
                                        case 0b0000000: if(debug) std::cout<<"SRLI Detected"<<std::endl;
                                                        x[rd] = x[rs1] >> (imm & 0x1F); break;             

                                        case 0b0100000: if(debug) std::cout<<"SRAI Detected"<<std::endl;
                                                        x[rd] = (int32_t)x[rs1] >> (imm & 0x1F); break;  
                                    }
                                    break;
                    }
                    break;
            case I_JALR : if(debug) std::cout<<"JALR Detected"<<std::endl;
                          x[rd] = pc + 4;
                          pc = (x[rs1] + imm) & 0xFFFFFFFE;
                          BranchTaken = true;
                          break;
            // case ECALL  : if(debug) std::cout<<"ECALL Detected"<<std::endl; 
            //                 switch(x[17])
            //                 {
            //                     case 63:   {
            //                         int fdr = x[10]; 
            //                         uint32_t addrr = x[11]; 
            //                         int sizer = x[12];
            //                         if (fdr == 0) 
            //                         { 
            //                             std::cin.read(&MemorySpace[addrr], sizer);
            //                         }
            //                         x[10] = std::cin.gcount(); 
            //                         break;}
            //                     case 64: {
            //                         int fdw = x[10]; 
            //                         uint32_t addrw = x[11]; 
            //                         int sizew = x[12];
            //                         if (fdw == 1) 
            //                         { 
            //                             std::cout.write(&MemorySpace[addrw], sizew)<<std::endl;
            //                             std::cout.flush();
            //                         }
            //                         x[10] = sizew;
            //                         break;}
            //                     } break;

                
            
            case S:
                    switch(funct3)
                    {
                        case 0b000: if(debug) std::cout<<"Store Byte detected"<<std::endl;
                                    StoreMem(x[rs1]+imm, byte, x[rs2]); break;               

                        case 0b001: if(debug) std::cout<<"Store Half Word detected"<<std::endl;
                                    StoreMem(x[rs1]+imm, halfword, x[rs2]); break;              

                        case 0b010: if(debug)std::cout<<"Store Word detected"<<std::endl;
                                    StoreMem(x[rs1]+imm, word, x[rs2]);   break ;              

                    }   
                    break;
            
            case B:
                    switch(funct3)
                    {
                        case 0b000: if(debug) std::cout<<"BEQ detected"<<std::endl;
                                    if (x[rs1] == x[rs2])                                       
                                    { 
                                        BranchTaken = true;
                                        pc += imm;
                                    }   break;                                                        
                        case 0b001: if(debug) std::cout<<"BNE detected"<<std::endl;
                                    if (x[rs1] != x[rs2])                                       
                                    {
                                        BranchTaken = true;
                                        pc += imm; 
                                    }   break;                                                        
                        case 0b100: if(debug) std::cout<<"BLT detected"<<std::endl;
                                    if ((int32_t)x[rs1] < (int32_t)x[rs2])                     
                                    {   
                                        BranchTaken = true;
                                        pc += imm; 
                                    }   break;                                                            
                        case 0b101: if(debug) std::cout<<"BGE detected"<<std::endl;

                                    if ((int32_t)x[rs1] >= (int32_t)x[rs2])                 
                                    {
                                        BranchTaken = true;
                                        pc += imm; 
                                    }   break;                         
                        case 0b110: if(debug) std::cout<<"BLTU detected"<<std::endl;
                                    if (x[rs1] < x[rs2])                                         
                                    {                       
                                        BranchTaken = true;
                                        pc += imm;    
                                    }   break;
                        case 0b111: if(debug) std::cout<<"BGEU detected"<<std::endl;
                                    if (x[rs1] >=x[rs2])                                      
                                    {
                                        BranchTaken = true;
                                        pc += imm; 
                                    }   break;     
                    } break;
            
            case U_LUI   :  if(debug) std::cout<<" LUI Detected "<<std::endl;
                            x[rd] = imm; break; 

            case U_AUIPC :  if(debug) std::cout<<" AUIPC Detected "<<std::endl;
                            x[rd] = pc + imm; break;

            
            case J       :  if(debug) std::cout<<" JAL Detected "<<std::endl;
                            x[rd] = pc + 4; 
                            pc = pc + imm; 
                            BranchTaken = true;
                            break;
                            
            case I_LOADS : 
                            switch(funct3)
                                    { 
                                        case 0b000: if(debug) std::cout<<"Load Byte Detected"<<std::endl;
                                                    x[rd] = ReadMem(byte, SIGNED, imm + x[rs1]); break;

                                        case 0b001: if(debug) std::cout<<"Load Half Word Detected"<<std::endl;
                                                    x[rd] = ReadMem(halfword, SIGNED,imm +  x[rs1]); break;   

                                        case 0b010: if(debug) std::cout<<"Load Word Detected"<<std::endl; 
                                                    x[rd] = ReadMem(word, UNSIGNED, imm +  x[rs1]); break;   

                                        case 0b100: if(debug) std::cout<<"Load Byte Unsigned Detected"<<std::endl;
                                                    x[rd] = ReadMem(byte, UNSIGNED, imm + x[rs1] ); break;    

                                        case 0b101: if(debug) std::cout<<"Load HalfWord Unsigned Detected"<<std::endl;
                                                    x[rd] = ReadMem(halfword, UNSIGNED, imm + x[rs1]);break; 
                                    } break;
            case FLW:  switch(funct3)
                        {
                            case 0b010: if(debug) std::cout<<"FLW Detected"<<std::endl;
                            f[rd] = ReadMem(word,UNSIGNED, x[rs1] + imm); break;
                        }break;
                                     
            case FSW:  switch(funct3)
                        {
                            case 0b010: if(debug) std::cout<<"FSW Detected"<<std::endl;
                                        StoreMem(f[rs1] + imm, word, f[rs2]); break;    //StoreMem(x[rs1]+imm, word, x[rs2]);   break ;  
                        } break;                        
            case FPA: { // FP Arithmetic
                        switch(funct3) 
                        {
                            case 0x00: if(debug) std::cout<<"FADD.S Detected"<<std::endl;
                                    f[rd] = f[rs1] + f[rs2]; break;  // FADD.S

                            case 0x04: if(debug) std::cout<<"FSUB.S Detected"<<std::endl;
                                    f[rd] = f[rs1] - f[rs2]; break;  // FSUB.S

                            case 0x08: if(debug) std::cout<<"FMUL.S Detected"<<std::endl;
                                    f[rd] = f[rs1] * f[rs2]; break;  // FMUL.S

                            case 0x0C: if(debug) std::cout<<"FDIV.S Detected"<<std::endl;
                                    f[rd] = f[rs1] / f[rs2]; break;  // FDIV.S

                            case 0x2C: if(debug) std::cout<<"FSQRT.S Detected"<<std::endl;
                                    f[rd] = sqrtf(f[rs1]); break;    // FSQRT.S
                            
                            case 0x50: if(debug) std::cout<<"FEQ.S Detected"<<std::endl;
                                    x[rd] = (f[rs1] == f[rs2]); break; // FEQ.S

                            case 0x51: if(debug) std::cout<<"FLT.S Detected"<<std::endl;
                                    x[rd] = (f[rs1] < f[rs2]); break;  // FLT.S
                            case 0x52: if(debug) std::cout<<"FLE.S Detected"<<std::endl;
                                    x[rd] = (f[rs1] <= f[rs2]); break; // FLE.S
                            
                            case 0x60: if(debug) std::cout<<"FCT.W.S Detected"<<std::endl;
                                    x[rd] = (int32_t)f[rs1]; break;   // FCVT.W.S

                            case 0x61: if(debug) std::cout<<"FCVT.WU.S Detected"<<std::endl;
                                    x[rd] = (uint32_t)f[rs1]; break;  // FCVT.WU.S

                            case 0x68: if(debug) std::cout<<"FCVT.S.W Detected"<<std::endl;
                                    f[rd] = (int32_t)x[rs1]; break;   // FCVT.S.W

                            case 0x69: if(debug) std::cout<<"FCVT.S.WU Detected"<<std::endl;
                                    f[rd] = (uint32_t)x[rs1]; break;  // FCVT.S.WU
                            
                            case 0x10: // FSGNJ.S
                                memcpy(&f[rd], &f[rs1], sizeof(float));
                                ((uint32_t*)&f[rd])[0] ^= 
                                    (((uint32_t*)&f[rs2])[0] & 0x80000000);
                                break;
                        }
                }
                break;

                }



    
    
    x[0]=0;            
    if(mode == Verbose) Print();

    
    }



    int main(int argc, char* argv[]) 
    {
        int ProgramSize = 0;
        pc = StartAddress;
        x[2] = StackAddress;


        switch(argc)
        {
            case 6: MemoryImage = argv[1];
                    pc = std::stoi(argv[2]);
                    x[2] = std::stoi(argv[3]);
                    mode = std::stoi(argv[4]);
                    debug = std::stoi(argv[5]);
                    break;
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



        if(debug)
        {
        std::cout << "StartAddress: " << StartAddress << "\n";
        std::cout << "StackAddress: " << StackAddress << "\n";
        std::cout << "Stack Pointer: "  << x[2] << "\n";
        }

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
                if(debug)
                {
                std::cout << "Address: 0x" << std::hex << address
                        << " => data: " << data << std::endl;
                }

                sscanf(data.c_str(), "%x", &value);
                ProgramSize += 4; 


                StoreMem(address, (data.size())/2, value );    //Storing into Memory Space

            }
        }
        file.close();
        
        std::cout<< "-----------------RISC-V Simulator---------------------------"<<std::endl;
        std::cout << "ProgramSize: " << ProgramSize << "\n";

        if(debug)
        {
            std::cout << "\nMemory Contents:\n";
            for (int i = 0; i < ProgramSize; i++) 
            {
                std::cout << std::hex << i << "  :" << std::hex << (int)MemorySpace[i] << std::endl;
            }
        }



        while(true)
        {
            if(mode == Verbose) std::cout << "Program Counter : 0x" << std::hex << pc << std::endl;
            Fetch();
            if((CurrentInstr == 0) ||((CurrentInstr == 0x8067) && (x[1] == 0)))
            {
                if(mode == !Verbose) std::cout << "Program Counter : 0x" << std::hex << pc << std::endl;
                Print();
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
