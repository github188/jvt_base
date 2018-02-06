#include <unistd.h>
#include <stdio.h>

#include <stdlib.h>
#include "json.h"
#include <iostream>
#include <fstream>
#include <assert.h>

using namespace std;


void demo_read_jsonFile()
{
    ifstream ifs;
    ifs.open("ex.json");
    assert(ifs.is_open());
 
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(ifs, root, false))
    {
        cerr << "parse failed \n";
        return;
    }

	Json::Value arrayObj = root["plug-ins"]; 
	for (int i=0; i<arrayObj.size(); i++) 
	{ 
	
		std::cout << arrayObj[i].asString() << std::endl;
	} 
	
	std::cout<< "demo_read_jsonFile" <<endl;
 	std::cout<<root["encoding"].asString()<<endl;
   	std::cout<<root["indent"]["length"].asInt()<<endl;
}

void demo_read_jsonMem()
{
    Json::Reader *pJsonParser = new Json::Reader(Json::Features::strictMode());
	string strJson = "{\"name\":\"tom\"}";
	Json::Value tempVal;
    if(!pJsonParser->parse(strJson, tempVal))
   {
       cout << "parse error" << endl;
   }
	
   	string name = tempVal["name"].asString();
	
	cout << "name = " << name << endl;
	delete pJsonParser;
}

void demo_write_object()
{
    Json::Value root;
    Json::FastWriter writer;
 
    root["name"] = "尼玛";
    root["salary"] = 100;
    root["msg"] = "work hard";
    Json::Value files;
    files[0] = "1.ts";
    files[1] = "2.txt";
    root["files"] = files;
 
    string json_file = writer.write(root);
    cout << "demo write json object ==============\n";
    cout << json_file << endl;
}


void demo_write_simple()
{
    Json::Value root;
    Json::FastWriter writer;
    Json::Value person;
 
    person["name"] = "tocy";
    person["age"] = 1000;
    root.append(person);
 
    string json_file = writer.write(root);

    cout << "demo write json ==============\n";
    cout << json_file << endl;
}

void demo_write_array()
{
    Json::Value root;
    Json::FastWriter writer;

    {
        Json::Value person; 
        person["name"] = "尼玛";
        person["salary"] = 100;
        root[0] = person;
    }
    
    {
        Json::Value person; 
        person["name"] = "Kit";
        person["salary"] = 89;
        root[1] = person;
    }
    
    root[2] = "a json note";
 
    string json_file = writer.write(root);
    cout << "demo write json ==============\n";
    cout << json_file << endl;
}


int main(int argc,char *argv[])
{
	//demo_read_jsonFile();
	//demo_write_simple();
	//demo_write_object();
	demo_write_array();
	getchar();
	return 0;
}
