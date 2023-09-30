#include <vector>
#include <array>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <map>
#include <fstream>
#include <filesystem>
#include "Process.h"
#include "StringUtils.h"

namespace fs = std::filesystem;

unsigned long Process::largestPortSize;

Process::Process(string jcmdLine) {
    char delimiter = ' ';
    stringstream ss(jcmdLine);
    string temp;
    getline(ss, temp, delimiter);
    pid = stoi(temp);
    getline(ss, temp, delimiter);
    mainClass = temp;
    getline(ss, temp, delimiter);
    path = temp;
    port = openedPorts();
}

vector<Process> Process::scan() {
    vector<Process> processList;
    string str = Process::exec("jcmd");
    char delimiter = '\n';
    stringstream ss(str);
    string temp;
    while (getline(ss, temp, delimiter)) {
        Process p = Process(temp);
        processList.push_back(p);
        unsigned long portSize = p.port.size();
        if (portSize > largestPortSize) largestPortSize = portSize;
    }
    return processList;
}

string Process::normalizedPort() const {
    return string(largestPortSize - port.size(), ' ').append(port);
}

string Process::exec(const char *cmd) {
    array<char, 128> buffer{};
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

void Process::kill() {
    string killCmd = "kill -9 " + to_string(this->pid);
    Process::exec(killCmd.c_str());
}

map<std::string, long> scanInodes(const char *path, map<std::string, long> inodeMap) {
    std::ifstream proc_net_tcp;

    proc_net_tcp.open(path);
    string myline;

    if (proc_net_tcp.is_open()) {
        while (proc_net_tcp) {
            getline(proc_net_tcp, myline);
            if (myline.empty()) {
                continue;
            }

            vector<string> res = split(myline);
            if (res[0] == "sl") {
                continue;
            }

            long listeningPort = hex2dec(split(res[1], ':')[1]);
            string inode = res[9];

            inodeMap[inode] = listeningPort;
        }
    }
    proc_net_tcp.close();
    return inodeMap;
}

string Process::openedPorts() const {
    string result;
    std::map<std::string, long> inodeMap;

    inodeMap = scanInodes("/proc/net/tcp", inodeMap);
    inodeMap = scanInodes("/proc/net/tcp6", inodeMap);

    std::string fileDescriptorPath = "/proc/" + to_string(this->pid) + "/fd";

    if (!fs::exists(fileDescriptorPath)) {
        return "";
    }

    for (const auto &entry: fs::directory_iterator(fileDescriptorPath)) {
        if (!entry.exists()) {
            continue;
        }
        if (!entry.is_symlink()) {
            continue;
        }
        string fdSymlinkName = fs::read_symlink(entry).filename().generic_string();
        vector<string> fdTokens = split(fdSymlinkName, ':');
        if (fdTokens[0] != "socket") continue;
        string inode = fdTokens[1].substr(1, fdTokens[1].size() - 2);
        long openedPort = inodeMap[inode];
        if (openedPort == 0) continue;
        result.append(to_string(openedPort)).append(";");
    }
    return result.substr(0, result.size() - 1);
}
