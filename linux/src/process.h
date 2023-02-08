#pragma once
namespace LibMemory{
    class Library;
    class Process{
    protected:
        unsigned int pid = 0;
    public:
        Process(const char *procName);
        Process();
        ~Process();
        
        void open(const char *procName);
        void close();

        bool inject(Library& lib);

        operator bool() const;
    };
}