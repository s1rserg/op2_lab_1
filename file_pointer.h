#ifndef FILE_POINTER_H
#define FILE_POINTER_H

#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;
#define MAX 1024

namespace fp {
    void menu();

    void inputFile(char *fileN);

    void outputFile(char *fileN);

    void processFile(char *fileN);

    void swapSubstrings(char *str, char *substrings[], int numSubstrings);

    void moveSubstringToEnd(char *str, char *substrings[], int numSubstrings);
}

#endif
