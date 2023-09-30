#include <ncurses.h>
#include <vector>
#include <filesystem>
#include "StringUtils.h"
#include "Menu.h"

using namespace std;
namespace fs = std::filesystem;


int main() {
    vector<Process> processList = Process::scan();
    Menu menu = Menu(&processList);
    Process* processToKill = menu.show();
    processToKill->kill();
    return 0;
}

