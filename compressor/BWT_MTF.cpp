#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
#include <algorithm>
#include "algorithms.h"

using namespace std;


char alphabet1[256];
int BLOCK_SIZE1 = 1024;
char EOL1 = (char) 0;


char mtf_direct1(char letter)
{
    char ind = 0;
    while (alphabet1[int(ind)]!=letter) ind++;
    for (int i = ind; i>0; i--) alphabet1[i]=alphabet1[i-1];
    alphabet1[0] = letter;
    return ind;
}

char mtf_reverse1(char ind)
{
    char letter = alphabet1[int(ind)];
    for (int i = ind; i>0; i--) alphabet1[i]=alphabet1[i-1];
    alphabet1[0] = letter;
    return letter;
}

string bwt_direct1(string s)
{
    s += EOL1;
    string matrix[s.size()];
    for (unsigned int i =0; i < s.size(); i++) matrix[i] = s.substr(i,s.size()-i)+s.substr(0,i);
    sort(matrix,matrix+s.size());
    string res = "";
    for (unsigned int i =0; i < s.size(); i++) res+=matrix[i][s.size()-1];
    return res;
}


string bwt_reverse1(string s)
{
    pair<char, int> last_col[s.size()];
    int ind;
    for (unsigned int i = 0; i < s.size(); i++)
    {
        last_col[i] = pair<char,int> (s[i],i);
        if (s[i]==EOL1)
        {
            ind = i;
        }
    }
    sort(last_col, last_col+s.size());
    int translations[s.size()];
    for (unsigned int i = 0; i < s.size(); i++) translations[last_col[i].second] = i;
    ind = translations[ind];
    string res = "";
    for (unsigned int i=0; i<s.size()-1; i++)
    {
        res=s[ind]+res;
        ind = translations[ind];
    }
    return res;
}


string bwt_mtf(string filename)
{
    for (int i = 0; i<256; i++) alphabet1[i]=(char)i;



    string result = "";
    unsigned int start = clock();

    /// ����
    ifstream in_file("..\\tests\\input\\"+filename+".txt");
    if (!in_file.is_open()) return "FAIL1;;";
    in_file.seekg(0, ios_base::end);
    int src_size = in_file.tellg();
    in_file.seekg(0, ios_base::beg);
    string line, transformed;
    string str = "";
    while (getline(in_file, line)) str+=line+"\n";
    str.erase(str.length()-1,1);
    for (unsigned int i = 0; i<str.size(); i+=BLOCK_SIZE1) transformed+=bwt_direct1(str.substr(i,BLOCK_SIZE1));
    str = transformed;
    transformed = "";
    for (unsigned int i = 0; i<str.size(); i++) transformed+=mtf_direct1(str[i]);
    in_file.close();
    // ����

    /// �����
    ofstream out_file("..\\tests\\BWT_MTF_out\\"+filename+".min",
                      ios_base::out | ios_base::trunc|ios_base::binary);
    if (!out_file.is_open()) return "FAIL2;;";

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
    // �����

    result += to_string((float)(clock()-start)/1000);
    for (int i = 0; i<256; i++) alphabet1[i]=(char)i;
    start = clock();

    /// �������������� ��� ������
    ifstream bin_file("..\\tests\\BWT_MTF_out\\"+filename+".min", ios_base::binary);
    if (!bin_file.is_open()) return "FAIL3;;";
    bin_file.seekg(0, ios_base::end);
    int res_size = bin_file.tellg();
    bin_file.seekg(0, ios_base::beg);
    unsigned char bytes[res_size];
    bin_file.read((char*)bytes, res_size);
    str="";
    transformed = "";
    for (int i = 0; i < res_size; i+=2) str += string(bytes[i],bytes[i+1]);
    for (unsigned int i=0; i < str.size(); i++) transformed += mtf_reverse1(str[i]);
    str=transformed;
    transformed = "";
    for (unsigned int i = 0; i<str.size(); i+=BLOCK_SIZE1+1) transformed+=bwt_reverse1(str.substr(i,BLOCK_SIZE1+1));
    bin_file.close();
    // �������������� ��� ������

    result += ";" + to_string((float)(clock()-start)/1000);
    result+=";"+to_string((float)src_size/res_size);
    return result;
}