#include <iostream>
#include <windows.h>
#include <fstream>
#include <ctime>
#include "algorithms.h"

using namespace std;

int main()
{
    float start = clock();

    ofstream result("..\\tests\\result.csv");
    if (!result.is_open()) return -1;
    result << ";;RLE;;;;;BWT-RLE;;;;;MTF-RLE;;;;;BWT-MTF-RLE;;;;;Shannon_Fano;;;;;Huffman;;;;;LZ78;;;;;LZ77" << endl;
    result << ";;t сж;t расж;k;res";
    result << ";;t сж;t расж;k;res";
    result << ";;t сж;t расж;k;res";
    result << ";;t сж;t расж;k;res";
    result << ";;t сж;t расж;k;res";
    result << ";;t сж;t расж;k;res";
    result << ";;t сж;t расж;k;res";
    result << ";;t сж;t расж;k;res" << endl << endl;
    string filename, res;
    string buff = "";
    WIN32_FIND_DATA FindFileData;
    HANDLE hf;
    hf=FindFirstFile("..\\tests\\input\\*.*", &FindFileData);
    if (hf==INVALID_HANDLE_VALUE) return -1;

    do
    {
        filename = FindFileData.cFileName;
        if (filename == "." or filename == "..") continue;

        result << filename << ";;";
        buff += filename + ":\n";

        res = rle(filename);
        result << res << ";;";
        buff+="RLE:\t\t" + res+"\n";

        res = bwt(filename);
        result <<  res << ";;";
        buff+="BWT:\t\t" + res+"\n";

        res = mtf(filename);
        result <<  res << ";;";
        buff+="MTF:\t\t" + res+"\n";

        res = bwt_mtf(filename);
        result <<  res << ";;";
        buff+="BWT_MTF:\t" + res+"\n";


        res = ShF(filename); // это
        result <<  res << ";;";
        buff+="Shannon_Fano:\t" + res+"\n";

        res = Huff(filename); // это
        result <<  res << ";;";
        buff+="Huff:\t\t" + res+"\n";

        res = lz78(filename); // это
        result <<  res << ";;";
        buff+="LZ78:\t\t" + res+"\n";

        res = lz77(filename);
        result <<  res << "" << endl;
        buff+="LZ77:\t\t" + res+"\n";

        cout << buff<< endl;
        buff="";

    }
    while (FindNextFile(hf,&FindFileData)!=0);

    result.close();
    FindClose(hf);

    cout << "\a";
    cout << "time: " << to_string((float)(clock()-start)/1000)<<endl;;
    system("pause");
    return 0;
}
