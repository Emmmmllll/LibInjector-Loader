#include "library.h"
#include <cstring>
#include <string>
#include <stdexcept>
using namespace LibMemory;

Library::Library(const char *path, unsigned int lm){open(path, lm);}
Library::Library(){}
Library::~Library(){close();}

void
Library::open(const char * path, unsigned int lm){
    delete[] this->path;
    size_t len = strlen(path);
    this->path = new char[len+1];
    memcpy(this->path, path, len);
    this->path[len] = '\0';

    handle = dlopen(path, lm);
    if(!handle){
        throw std::runtime_error(std::string(dlerror()));        
    }
}
void
Library::close(){
    delete[] this->path;
    this->path = nullptr;
    dlclose(handle);
}
void *
Library::loadsym(const char * name) const{
    void * sym = dlsym(handle, name);
    char * error = dlerror();
    if (error)
        throw std::runtime_error(std::string(error));
    return sym;
}
const char *
Library::getPath() const {return this->path;}
void *
Library::getHandle() const {return this->handle;}
Library::operator bool() const {return (bool)this->handle;}