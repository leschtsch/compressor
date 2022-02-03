#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
#include "RLE.h"


using namespace std;

string rle(string filename)
{
    string result = "";
    unsigned int start = clock();

    /// ввод
    ifstream in_file("C:\\Users\\ПК\\Desktop\\проги\\проект 10.2\\compressor\\tests\\input\\"+filename+".txt");
    if (!in_file.is_open()) return "FAIL1";
    in_file.seekg(0, ios_base::end);
    int src_size = in_file.tellg();
    in_file.seekg(0, ios_base::beg);
    string line;
    string str = "";
    while (getline(in_file, line)) str+=line+"\n";
    str.erase(str.length()-1,1);
    //cout << str << endl;
    in_file.close();
    // ввод

    /// вывод
    ofstream out_file("C:\\Users\\ПК\\Desktop\\проги\\проект 10.2\\compressor\\tests\\RLE_out\\"+filename+".min",
                      ios_base::out | ios_base::trunc|ios_base::binary);
    if (!out_file.is_open()) return "FAIL2";

    unsigned char counter = 0;
    unsigned char current = str[0];
    for (unsigned int pointer = 0; pointer<str.length(); pointer++)
    {
        if (current==str[pointer])counter++;
        else
        {
            out_file.write((char *)&counter,sizeof(counter));
            out_file.write((char *)&current,sizeof(current));
            counter=1;
            current=str[pointer];
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
    // вывод

    result += to_string((float)(clock()-start)/1000);
    start = clock();

    /// раскодирование для замера
    ifstream bin_file("C:\\Users\\ПК\\Desktop\\проги\\проект 10.2\\compressor\\tests\\RLE_out\\test1.min", ios_base::binary);
    if (!bin_file.is_open()) return "FAIL3";
    bin_file.seekg(0, ios_base::end);
    int res_size = bin_file.tellg();
    bin_file.seekg(0, ios_base::beg);
    unsigned char bytes[res_size];
    bin_file.read((char*)bytes, res_size);
    str="";
    for (int i = 0; i < res_size; i+=2) str += string(bytes[i],bytes[i+1]);
    // раскодирование для замера

    result += "/" + to_string((float)(clock()-start)/1000);
    result+="/"+to_string((float)src_size/res_size);

    return result;
}
