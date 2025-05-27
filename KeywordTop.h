//
// Created by 追影子的熊 on 2024/4/25.
//

#ifndef DBLP_KEYWORDTOP_H
#define DBLP_KEYWORDTOP_H

#include "all_header.h"
#include "func.h"

class KeywordTop
{
private:
    vector<unordered_map<string, int>> freqMap;
    vector<vector<string>> titles;
    string file_path ;
    string file;
    int firstYear = 1936;

public:
    KeywordTop();
    void initPath(const string & str);
    void countAndSortKeywords(int topK);
    void getTitle();
    void getKeyWordTop(int year);
    vector<KeywordCount> keyWords;
};


#endif//DBLP_KEYWORDTOP_H
