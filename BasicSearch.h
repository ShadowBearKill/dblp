//
// Created by 追影子的熊 on 2024/4/23.
//

#ifndef DBLP_BASICSEARCH_H
#define DBLP_BASICSEARCH_H

#include "all_header.h"
#include "func.h"

class AuthorList
{
public:
    unsigned int slotID;
    vector<string> authorsName;
    vector<vector<long long>> authorsArticle;
    explicit AuthorList(const unsigned int& ID){slotID = ID;authorsName.reserve(750);authorsArticle.reserve(750);}
};

class TitleList
{
public:
    unsigned int slotID;
    vector<string> titlesName;
    vector<long long> titlesArticle;
    explicit TitleList(const unsigned int& ID){slotID = ID;titlesName.reserve(1400);titlesArticle.reserve(1400);}
};


class BasicSearch
{
private:

    string file_path;
    string file;
    list<AuthorList> authorList;
    list<TitleList> titleList;

    void titleGetFromMemory(const string &_title, list<TitleList>::iterator it);
    void titleAddMemory(const unsigned int & slot,const string& _title,list<TitleList>::iterator it);
    void authorGetFromMemory(const string &_author, list<AuthorList>::iterator it);
    void authorAddMemory(const unsigned int & slot,const string& _author,list<AuthorList>::iterator it);
public:
    vector<long long> articlePos;
    BasicSearch() = default;
    void initPath(const string & str);
    void getPosFormTitle(const string& title);
    void getPosFormAuthor(const string& author);
};


#endif//DBLP_BASICSEARCH_H
