//
// Created by 追影子的熊 on 2024/4/25.
//

#ifndef DBLP_AUTHORTOP_H
#define DBLP_AUTHORTOP_H

#include "all_header.h"
#include "func.h"


class AuthorTop
{
    std::priority_queue<AuthorCount, vector<AuthorCount>, CompareByArticleCount > pq;
    //模块化测试数据，整合时应删除
    string file_path;
    string file;


    void saveAuthorCountData();

public:
    void initPath(const string & str);
    void processAuthorCountData();
    void getTopAuthor();
    vector< AuthorCount> authors;

};


#endif//DBLP_AUTHORTOP_H
