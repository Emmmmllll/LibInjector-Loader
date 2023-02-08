#include "process.h"

using namespace LibMemory;

Process::Process(const char *proc){open(proc);}
Process::Process(){}
Process::~Process(){close();}

void
Process::open(const char *proc){
    
}
void
Process::close(){

}