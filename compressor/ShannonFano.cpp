#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <map>
#include "algorithms.h"

using namespace std;

static string dec2bin(unsigned char n)
{
    string r;
    while(n!=0)
    {
        r=(n%2==0 ?"0":"1")+r;
        n/=2;
    }
    return string(8-r.size(),'0') + r;
}

static unsigned char bin2dec(string s)
{
    string s1 = s.substr(0,s.size());
    unsigned char n = 0;
    char deg = 1;

    while (s1.size()>0)
    {
        n += s1[s1.size()-1]=='1' ? deg : 0;
        deg*=2;
        s1 = s1.substr(0,s1.size()-1);
    }

    return n;
}

static bool comp(pair<char, int> a, pair<char, int> b)
{
    return a.second < b.second;
}

static pair<unsigned char,int> frequences[256]; // символ, частота
static string codes[256];

static void get_freq(string s)
{
    for (unsigned char i : s) frequences[i].second++;
}

static void get_code(string code, int left, int right) // ОГО!!! Я НЕ НАЛАЖАЛ С РЕКУРСИЕЙ И ГРАНИЦАМИ!!!
{
    if (right-left<1)
    {
        codes[frequences[left].first] = code;
        return;
    }
    int sum = 0;
    for (int i = left; i <= right; i++) sum += frequences[i].second;
    int min_dif = sum;
    int min_ind = left;
    for (int i = left; i < right; i++)
    {
        sum -= 2*frequences[i].second;
        if (abs(sum) <= min_dif)
        {
            min_dif = abs(sum);
            min_ind = i;
        }

    }
    get_code(code+"0",left,min_ind);
    get_code(code+"1",min_ind+1,right);
}

string ShF(string filename)
{
    for (int i = 0; i < 256; i++) frequences[i]= {(unsigned char)i,0};
    for (int i = 0; i < 256; i++) codes[i]="";

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
    in = str;
    in_file.close();
    // ввод =======================================================================================

    /// вывод =====================================================================================
    int len = str.size();

    get_freq(str);
    sort(frequences, frequences+256, comp);
    int i = 0;
    while (!frequences[i].second) i++;
    if (i == 255) get_code("0",255,255);
    else get_code("",i,255);


    ofstream out_file("..\\tests\\ShF_out\\"+filename+".min",
                      ios_base::out | ios_base::trunc|ios_base::binary);
    if (!out_file.is_open()) return "FAIL2;;;";


    out_file.write((char*)&len, sizeof(len));
    string buff = "";
    int t = 0;

    for (int i = 0; i < 256; i++) if (codes[i].size()>0)
        {
            buff += dec2bin(i);
            buff += dec2bin(codes[i].size());
            buff+=codes[i];
            t+=1;
        }

    buff = dec2bin(t)+buff;

    unsigned char next = 0;
    while (buff.size()>=8)
    {
        next = bin2dec(buff.substr(0,8));
        buff.erase(0,8);
        out_file.write((char *)&next,sizeof(next));
    }

    for (unsigned char i : str)
    {
        buff += codes[i];
        while (buff.size()>=8)
        {
            next = bin2dec(buff.substr(0,8));
            buff.erase(0,8);
            out_file.write((char *)&next,sizeof(next));
        }
    }
    while (buff.size()<8) buff +='0';
    next = bin2dec(buff.substr(0,8));
    buff.erase(0,8);
    out_file.write((char *)&next,sizeof(next));
    out_file.close();
    // вывод ======================================================================================

    result += to_string((float)(clock()-start)/1000);
    start = clock();

    /// раскодирование для замера =================================================================

    ifstream bin_file("..\\tests\\ShF_out\\"+filename+".min", ios_base::binary);
    if (!bin_file.is_open()) return "FAIL3;;;";

    bin_file.seekg(0, ios_base::end);
    int res_size = bin_file.tellg();
    bin_file.seekg(0, ios_base::beg);

    unsigned char bytes[res_size];
    bin_file.read((char*)&len,sizeof(len));
    bin_file.read((char*)bytes, res_size);
    map<string,unsigned char> keys;
    str = "";
    int pointer = 0;

    unsigned char symb_num = bytes[pointer++];
    for (int i = 0; i < symb_num; i++)
    {
        while (str.size() < 16) str += dec2bin(bytes[pointer++]);
        unsigned char cur = bin2dec(str.substr(0,8));
        str.erase(0,8);
        unsigned char code_len = bin2dec(str.substr(0,8));
        str.erase(0,8);
        while (str.size()<code_len) str += dec2bin(bytes[pointer++]);


        keys[str.substr(0,code_len)]=cur;
        str.erase(0,code_len);
    }

    string res = "";
    int code_len = 1;
    for (int i =0; i<len; i++)
    {
        while (str.size()<256 && pointer < res_size) str += dec2bin(bytes[pointer++]);
        while (keys.count(str.substr(0,code_len)) == 0 && code_len<str.size()) code_len++;
        res+=keys[str.substr(0,code_len)];
        str.erase(0,code_len);
        code_len=1;
    }
    out = res;

    bin_file.close();
    // раскодирование для замера ==================================================================
    result += ";" + to_string((float)(clock()-start)/1000);
    result+=";"+to_string((float)src_size/res_size);
    result+=((in==out) ? ";OK" : ";ERR");

    return result;
}
