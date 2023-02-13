#pragma once
namespace LibMemory{
    class Library;

    class Process{
    protected:
        int pid;
    public:
        Process(int pid);
        Process();
        ~Process();
        
        static Process open(const char *procName, unsigned int index = 0);
        static Process openFirst(const char *procName);
        Process openNext(const char *procName) const;
        static Process openLast(const char *procName);
        int sig(int signal) const;
        int getpid() const;        

        void close();

        bool inject(Library& lib);

        operator bool() const;
    };
}