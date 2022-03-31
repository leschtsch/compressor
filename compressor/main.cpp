#include <iostream>
#include <windows.h>
#include <fstream>
#include "algorithms.h"
using namespace std;
int main()
{
    ofstream result("..\\tests\\result.csv");
    if (!result.is_open()) return -1;
    result << ";;RLE;;;;;BWT-RLE;;;;;MTF;;;;;BWT_MTF;;;;;Shannon_Fano;;;;;Huffman;;;;;LZ78;;;;;LZ77" << endl;
    result << ";;t סז;t נאחז;k;res";
    result << ";;t סז;t נאחז;k;res";
    result << ";;t סז;t נאחז;k;res";
    result << ";;t סז;t נאחז;k;res";
    result << ";;t סז;t נאחז;k;res";
    result << ";;t סז;t נאחז;k;res";
    result << ";;t סז;t נאחז;k;res";
    result << ";;t סז;t נאחז;k;res" << endl << endl;
    string filename, res;
    string buff = "";
    WIN32_FIND_DATA FindFileData;
    HANDLE hf;
    hf=FindFirstFile("..\\tests\\input\\*.txt", &FindFileData);
    if (hf==INVALID_HANDLE_VALUE) return -1;

    do
    {
        filename = FindFileData.cFileName;

        filename.erase(filename.length()-4,4); // סעונועü .txt
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

        res = ShF(filename);
        result <<  res << ";;";
        buff+="Shannon_Fano:\t" + res+"\n";

        res = Huff(filename);
        result <<  res << ";;";
        buff+="Huff:\t\t" + res+"\n";

        res = lz78(filename);
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

    system("pause");
    return 0;
}
