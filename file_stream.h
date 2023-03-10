#ifndef FILE_STREAM_H
#define FILE_STREAM_H

#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

using namespace std;
#define MAX 1024

namespace fs {
    void menu();

    void inputFile(string name);

    void outputFile(string name);

    void processFile(string name);

    void swapSubstrings(string &str, string substrings[], int numSubstrings);

    void moveSubstringToEnd(string &str, string substrings[], int numSubstrings);
}

#endif