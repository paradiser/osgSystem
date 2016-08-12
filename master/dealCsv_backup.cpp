//
// Created by wbl on 16-7-29.
//
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "master.h"

using namespace::std;
//const string csvfp = "/home/wsj/files/osgFiles/1.csv";
//#define csvfp "../1.csv"

vector<vector<string>> readCsv(string filePath)
{
    std::vector<vector<string>>  context;
    string line, field;
    ifstream in(filePath);
    if(in.is_open())
    {
        while(getline(in, line))
        {
            istringstream stream(line);
            std::vector<string> rowContext;
            while(getline(stream, field, ','))
            {
                rowContext.push_back(field);
            }
            context.push_back(rowContext);
        }
    }
    else
        cout<<"open csv error."<<endl;
    return context;
}

string getCsvValue(string filePath, int row, int col)
{
    vector<vector<string>> context = readCsv(filePath);
	string result = context[row][col];
	return result;
}

int setCsvValue(string filePath, int row, int col, string setValue)
{
    vector<vector<string>> context = readCsv(filePath);
    context[row][col] = setValue;
    ofstream out(filePath);
    if(out.is_open()) {
        for (vector<string> line:context) {
            string aline="";
            for (string str:line)
                aline += str + ",";
            aline += "\n";
            out << aline;
        }
        out.close();
        return 0;
    }
    else
    {
        cout<<"save failed."<<endl;;
        return -1;
    }
}

/*int main()
{
    string temp = getCsvValue(csvfp, 1, 1);
    cout<<temp<<endl;
    bool haveatry = setCsvValue(csvfp, 1,1,"ccc");
    temp = getCsvValue(csvfp, 1, 1);
    cout<<temp<<endl;
    return 0;

}*/
