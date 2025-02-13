#include <iostream>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>

#define DefaultFile "sample.mem"

uint8_t MemorySpace[1 << 16] = {};
uint32_t StartAddress = 0;
uint32_t StackAddress = 1 << 16;
uint32_t x[32] = {};
uint32_t sp = 0;
uint32_t pc = 0;
std::string MemoryFile;




void Load(){
    //loading Unsgined byte to register from memory
    x[4]=MemorySpace[3];
    std::cout<<"Unsigned Byte:"<<"x[4]="<<std::hex<<x[4]<<std::endl;

    //loading Signed byte to register from memory
    x[4]=MemorySpace[3]>>7?MemorySpace[3]|(0xffffff00):MemorySpace[3];
    std::cout<<"Signed Byte:"<<"x[4]="<<std::hex<<x[4]<<std::endl;

    //loading Unsgned Signed Half word to register from memory
    x[4]=MemorySpace[0x11]|MemorySpace[0x12]<<8;
    std::cout<<"Unsigned TwoBytes:"<<"x[4]="<<std::hex<<x[4]<<std::endl;

    //loading Signed Half word to register from memory
    x[4]= (MemorySpace[0x12]>>7) ?  (MemorySpace[0x11]|(MemorySpace[0x12]<<8)|(0xffff0000)) : (MemorySpace[0x11]|(MemorySpace[0x12]<<8));
    std::cout<<"Signed Two Bytes:"<<"x[4]="<<std::hex<<x[4]<<std::endl;

    //Loading word to the register
    x[4]=MemorySpace[3];

}

 void Store(){
    x[0]=0x12345678;
    std::cout<<"Register value to be stored"<<x[0]<<std::endl;
    MemorySpace[100]=x[0];       //storing a byte
    std::cout<<"Storing One Byte"<<"\n";
    std::cout<<"MemorySpace[0]="<<std::hex<<static_cast<int>(MemorySpace[0])<<"\n";

    MemorySpace[100]=x[0];       //storing Half word
    MemorySpace[100+1]=x[0]>>8;
    std::cout<<"Storing half word"<<"\n";
    std::cout<<"MemorySpace[0]="<<std::hex<<static_cast<int>(MemorySpace[0])<<"\n";
    std::cout<<"MemorySpace[1]"<<std::hex<<static_cast<int>(MemorySpace[1])<<"\n";

    MemorySpace[100]=x[0];      //Storing word
    std::cout<<"Storing Four Bytes"<<"\n";
    std::cout<<"MemorySpace[0]="<<std::hex<<static_cast<int>(MemorySpace[0])<<"\n";
    MemorySpace[100+1]=x[0]>>8;
    std::cout<<"MemorySpace[1]="<<std::hex<<static_cast<int>(MemorySpace[1])<<"\n";
    MemorySpace[100+2]=x[0]>>16;
    std::cout<<"MemorySpace[2]="<<std::hex<<static_cast<int>(MemorySpace[2])<<"\n";
    MemorySpace[100+3]=x[0]>>24;
    std::cout<<"MemorySpace[3]="<<std::hex<<static_cast<int>(MemorySpace[3])<<"\n";


    }

