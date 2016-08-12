//
// Created by wbl on 16-7-29.
//
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace::std;
const string csvfp = "/home/wbl/Desktop/1.csv";

vector<string> split(string rawString, char spliter)
{
    vector<string> result;
    string tmp="";
    for(int i=0;i<rawString.length();i++)
    {
        if(rawString[i]==spliter){
            result.push_back(tmp);
            tmp="";
        }
        else
            tmp += rawString[i];
    }
    result.push_back(tmp);
    return result;

}


string join(vector<string> vec, char joiner)
{
    string result="";
    for(auto str:vec)
        result+=str+joiner;
    return result.substr(0,result.length()-1);
}


vector<vector<string>> readCsv(string filePath)
// read a csv file,
// return vector<vector<string>> contains strings if the file if success;
// return  vector<vector\+0<string>> contains nothing if failed.
{
    std::vector<vector<string>>  context;
    string line, field;
    ifstream in(filePath);
    if(in.is_open())
    {
        while(getline(in, line))
        {
            std::vector<string> rowContext=split(line, ',');
            context.push_back(rowContext);
        }
        in.close();
    }
    else
        cout<<"open csv error."<<endl;
    return context;
}


bool writeCsv(string filePath, vector<vector<string>> content)
// save a vector<vector<string>> variable to the filePath(param);
// return success?true:false;

{
    vector<vector<string>> old_content = readCsv(filePath);
    if(content==old_content) return true;
    ofstream out(filePath);
    if(out.is_open())
    {
        int i;
        for(i = 0; i < content.size(); i++) {
            out << join(content[i], ',')<<'\n';
            out.flush();
        }
        out.close();
        return true;
    }
    cerr<<"failed to write to "<<filePath<<endl;
    return false;
}


string getCsvValue(string filePath, int row, int col)
// if row and col are valid index
// return the string of the coordinate in the csv.

{
    vector<vector<string>> context = readCsv(filePath);
    if(row<context.size()&&col<context[row].size()) {
        string result = context[row][col];
        return result;
    }
    else{
        cout<<"get value from "<<filePath<<" failed.";
        return "";
    }
}


bool setCsvValue(string filePath, int row, int col, string setValue)
// if row and col are valid index,
// set the string at the coordinate to the setValue.

{
    vector<vector<string>> context = readCsv(filePath);
    if(row<context.size()&&col<context[row].size()) {
        context[row][col] = setValue;
        return writeCsv(filePath, context);
    }
    else {
        cout<<"failed to set value of "<<filePath<<"."<<endl;
        cout<<"out of index"<<endl;
        return false;
    }
}


bool appendInfoToCsv(string filePath, string info)
//add a row(means a record) to the end of the file.

{
    vector<vector<string>> old_content = readCsv(filePath);
    vector<string> a_server;
    a_server = split(info, ',');
    old_content.push_back(a_server);
    return writeCsv(filePath, old_content);
}


bool deleteInfoFromCsv(string filePath, string keyword)
//delete one row of the file by the kye word "keyword."

{
    vector<vector<string>> old_content, content;
    //old-content means the content of the file,
    // delete one record and save the others in content.

    old_content = readCsv(filePath);
    for(auto serv:old_content)
        if(serv[0] != keyword) content.push_back(serv);
    return writeCsv(filePath, content);
}

//bool initCsv(string filePath)
//{
//    ofstream out(filePath);
//    if(out.is_open())
//    {
//        out.close();
//        return true;
//    }
//    else
//        return false;
//}


/*int main()
{
    bool status = deleteInfoFromCsv(csvfp, "a1");
    if(status)
    {
        vector<vector<string>> context = readCsv(csvfp);
        for(auto row:context)
            for(auto field:row)
                cout<<field<<endl;
    }
    return 0;

}
*/