//
// Created by 追影子的熊 on 2025/5/25.
//

#ifndef DBLP_NEWINIT_H
#define DBLP_NEWINIT_H

#include "all_header.h"
#include "func.h"
#include "pugixml.hpp"

class AuthorSlots
{
public:
    map<string,list<long long>> slot_author;
};
class Authors
{
public:
    vector<AuthorSlots> all_author;
    Authors():all_author(5021){}
};

class TitleSlots
{
public:
    map<string,long long> slot_title;
};
class Titles
{
public:
    vector<TitleSlots> all_title;
    Titles():all_title(5021){}
};

class YearSlots
{
public:
    map<string,long long> slot_year;//1936-2024,89
};
class Years
{
public:
    vector<YearSlots> all_year;
    Years():all_year(89){};
};

class newInit {
private:
    bool is_inited();
    void creat_dir(const string &subDir = "");
    void parseXml();
    void save_file();

    string file_path;
    string file;

    Authors authors;
    Titles titles;
    Years years;

    QTextEdit html;

public:
    newInit();
    bool initPath(const string &str);
    void ini();
    void set_inited_flag();
    string titleCache = "\0";
    int yearCache = 0;
    void getHtmlData(string &str);
    void pushTitle(unsigned int slot, const string &title, long long pos);
    void pushYear(unsigned int year, const string &title, long long pos);
    void pushAuthor(unsigned int slot, const string &author, long long pos);
    long long current_offset = 0;
};

class SAXHandler : public pugi::xml_tree_walker {
private:
    newInit *init;

public:
    explicit SAXHandler(newInit *init) : init(init) {}
    bool for_each(pugi::xml_node &node) override;
    long long pos_last = 0;
};


#endif//DBLP_NEWINIT_H
