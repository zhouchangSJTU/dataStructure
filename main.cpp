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

    size_t line_len = BUFF_LEN;
    int len = 0;

    HashMap<string, string, HashFunc, EqualKey> *data = new HashMap<string, string, HashFunc, EqualKey>(1000000);
    HashMap<string, string, HashFunc, EqualKey> *data_both = new HashMap<string, string, HashFunc, EqualKey>(1000000);

    string logInfo = "test";
    int count = 0;
    int dcount = 0;
    ifstream fp_1(FILE_1_PATH);
    ifstream fp_2(FILE_2_PATH);
    if (!fp_1.is_open())
    {
        return -1;
    }
    while((std::getline(fp_1, line)))
    {
        if(dcount<1000000)
        {
            data->insert(line, logInfo);
            dcount = dcount + 1;
        }
        else
        {
            if (!fp_2.is_open())
            {
                return -1;
            }
            while((std::getline(fp_2, line_2)))
            {
                if(data->find(line_2).length()!=0)
                {
                    count = count + 1;
                    data_both->insert(line_2, data->find(line_2));
                }
            }
            fp_2.close();
            delete data;
            HashMap<string, string, HashFunc, EqualKey> *data = new HashMap<string, string, HashFunc, EqualKey>(1000000);
            dcount = 0;
        }
    }

    if (!fp_2.is_open())
    {
        return -1;
    }
    while((std::getline(fp_2, line_2)))
    {
        printf(data->find(line_2).c_str());
        if(data->find(line_2).length()!=0)
        {
            count = count + 1;
            data_both->insert(line_2, data->find(line_2));
        }
    }
    fp_2.close();
    fp_1.close();
    printf(to_string(count).c_str());
    return 0;
}