int main(int argc, char* argv[]) {
    int ProgramSize = 0;

    if (argc >= 4) {
        MemoryFile = argv[1];
        StartAddress = std::stoi(argv[2]);
        StackAddress = std::stoi(argv[3]);
    } else {
        MemoryFile = DefaultFile;
    }

    #ifdef debug
    std::cout << "StartAddress: " << StartAddress << "\n"
              << "StackAddress: " << StackAddress << "\n";
    #endif

    std::ifstream file(MemoryFile);
    if (!file) {
        std::cerr << "Error: Could not open file!" << std::endl;
        return 0;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream stream(line);
        uint32_t address, value;
        char colon;
        std::string bytes;

        if (stream >> std::hex >> address >> colon >> bytes) {
            #ifdef debug
            std::cout << "Address: 0x" << std::hex << address
                      << " => bytes: " << bytes << std::endl;
            #endif

            sscanf(bytes.c_str(), "%x", &value);
            ProgramSize += 4;  // Increment by 4 for each instruction read (32-bit instruction)

            if (bytes.size() == 8) {
                MemorySpace[address]   = value & 0xFF;
                MemorySpace[address + 1] = (value >> 8) & 0xFF;
                MemorySpace[address + 2] = (value >> 16) & 0xFF;
                MemorySpace[address + 3] = (value >> 24) & 0xFF;
            } else if (bytes.size() == 4) {
                MemorySpace[address]   = value & 0xFF;
                MemorySpace[address + 1] = (value >> 8) & 0xFF;
            } else if (bytes.size() == 2) {
                MemorySpace[address]   = value & 0xFF;
            }
        }
    }
    file.close();

    std::cout << "ProgramSize: " << ProgramSize << "\n";

    #ifdef debug
    std::cout << "\nMemory Contents:\n";
    for (int i = 0; i < ProgramSize; i++) {
        std::cout << std::hex << i << "  :" << std::hex << (int)MemorySpace[i] << std::endl;
    }
    #endif

    #ifdef debug
    Load();
    Store();
    #endif

    uint32_t CI;
    uint32_t opcode;
    uint32_t rd;
    uint32_t funct3;
    uint32_t rs1;
    uint32_t imm;
    // std::cout << "pc=" << pc << std::endl;
    // std::cout << "ProgramSize=" << ProgramSize << std::endl;
    pc=StartAddress;

    while (pc < (1<<16)) {

        CI = MemorySpace[pc] | (MemorySpace[pc + 1] << 8) | (MemorySpace[pc + 2] << 16) | (MemorySpace[pc + 3] << 24);
        std::cout << "Current Instruction: " <<std::hex<< CI << std::endl;

        if(CI==0){
            return 0;
        }

        pc += 4;

        // imm = CI >> 20;
        // opcode = CI & 0x7f;
        // std::cout << "Current Opcode: " << opcode << std::endl;

        // rd = (CI >> 7) & 0x1f;
        // funct3 = (CI >> 12) & 0x7;
        // std::cout << "Current Funct3: " << funct3 << std::endl;
        // rs1 = (CI >> 15) & 0x1f;

        // switch (opcode) {
        //     case 0x13:
        //         if (funct3 == 0x0) {
        //             std::cout << "Add Immediate detected" << std::endl;
        //             x[rd] = x[rs1] + imm;
        //             std::cout<<"x["<<rd<<"]"<<x[rd]<<std::endl;
        //         }
        //         break;

        //     case 0x3:
        //         if (funct3 == 0x2) {
        //             std::cout << "Load word instruction detected" << std::endl;
        //             x[rd] = MemorySpace[x[rs1]] | (MemorySpace[x[rs1] + 1] << 8) | (MemorySpace[x[rs1] + 2] << 16) | (MemorySpace[x[rs1] + 3] << 24);
        //             std::cout<<"x["<<rd<<"]"<<x[rd]<<std::endl;
        //         }
        //         else if (funct3 == 0x0) {
        //             std::cout << "Load byte instruction detected" << std::endl;
        //             if (MemorySpace[x[rs1]]<<7 ) {
        //                 x[rd] = MemorySpace[x[rs1]] | 0xFFFFFF00;
        //             } else {
        //                 x[rd] = MemorySpace[x[rs1]];
        //             }
        //             std::cout<<"x["<<rd<<"]"<<x[rd]<<std::endl;
        //         }
        //         else if (funct3 == 0x1) {
        //             std::cout << "Load half word instruction detected" << std::endl;
        //             if (MemorySpace[x[rs1] + 1] <<7) {
        //                 x[rd] = (MemorySpace[x[rs1] + 1] << 8) | MemorySpace[x[rs1]] | 0xFFFF0000;
        //             } else {
        //                 x[rd] = (MemorySpace[x[rs1] + 1] << 8) | MemorySpace[x[rs1]];
        //             }
        //             std::cout<<"x["<<rd<<"]"<<x[rd]<<std::endl;
        //         }
        //         else if (funct3 == 0x4) {
        //             std::cout << "Load half word unsigned instruction detected" << std::endl;
        //             x[rd] = (MemorySpace[x[rs1] + 1] << 8) | MemorySpace[x[rs1]];
        //             std::cout<<"x["<<rd<<"]"<<x[rd]<<std::endl;
        //         }
        //         else if (funct3 == 0x5) {
        //             std::cout << "Load byte unsigned instruction detected" << std::endl;
        //             x[rd] = MemorySpace[x[rs1]];
        //             std::cout<<"x["<<rd<<"]"<<x[rd]<<std::endl;
        //         }
        //         break;
            
        //     case 0x23: std::cout<<"Store Instruction detected" <<std::endl;
        //                if(funct3==0x2)
        //                {
                        
        //                 std::cout<<"Store word Instruction detected" <<std::endl;
        //                 MemorySpace[x[rd]]=x[rs1];
        //                 MemorySpace[x[rd]+1]=x[rs1]<<8;
        //                 MemorySpace[x[rd]+2]=x[rs1]<<16;
        //                 MemorySpace[x[rd]+3]=x[rs1]<<24;
        //                 std::cout<<"x[rs1]="<<std::dec<<x[rs1]<<std::endl;
        //                }
        //                break;


        // }

    }
    // std::cout<<"MemorySpace[1000]="<<std::hex<<static_cast<int>(MemorySpace[1000])<<"\n";
    // std::cout<<"MemorySpace[1001]="<<std::hex<<static_cast<int>(MemorySpace[1001])<<"\n";
    // std::cout<<"MemorySpace[1002]="<<std::hex<<static_cast<int>(MemorySpace[1002])<<"\n";
    // std::cout<<"MemorySpace[1003]="<<std::hex<<static_cast<int>(MemorySpace[1003])<<"\n";


    // std::cout<<"x[11]="<<std::dec<<x[11]<<std::endl;


    return 0;
}
