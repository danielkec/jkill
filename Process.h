//
// Created by daniel on 16.9.23.
//
#include <cstdio>
#include <cstring>
#include <sstream>

using namespace std;



class Process {
    [[nodiscard]] string openedPorts() const;


public:
    explicit Process(string jcmdLine);
    [[nodiscard]] string normalizedPort() const;
    void kill();
    static string exec(const char *cmd);
    static vector<Process> scan();
    int pid;
    string mainClass;
    string path;
    string port;
    static unsigned long largestPortSize;
};
