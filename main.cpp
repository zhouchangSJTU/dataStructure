#include<stdio.h>
#include<fstream>
#include<string>
#include "zhashmap.h"
using namespace std;

#define FILE_1_PATH "./testdata1.txt"
#define FILE_2_PATH "./testdata2.txt"
#define BUFF_LEN 1200

class HashFunc
{
    public:
    int operator()(const string & key)
    {
        int hash = 0;
        for(int i=0;i<key.length();i++)
        {
            hash = hash << 7^key[i];
        }
        return (hash & 0x7FFFFFFF);
    }
};

class EqualKey
{
    public:
    bool operator()(const string & A, const string & B)
    {
        if (A.compare(B) == 0)
        {
            return true;
        }
        else
        {
            return false;
        }

    }
};

int main()
{
    FILE *fp = nullptr;
    char *file_1 = FILE_1_PATH;
    char *file_2 = FILE_2_PATH;
    string line = "";
    string line_2 = "";
    //printf("test");
    //char *line_2 = nullptr;
    size_t line_len = BUFF_LEN;
    int len = 0;
    HashMap<string, string, HashFunc, EqualKey> data(1000000);
    HashMap<string, string, HashFunc, EqualKey> data_both(1000000);
    //printf("test");
    string logInfo = "test";
    //printf("success");
    int count = 0;
    ifstream fp_1(FILE_1_PATH);
    ifstream fp_2(FILE_2_PATH);
    if (!fp_1.is_open())
    {
        return -1;
    }
    while((std::getline(fp_1, line)))
    {
        data.insert(line, logInfo);
        printf(data.find(line).c_str());
        printf("=====\n");
        //printf(line.c_str());
        //printf("the content of each line is:\n%s",line);
        //printf("the length of each line is: %d\n\n",len);
    }
    fp_1.close();

    if (!fp_2.is_open())
    {
        return -1;
    }
    while((std::getline(fp_2, line_2)))
    {
        printf(line_2.c_str());
        printf("ccc\n");
        printf(data.find(line_2).c_str());
        printf("xxx\n");
        if(data.find(line_2).length()!=0)
        {
            count = count + 1;
            printf("success");
            data_both.insert(line_2, data.find(line_2));
        }
    }
    printf(to_string(count).c_str());

    fp_2.close();

    //getchar();
    return 0;
}
