//
// Created by daniel on 19.9.23.
//
#include "StringUtils.h"
using namespace std;

vector<string> split(const string& text) {
    vector<string> resultList;
    stringstream ss(text);
    string word;
    while (ss >> word) { // Extract word from the stream.
        resultList.push_back(word);
    }
    return resultList;
}

vector<string> split(const string& text, char delimiter) {
    vector<string> resultList;
    stringstream ss(text);
    string temp;
    while (getline(ss, temp, delimiter)) {
        resultList.push_back(temp);
    }
    return resultList;
}

long long hex2dec(std::string hex)
{
    std::string::size_type sz = 0;
    try
    {
        hex = "0x"s + hex;
        return std::stoll(hex, &sz, 16);
    }
    catch (...)
    {
        return 0;
    }
}