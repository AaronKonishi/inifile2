#include <iostream>
#include "inifile.h"

using namespace inifile;

int main()
{
	string filepath = "test/test.ini";

	inifile::IniFile ini;

	if(ini.load(filepath) == RET_ERR){
        cout<<"load error\n";
        return -1;
    }

	//获取指定段的指定项的值
	string db_name;
	int ret = false;
    ret = ini.getValue("COMMON", "DB", db_name);
    if(!ret)
        cout<<"DB="<<db_name<<endl;
    else
        cout<<"Can't find DB\n";
	//设置新值和注释
    ini.print();
    // ret = ini.setValue("TEST", "NAME", "root", "user");
    // 保存到文件
    ini.saveas("test/new.ini");
}

