#include <iostream>
#include <windows.h>
#include <fstream>
#include "algorithms.h"
using namespace std;
int main()
{
    ofstream result("..\\tests\\result.csv");
    if (!result.is_open()) return -1;
    result << ";;RLE;;;;BWT-RLE;;;;MTF;;;;BWT_MTF" << endl;
    string filename, res;

    WIN32_FIND_DATA FindFileData;
    HANDLE hf;
    hf=FindFirstFile("..\\tests\\input\\*.txt", &FindFileData);
    if (hf==INVALID_HANDLE_VALUE) return -1;
    do
    {
        filename = FindFileData.cFileName;

        filename.erase(filename.length()-4,4);
        result << filename << ";;";
        cout << filename << ":" << endl;

        res = rle(filename);
        result << res << ";;";
        cout << "RLE:" << " " << res << endl;

        res = bwt(filename);
        result <<  res << ";;";
        cout << "BWT:" << " " << res << endl;

        res = mtf(filename);
        result <<  res << ";;";
        cout << "MTF:" << " " << res << endl;

        res = bwt_mtf(filename);
        result <<  res << "" << endl;
        cout << "BWT_MTF:" << " " << res << endl;

        cout << endl;
    }
    while (FindNextFile(hf,&FindFileData)!=0);
    result.close();
    FindClose(hf);
    system("pause");
    return 0;
}
