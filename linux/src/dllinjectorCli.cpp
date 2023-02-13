#include "process.h"
#include "library.h"

#include <iostream>

int main(){
    using LibMemory::Process;
    std::cout << "I am a cli" << std::endl;
    Process p = Process::openFirst("systemd");
    std::cout << "btw systemd's pid is " << p.getpid() << '.' << std::endl;

    return 0;
}