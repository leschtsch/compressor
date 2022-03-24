#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>

using namespace std;

static int test_num = 0;

static int random(int a, int b)
{
    return (int)(a+(double)(b-a+1)*rand()/(RAND_MAX+1));
}

static void one_symbol(int q)
/**
    Эта функция создает тесты в виде строки одинаковых символов случайной длины.
    Параметры:
        q - количество пар тестов
*/
{
    for (int i = 0; i < q; i++)
    {
        ofstream out_file("..\\tests\\input\\one_symbol"+to_string(test_num++)+".txt");
        if (!out_file.is_open()) continue;

        string s = string(random(100,1000),(char)(97+random(0,25)));
        out_file << s;

        out_file.close();
    }
    for (int i = 0; i < q; i++)
    {
        ofstream out_file("..\\tests\\input\\one_symbol_big"+to_string(test_num++)+".txt");
        if (!out_file.is_open()) continue;

        string s = string(random(10000,100000),(char)(97+random(0,25)));
        out_file << s;

        out_file.close();
    }
}

static void grouped_symbols(int q, int group_size)
/**
    Эта функция создает тесты, где символы повторяются по несколько раз подряд.
    Параметры:
        q - количество пар тестов
        group_size - как много раз(максимум) может повториться символ. В процентах от длины теста.
*/
{
    int gr_sz;
    for (int i = 0; i < q; i++)
    {

        ofstream out_file("..\\tests\\input\\grouped_symbols"+to_string(test_num++)+".txt");
        if (!out_file.is_open()) continue;

        string s = "";
        int c = random(100,1000);
        gr_sz = (c*group_size+99)/100;
        int n;
        while (c>0)
        {
            n = random(1,min(gr_sz,c));
            c-=n;
            s+= string(n, (char)(97+random(0,25)));
        }
        out_file << s;

        out_file.close();
    }
    for (int i = 0; i < q; i++)
    {
        ofstream out_file("..\\tests\\input\\grouped_symbols_big"+to_string(test_num++)+".txt");
        if (!out_file.is_open()) continue;

        string s = "";
        int c = random(10000,100000);
        gr_sz = (c*group_size+99)/100;
        int n;
        while (c>0)
        {
            n = random(1,min(gr_sz,c));
            c-=n;
            s+= string(n, (char)(97+random(0,25)));
        }
        out_file << s;

        out_file.close();
    }
}

static void random_test(int q)
/**
    Эта функция создает тесты в виде случайных строк.
    Параметры:
        q - количество пар тестов
*/
{
    for (int i = 0; i < q; i++)
    {
        ofstream out_file("..\\tests\\input\\random"+to_string(test_num++)+".txt");
        if (!out_file.is_open()) continue;

        string s = "";

        for (int j = 0; j < random(100,1000); j++)
        {
            s += (char)(97+random(0,25));
        }

        out_file << s;

        out_file.close();
    }
    for (int i = 0; i < q; i++)
    {
        ofstream out_file("..\\tests\\input\\random_big"+to_string(test_num++)+".txt");
        if (!out_file.is_open()) continue;

        string s = "";

        for (int j = 0; j < random(10000,100000); j++)
        {
            s += (char)(97+random(0,25));
        }

        out_file << s;

        out_file.close();
    }
}

static void sequence(int q, int seq_len)
/**
    Эта функция создает тесты в виде повторяющейся последовательности символов.
    Параметры:
        q - количество пар тестов
        seq_len - длина повторяющейся последовательности

*/
{
    for (int i = 0; i < q; i++)
    {

        ofstream out_file("..\\tests\\input\\sequence"+to_string(test_num++)+".txt");
        if (!out_file.is_open()) continue;

        string s = "";
        int c = random(100,1000);
        string seq = "";
        for (int i = 0;i<seq_len;i++) seq+=(char)(97+random(0,25));
        while (c>0)
        {
            s+=seq;
            c-= seq_len;
        }
        out_file << s;

        out_file.close();
    }
    for (int i = 0; i < q; i++)
    {

        ofstream out_file("..\\tests\\input\\sequence_big"+to_string(test_num++)+".txt");
        if (!out_file.is_open()) continue;

        string s = "";
        int c = random(10000,100000);
        string seq = "";
        for (int i = 0;i<seq_len;i++) seq+=(char)(97+random(0,25));
        while (c>0)
        {
            s+=seq;
            c-= seq_len;
        }
        out_file << s;

        out_file.close();
    }
}

static void not_random(int q, string alphabet)
/**
    Эта функция создает тесты где символы встречаются с разной частотой.
    Параметры:
        q - количество пар тестов
        alphabet - строка, из которой выбираются символы. Например "aab" - a встречается в два разачаще, чем b
*/
{

    for (int i = 0; i < q; i++)
    {
        ofstream out_file("..\\tests\\input\\not_random"+to_string(test_num++)+".txt");
        if (!out_file.is_open()) continue;

        string s = "";

        for (int j = 0; j < random(100,1000); j++)
        {
            s += alphabet[random(0,alphabet.size()-1)];
        }

        out_file << s;

        out_file.close();
    }
    for (int i = 0; i < q; i++)
    {
        ofstream out_file("..\\tests\\input\\not_random_big"+to_string(test_num++)+".txt");
        if (!out_file.is_open()) continue;

        string s = "";

        for (int j = 0; j < random(10000,100000); j++)
        {
            s += alphabet[random(0,alphabet.size()-1)];
        }

        out_file << s;

        out_file.close();
    }
}

int main()
{
    srand((int)(time(0)*1000));
    one_symbol(5);
    random_test(5);
    for (int i = 1; i <= 10; i++) grouped_symbols(3,i*10);
    sequence(3,10);
    sequence(3,100);
    sequence(3,1000);
    not_random(3, "aaaabbc");
    not_random(3, "helloworld");
    not_random(3, "someexampletext");
    not_random(3, "disarraycalumniatordiviner");
    not_random(3, "barmanexhilarationcauseless");
    system("pause");
    return 0;
}
