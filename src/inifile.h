/* Copyright (c) 2014-2019 WinnerHust
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 
 * The names of its contributors may be used to endorse or promote products
 * derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef INIFILE_H_
#define INIFILE_H_

#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <string.h>

using namespace std;
namespace inifile
{
const int RET_OK  = 0;
const int RET_ERR = -1;
const char delim[] = "\n";
struct IniItem {
    string key;
    string blank1;
    string blank2;
    string value;
    string blank3;
    string comment;  // 每个键的注释，都是指该行上方的内容
    string right_comment;
};

struct IniSection {
    typedef vector<IniItem>::iterator IniItem_it;  // 定义一个迭代器，即指向键元素的指针
    IniItem_it begin() {
        return items.begin();  // 段结构体的begin元素指向items的头指针
    }

    IniItem_it end() {
        return items.end();  // 段结构体的begin元素指向items的尾指针
    }

    string name;
    string comment;  // 每个段的注释，都是指该行上方的内容
    string right_comment;
    vector<IniItem> items;  // 键值项数组，一个段可以有多个键值，所有用vector来储存
};

class IniFile
{
 public:
    IniFile();
    ~IniFile() {
        release();
    }

 public:
    /* 打开并解析一个名为fname的INI文件 */
    int load(const string &fname);
    /* 将内容保存到当前文件 */
    int save();
    /* 将内容另存到一个名为fname的文件 */
    int saveas(const string &fname);

    /* 获取section段第一个键为key的string值，成功返回0，否则返回错误码 */
    int getStringValue(const string &section, const string &key, string *value);
    /* 获取section段第一个键为key的int值，成功返回0，否则返回错误码 */
    int getIntValue(const string &section, const string &key, int *value);
    /* 获取section段第一个键为key的double值，成功返回0，否则返回错误码 */
    int getDoubleValue(const string &section, const string &key, double *value);
    /* 获取section段第一个键为key的bool值，成功返回0，否则返回错误码 */
    int getBoolValue(const string &section, const string &key, bool *value);

    /* 获取section段第一个键为key的string值，成功返回获取的值，否则返回默认值 */
    void getStringValueOrDefault(const string &section, const string &key, string *value, const string &defaultValue);
    /* 获取section段第一个键为key的int值，成功返回获取的值，否则返回默认值 */
    void getIntValueOrDefault(const string &section, const string &key, int *value, int defaultValue);
    /* 获取section段第一个键为key的double值，成功返回获取的值，否则返回默认值 */
    void getDoubleValueOrDefault(const string &section, const string &key, double *value, double defaultValue);
    /* 获取section段第一个键为key的bool值，成功返回获取的值，否则返回默认值 */
    void getBoolValueOrDefault(const string &section, const string &key, bool *value, bool defaultValue);

    /* 获取section段所有键为key的值,并将值赋到values的vector中 */
    int getValues(const string &section, const string &key, vector<string> *values);

    /* 是否存在某个section */
    bool hasSection(const string &section);

    /* 是否存在某个key */
    bool hasKey(const string &section, const string &key);

    int SetStringValue(const string &section, const string &key, const string &value);
    int SetIntValue(const string &section, const string &key, int value);
    int SetDoubleValue(const string &section, const string &key, double value);
    int SetBoolValue(const string &section, const string &key, bool value);

    /*删除段*/
    void deleteSection(const string &section);
    /*删除特定段的特定参数*/
    void deleteKey(const string &section, const string &key);
    /*设置注释标记符列表*/
    void setCommentFlags(const vector<string> &flags);
 private:
    /* 获取section段所有键为key的值,并将值赋到values的vector中,,将注释赋到comments的vector中 */
    int getValues(const string &section, const string &key, vector<string> *value, vector<string> *comments);

    /*同时设置值和注释*/
    int setValue(const string &section, const string &key, const string &value, const string &comment = "");

    /* 去掉str前面的c字符 */
    static void trimleft(string &str, char c = ' ');
    /* 去掉str后面的c字符 */
    static void trimright(string &str, char c = ' ');
    /* 去掉str前面和后面的空格符,Tab符等空白符 */
    static void trim(string &str);
    /* 将字符串str按分割符delim分割成多个子串 */
 private:
    IniSection *getSection(const string &section = "");
    void release();
    int getline(string &str, FILE *fp);
    size_t split(string &str, string &left_str, string &right_str, string &seperator);
    bool isComment(const string &str);
    bool parse(const string &content, string &key, string &blank1, string &blank2, string &value, string &blank3, char c = '=');
    // for debug
    void print();

 private:
    /* 获取section段第一个键为key的值,并将值赋到value中 */
    int getValue(const string &section, const string &key, string *value);
    /* 获取section段第一个键为key的值,并将值赋到value中,将注释赋到comment中 */
    int getValue(const string &section, const string &key, string *value, string *comment);

    int StringCmpIgnoreCase(const string &str1, const string &str2);

 private:
    typedef map<string, IniSection *>::iterator map_it;
    typedef vector<IniSection *>::iterator IniSection_it;

    map<string, IniSection *> sections_map;  // 用于存储段集合的map容器
    vector<IniSection *> sections_vt;  // 用于存储段集合的vector容器
    string fname_;
    vector<string> flags_;
};

}  // endof namespace inifile

#endif  // INIFILE_H_

