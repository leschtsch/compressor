#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
#include <algorithm>
#include "algorithms.h"

using namespace std;


static unsigned char alphabet[256];


static unsigned char mtf_direct(unsigned char letter)
{
    unsigned char ind = 0;
    while (alphabet[(int)ind]!=letter) ind++;
    for (int i = ind; i>0; i--) alphabet[i]=alphabet[i-1];
    alphabet[0] = letter;
    return ind;
}

static unsigned char mtf_reverse(unsigned char ind)
{
    unsigned char letter = alphabet[int(ind)];
    for (int i = ind; i>0; i--) alphabet[i]=alphabet[i-1];
    alphabet[0] = letter;
    return letter;
}



string mtf(string filename)
{
    for (int i = 0; i<256; i++) alphabet[i]=(unsigned char)i;


    string in, out;
    string result = "";
    float start = clock();

    /// ввод ======================================================================================
    ifstream in_file("..\\tests\\input\\"+filename,ios_base::binary);
    if (!in_file.is_open()) return "FAIL1;;;";

    in_file.seekg(0, ios_base::end);
    int src_size = in_file.tellg();
    in_file.seekg(0, ios_base::beg);

    string line, transformed;
    string str = "";
    while (getline(in_file, line)) str+=line+"\n";
    str.erase(str.length()-1,1);
    in = str;
    for (unsigned int i = 0; i<str.size(); i++)transformed+=mtf_direct(str[i]);

    in_file.close();
    // ввод =======================================================================================

    /// вывод =====================================================================================
    ofstream out_file("..\\tests\\MTF_out\\"+filename+".min",
                      ios_base::out | ios_base::trunc|ios_base::binary);
    if (!out_file.is_open()) return "FAIL2;;;";

    unsigned char counter = 0;
    unsigned char current = transformed[0];
    for (unsigned int pointer = 0; pointer<transformed.length(); pointer++)
    {
        if (current==transformed[pointer])counter++;
        else
        {
            out_file.write((char *)&counter,sizeof(counter));
            out_file.write((char *)&current,sizeof(current));
            counter=1;
            current=transformed[pointer];
        }
        if (counter==255)
        {
            out_file.write((char *)&counter,sizeof(counter));
            out_file.write((char *)&current,sizeof(current));
            counter=0;
        }
    }

    out_file.write((char *)&counter,sizeof(counter));
    out_file.write((char *)&current,sizeof(current));
    counter=1;
    out_file.close();
    // вывод ======================================================================================

    result += to_string((float)(clock()-start)/1000);
    for (int i = 0; i<256; i++) alphabet[i]=(char)i;
    start = clock();

    /// раскодирование для замера =================================================================
    ifstream bin_file("..\\tests\\MTF_out\\"+filename+".min", ios_base::binary);
    if (!bin_file.is_open()) return "FAIL3;;;";

    bin_file.seekg(0, ios_base::end);
    int res_size = bin_file.tellg();
    bin_file.seekg(0, ios_base::beg);

    unsigned char bytes[res_size];
    bin_file.read((char*)bytes, res_size);

    str="";
    transformed = "";
    for (int i = 0; i < res_size; i+=2) str += string(bytes[i], bytes[i+1]);

    for (unsigned int i=0; i < str.size(); i++) transformed += mtf_reverse(str[i]);
    out = transformed;

    bin_file.close();

    // раскодирование для замера ==================================================================

    result += ";" + to_string((float)(clock()-start)/1000);
    result+=";"+to_string((float)src_size/res_size);
    result+=((in==out) ? ";OK" : ";ERR");

    return result;
}
