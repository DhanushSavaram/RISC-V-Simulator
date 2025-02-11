#include <iostream>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>

#define DefaultFile "sample.mem"


uint8_t MemorySpace[1 << 16] = {};
uint32_t StartAddress = 0;
uint32_t StackAddress = 1<<16;
uint32_t x[32] = {};
uint32_t sp = 0;
uint32_t pc = 0;
std::string MemoryFile;

// struct DataStruct {
//     uint32_t I_format : 32; 

//     unsigned int  : ; 
//     unsigned int field2 : 20; 
//     unsigned int field3 : 2;  
// };


void Load(){
    //loading Unsgined byte to register from memmory
    x[4]=MemorySpace[3];
    std::cout<<"Unsigned Byte:"<<"x[4]="<<std::hex<<x[4]<<std::endl;

    //loading Signed byte to register from memory
    x[4]=MemorySpace[3]>>7?MemorySpace[3]|(0xffffff00):MemorySpace[3];
    std::cout<<"Signed Byte:"<<"x[4]="<<std::hex<<x[4]<<std::endl;

    //loading Signed two bytes to register from memory
    x[4]=MemorySpace[0x11]|MemorySpace[0x12]<<8;
    std::cout<<"Unsigned TwoBytes:"<<"x[4]="<<std::hex<<x[4]<<std::endl;

    //loading Signed two bytes to register from memory
    x[4]= (MemorySpace[0x12]>>7) ?  (MemorySpace[0x11]|(MemorySpace[0x12]<<8)|(0xffff0000)) : (MemorySpace[0x11]|(MemorySpace[0x12]<<8));
    std::cout<<"Signed Two Bytes:"<<"x[4]="<<std::hex<<x[4]<<std::endl;

    //Loading 4byte value to the register
    x[4]=MemorySpace[3];

}

 void Store(){
    x[0]=0x12345678;
    MemorySpace[0]=x[0];       //storing a byte
    std::cout<<"Storing One Byte"<<"\n";
    std::cout<<"MemorySpace[0]="<<std::hex<<static_cast<int>(MemorySpace[0])<<"\n";

    MemorySpace[0]=x[0];       //storing two bytes
    MemorySpace[1]=x[0]>>8;
    std::cout<<"Storing Two Bytes"<<"\n";
    std::cout<<"MemorySpace[0]="<<std::hex<<static_cast<int>(MemorySpace[0])<<"\n";
    std::cout<<"MemorySpace[1]"<<std::hex<<static_cast<int>(MemorySpace[1])<<"\n";

    MemorySpace[0]=x[0];      //Storing four bytes
    std::cout<<"Storing Four Bytes"<<"\n";
    std::cout<<"MemorySpace[0]="<<std::hex<<static_cast<int>(MemorySpace[0])<<"\n";
    MemorySpace[1]=x[0]>>8;
    std::cout<<"MemorySpace[1]="<<std::hex<<static_cast<int>(MemorySpace[1])<<"\n";
    MemorySpace[2]=x[0]>>16;
    std::cout<<"MemorySpace[2]="<<std::hex<<static_cast<int>(MemorySpace[2])<<"\n";
    MemorySpace[3]=x[0]>>24;
    std::cout<<"MemorySpace[3]="<<std::hex<<static_cast<int>(MemorySpace[3])<<"\n";


    }






int main(int argc, char* argv[]) {
    int ProgramSize;

    if (argc >= 4) {
        MemoryFile = argv[1];
        StartAddress = std::stoi(argv[2]);
        StackAddress = std::stoi(argv[3]);
    } else {
        MemoryFile = DefaultFile;
    }
    #ifdef debug
    std::cout << "StartAddress: " << StartAddress << "\n"
OBOBOBOB              << "StackAddress: " << StackAddress << "\n";
OBOB    #endif

OB    std::ifstream file(MemoryFile);
    if (!file) {
OBOBOBOB        std::cerr << "Error: Could not open file!" << std::endl;
OB        return 0;
    }
OBOB
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream stream(line);
        uint32_t address,value;
        char colon;
OBOBOBOBOBOB        std::string bytes;

        if (stream >> std::hex >> address >> colon >> bytes) {
OB            #ifdef debug
            std::cout << "Address: 0x" << std::hex << address
                      << " => bytes: " << bytes << std::endl;
            #endif

                sscanf(bytes.c_str(), "%x", &value);
                ProgramSize+=4;

                if(bytes.size()==8){
                    MemorySpace[address]   = value & 0xFF;
                    MemorySpace[address+1] = (value >> 8) & 0xFF;
                    MemorySpace[address+2] = (value >> 16) & 0xFF;
                    MemorySpace[address+3] = (value >> 24) & 0xFF;}
                else if (bytes.size()==4){
                    MemorySpace[address]   = value & 0xFF;
                    MemorySpace[address+1] = (value >> 8) & 0xFF;

                }
                else if (bytes.size()==2){
                    MemorySpace[address]   = value & 0xFF;

                }

        }
    }
    file.close();
    std::cout<<"ProgramSize"<<ProgramSize<<"\n";
    #ifdef debug
    std::cout << "\nMemory Contents:\n";
    for (int i = 0; i < ProgramSize; i++) {
        if (MemorySpace[i] != 0) {
            std::cout << std::hex << i <<"  :" << std::hex << (int)MemorySpace[i] << std::endl;
        }

    }
    #endif

    #ifdef debug
    if ()
    Load();
    Store();
    #endif




    return 0;
}



