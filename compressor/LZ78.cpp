#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
#include <map>
#include "algorithms.h"

using namespace std;

string lz78(string filename)
{
    string in, out;
    string result = "";
    float start = clock();

    /// ввод ======================================================================================
    ifstream in_file("..\\tests\\input\\"+filename,ios_base::binary);
    if (!in_file.is_open()) return "FAIL1;;;";

    in_file.seekg(0, ios_base::end);
    int src_size = in_file.tellg();
    in_file.seekg(0, ios_base::beg);

    string line;
    string str = "";
    while (getline(in_file, line)) str+=line+"\n";
    str.erase(str.length()-1,1);
    int len = str.size();

    in = str;
    in_file.close();
    // ввод =======================================================================================

    /// вывод =====================================================================================
    ofstream out_file("..\\tests\\LZ78_out\\"+filename+".min",
                      ios_base::out | ios_base::trunc|ios_base::binary);
    if (!out_file.is_open()) return "FAIL2;;;";

    map<string, unsigned short> dict;
    unsigned short ds = 0;
    dict[""] = ds++;
    string buff = "";

    out_file.write((char*)&len, sizeof(len));
    while (str.size() > 0)
    {
        while (str.size() > 0 && dict.count(buff)>0)
        {
            buff+=str[0];
            str.erase(0,1);
        }
        dict[buff]=ds++;
        unsigned short n1 = dict[buff.substr(0,buff.size()-1)];
        out_file.write((char*)&n1, sizeof(n1));
        char n2 = buff[buff.size()-1];
        out_file.write((char*)&n2,sizeof(n2));
        buff = "";
        if (dict.size()>65000)
        {
            while (dict.size()>0) dict.erase(dict.begin());
            ds = 0;
            dict[""] = ds++;
            buff = "";
        }
    }
    out_file.close();
    // вывод ======================================================================================

    result += to_string((float)(clock()-start)/1000);
    start = clock();

    /// раскодирование для замера =================================================================

    ifstream bin_file("..\\tests\\LZ78_out\\"+filename+".min", ios_base::binary);
    if (!bin_file.is_open()) return "FAIL3;;;";

    bin_file.seekg(0, ios_base::end);
    int res_size = bin_file.tellg();
    bin_file.seekg(0, ios_base::beg);

    map<unsigned short, string> reverse_dict;
    ds = 0;
    reverse_dict[ds++]="";
    str = "";

    bin_file.read((char*)&len, sizeof(len));
    for (int i =0; i<res_size; i+=3)
    {
        unsigned short n1;
        bin_file.read((char*)&n1, sizeof(n1));
        char n2;
        bin_file.read((char*)&n2, sizeof(n2));
        reverse_dict[ds++] = reverse_dict[n1]+n2;
        str+=reverse_dict[n1]+n2;
        if (reverse_dict.size()>65000)
        {
            while (reverse_dict.size()>0) reverse_dict.erase(reverse_dict.begin());
            ds = 0;
            reverse_dict[ds++] = "";
        }
    }
    str=str.substr(0,len);

    out = str;
    // раскодирование для замера ==================================================================

    result += ";" + to_string((float)(clock()-start)/1000);
    result+=";"+to_string((float)src_size/res_size);
    result+=((in==out) ? ";OK" : ";ERR");
    return result;
}
