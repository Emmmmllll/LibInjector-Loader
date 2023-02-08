#pragma once
#include <dlfcn.h>
namespace LibMemory{
    class Library{
    protected:
        char * path = nullptr;
        void * handle = nullptr;
    public:
        enum OpenMethod : unsigned int { 
            Lazy = RTLD_LAZY,
            Now = RTLD_NOW,
            Global = RTLD_GLOBAL,
            Local = RTLD_LOCAL,
            NoDelete = RTLD_NODELETE,
            NoLoad = RTLD_NOLOAD,
            DeepBind = RTLD_DEEPBIND
        };
        Library(const char *path, unsigned int method = OpenMethod::Lazy);
        Library();
        ~Library();

        void * loadsym(const char *name) const;
    template<typename T>
        void loadsym(const char *name, T& sym) const{
            *reinterpret_cast<void**>(&sym) = loadsym(name);}
        void open(const char *path, unsigned int method = OpenMethod::Lazy);
        void close();
        const char * getPath() const;
        void * getHandle() const;

        operator bool() const;
    };
}