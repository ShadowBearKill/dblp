//
// Created by 追影子的熊 on 2024/4/21.
//

#ifndef DBLP_FUNC_H
#define DBLP_FUNC_H


#include "all_header.h"

namespace func
{
    string getLastPrefix(const string &inputStr);

    unsigned hash(const string & str);
    bool findPublicRecord(string& s);

    int findData(string& s);
    string getAuthorData(string s);
    string getTitleData(string s);
    string getYearData(string s);
    bool findEnd(const string & str);
    bool isNumeric(const std::string& str);
    pair<string,string> splitTitleInfo(const string & str);
    bool strIsDigit(const string & str);
    vector<string> splitAuthorInfo(string & str);
    AuthorCount getArticleCount(const string & str);
    int countSpaces(const std::string& str);
    string preprocessWord(const std::string& word);
    std::vector<std::string> extractKeywords(const std::string& title);
    vector<int> getEdges(const string& str);
    bool check(const std::vector<int>& vec1, const std::vector<int>& vec2,int&d1,int &d2);
    int binarySearch(const std::vector<int>& vec, int val);
    vector<vector<string>> getArticleInformation(long long pos,const string & file);
    bool findFrequentWords(string s);
    AuthorCount splitAuthorCount(const string & str);
    KeywordCount splitKeywordsCount(const string & str);

    pair<int,int> getClusterResult(const string & str);

}

using func::hash;
using func::getLastPrefix;


#endif//DBLP_FUNC_H
