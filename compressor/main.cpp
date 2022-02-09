#include <iostream>
#include <windows.h>
#include <fstream>
#include "algorithms.h"
using namespace std;
int main()
{
    ofstream result("tests\\result.csv");
    if (!result.is_open()) return -1;
    result << ";;RLE;;;;BWT-RLE;;;" << endl;

    WIN32_FIND_DATA FindFileData;
    HANDLE hf;
    hf=FindFirstFile("tests\\input\\*.txt", &FindFileData);
    if (hf==INVALID_HANDLE_VALUE) return -1;
    do
    {
        string filename = FindFileData.cFileName;
        filename.erase(filename.length()-4,4);
        result << filename << ";;";
        result << rle(filename) << ";;";
        result << bwt(filename) << endl;
    }
    while (FindNextFile(hf,&FindFileData)!=0);
    result.close();
    FindClose(hf);
    system("pause");
    return 0;
}
