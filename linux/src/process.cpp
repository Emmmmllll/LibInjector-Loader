#include "process.h"
#include <signal.h>
#include <sys/ptrace.h>
#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <functional>

inline bool isNumeric(const char *s) {
    size_t i = 0;
    while (s[i]){
        if (s[i] < '0' || s[i] > '9')
            return false;
        i++;
    }
    return true;
}
inline bool isDir(const dirent& dir) {return dir.d_type == DT_DIR; }
inline unsigned int getSymLinkFileName(char str[256], char * path){
    constexpr int MAX_FILENAME_LENGTH = 255;
    constexpr int MAX_PATH_LENGTH = 4096;
    char buf[MAX_PATH_LENGTH];
    int readBytes = readlink(path, buf, sizeof(buf));
    if (readBytes == -1) return 0;
    unsigned int offset = readBytes > MAX_FILENAME_LENGTH ? readBytes - MAX_FILENAME_LENGTH : 0;
    unsigned int len = readBytes - offset;

    memcpy(str, buf + offset, len);

    str[len + 1] = '\0'; 
    return len;       
}
bool compareCmdLine(const char *name, const dirent& dir){
    char * path, execPath[256];
    path = new char[11+strlen(dir.d_name)];
    sprintf(path, "/proc/%s/exe", dir.d_name);
    size_t len = getSymLinkFileName(execPath, path); // returns right end of path
    delete[] path;
    if (!len) return false;
    size_t i = len + 1;
    while(i--){
        if (execPath[i] == '/' && (!i || execPath[i-1] != '\\' ))
            break;
    }
    i++;
    memmove(execPath, execPath + i, len - i);
    len -= i;
    execPath[len] = '\0';
    
    if (strlen(name) != len) return false;
    
    for(i = 0; i < len + 1; i++)
        if (name[i] != execPath[i]) return false;
    
    return true;
}
void forFileOfDir(const char * path, std::function<bool(const dirent&)> checkFunc){
    DIR * d = opendir(path);
    struct dirent * dir;
    if (d){
        while ((dir = readdir(d)) != NULL)
        {
            if (checkFunc(*dir))
                break;
        }
        closedir(d);
    }
}

using namespace LibMemory;

Process::Process(int pid) : pid(pid) {}
Process::Process(){}
Process::~Process(){close();}

Process
Process::open(const char *proc, unsigned int index){
    int npid = 0;
    if (!proc[0])
        return Process(npid);
    forFileOfDir("/proc", [proc, &npid, &index](const dirent& dir) -> bool {
        if (isDir(dir) && isNumeric(dir.d_name) && compareCmdLine(proc, dir)){
            if (!index){
                npid = strtol(dir.d_name, 0, 10);
                return true;
            }
            index--;
        }
        return false;
    });
    return Process(npid);
}
Process
Process::openFirst(const char *proc){
    int npid = 0;
    if (!proc[0])
        return Process(npid);
    forFileOfDir("/proc", [proc, &npid](const dirent& dir) -> bool {
        if(isDir(dir) && isNumeric(dir.d_name) && compareCmdLine(proc, dir)){
            npid = strtol(dir.d_name, 0, 10);
            return true;
        }
        return false;
    });
    return Process(npid);
}
Process
Process::openNext(const char *proc) const{
    int npid = 0;
    if (!proc[0])
        return Process(npid);
    forFileOfDir("/proc", [proc, &npid, this](const dirent& dir) -> bool {
        if(isDir(dir) && isNumeric(dir.d_name) && compareCmdLine(proc, dir)){
            int temp = strtol(dir.d_name, 0, 10);
            if(temp <= this->pid)
                return false;
            npid = temp;
            return true;
        }
        return false;
    });
    return Process(npid);
}
Process
Process::openLast(const char *proc){
    int npid = 0;
    if (!proc[0])
        return Process(npid);
    forFileOfDir("/proc", [proc, &npid](const dirent& dir) -> bool {
        if(isDir(dir) && isNumeric(dir.d_name) && compareCmdLine(proc, dir))
            npid = strtol(dir.d_name, 0, 10);
        return false;
    });
    return Process(npid);
}

int
Process::sig(int signal) const { return kill(pid, signal); }

void
Process::close(){

}

Process::operator bool() const {return pid != 0;}
int Process::getpid() const {return pid;}