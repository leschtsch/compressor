#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <map>
#include <set>
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

static pair<unsigned char,int> frequences[256]; // символ, частота
static string codes[256];

static void get_freq(string s)
{
    for (unsigned char i : s) frequences[i].second++;
}

static void get_code()
{
    multiset<pair<int, string>> tree;
    for (pair<unsigned char,int> i : frequences) if (i.second != 0) tree.insert(pair<int, string>(i.second, string(1,i.first)));
    if (tree.size()==1)
    {
        pair<int, string> letter = *tree.begin();
        codes[letter.second[0]] = "1";
        return;
    }

    while(tree.size()>1)
    {
        pair<int, string> left = *tree.begin();
        tree.erase(tree.begin());
        for (unsigned char i:left.second) codes[i]="0"+codes[i];
        pair<int, string> right = *tree.begin();
        for (unsigned char i:right.second) codes[i]="1"+codes[i];
        tree.erase(tree.begin());
        pair<int, string> new_;
        new_.first=left.first+right.first;
        new_.second=left.second+right.second;
        tree.insert(new_);
    }
    return;
}

string Huff(string filename)
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
    get_code();
    ofstream out_file("..\\tests\\Huff_out\\"+filename+".min",
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

    ifstream bin_file("..\\tests\\Huff_out\\"+filename+".min", ios_base::binary);
    if (!bin_file.is_open()) return "FAIL3;;;";

    bin_file.seekg(0, ios_base::end);
    int res_size = bin_file.tellg();
    bin_file.seekg(0, ios_base::beg);

    unsigned char bytes[res_size];
    bin_file.read((char*)&len,sizeof(len));
    bin_file.read((char*)bytes, res_size);
    map<string,char> keys;
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
        if (keys.count(str.substr(0,code_len)) == 0) break;
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
