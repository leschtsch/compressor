#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
#include "algorithms.h"


using namespace std;

static pair<int, int> get_code(string buff, string str)
{
    pair<int,int> res = {0,0};
    for (int i = 0; i < buff.size(); i++)
    {
        string temp_buff = buff.substr(i,buff.size()-i);
        int str_pos = 0;
        while (str_pos < str.size() && str_pos<4095 && temp_buff[0]==str[str_pos])
        {
            str_pos+=1;
            temp_buff+=temp_buff[0];
            temp_buff.erase(0,1);
        }
        if (str_pos>res.second)res = {buff.size()-i,str_pos};
    }
    return res;
}

string lz77(string filename)
{
    string in, out;
    string result = "";
    float start = clock();

    /// ���� ======================================================================================
    ifstream in_file("..\\tests\\input\\"+filename+".txt");
    if (!in_file.is_open()) return "FAIL1;;";

    in_file.seekg(0, ios_base::end);
    int src_size = in_file.tellg();
    in_file.seekg(0, ios_base::beg);

    string line;
    string str = "";
    while (getline(in_file, line)) str+=line+"\n";
    str.erase(str.length()-1,1);
    in = str;


    in_file.close();
    // ���� =======================================================================================

    /// ����� =====================================================================================
    ofstream out_file("..\\tests\\LZ77_out\\"+filename+".min",
                      ios_base::out | ios_base::trunc|ios_base::binary);
    if (!out_file.is_open()) return "FAIL2;;";

    string buff = "";
    while (str.size()>0)
    {
        buff.erase(0,max((int) buff.size()-4095,0));
        pair<unsigned int,unsigned int> code = get_code(buff,str);
        buff += str.substr(0,code.second);
        str.erase(0,code.second);
        char next = str[0];
        buff += next;
        str.erase(0,1);

        unsigned short a = (code.first << 4) + (code.second >> 8);
        unsigned char b = (unsigned char)code.second;
        out_file.write((char*)&a, sizeof(a));
        out_file.write((char*)&b, sizeof(b));
        out_file.write((char*)&next, sizeof(next));
    }
    out_file.close();
    // ����� ======================================================================================

    result += to_string((float)(clock()-start)/1000);
    start = clock();

    /// �������������� ��� ������ =================================================================
    ifstream bin_file("..\\tests\\LZ77_out\\"+filename+".min", ios_base::binary);
    if (!bin_file.is_open()) return "FAIL3;;";

    bin_file.seekg(0, ios_base::end);
    int res_size = bin_file.tellg();
    bin_file.seekg(0, ios_base::beg);

    buff = "";
    str = "";
    for (int i = 0; i < res_size; i+=4)
    {
        unsigned short a;
        unsigned char b;
        char next;
        pair<unsigned int,unsigned int> code;
        bin_file.read((char*)&a,sizeof(a));
        bin_file.read((char*)&b,sizeof(b));
        bin_file.read((char*)&next,sizeof(next));

        code.first = a>>4;
        code.second = a;
        code.second = (code.second << 28) >> 20;
        code.second+=b;

        code.first = buff.size()-code.first;
        for (int i = 0; i < code.second; i++)
        {
            str+=buff[code.first];
            buff += buff[code.first++];
        }
        str+=next;
        buff += next;
        buff.erase(0,max((int) buff.size()-4095,0));
    }
    if (str[str.size()-1]==0) str.erase(str.size()-1,1);
    out = str;
    // �������������� ��� ������ ==================================================================
    result += ";" + to_string((float)(clock()-start)/1000);
    result+=";"+to_string((float)src_size/res_size);
    result+=((in==out) ? ";OK" : ";ERR");
    return result;
}
