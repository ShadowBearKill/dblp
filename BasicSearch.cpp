//
// Created by 追影子的熊 on 2024/4/23.
//

#include "BasicSearch.h"

void BasicSearch::initPath(const string & str)
{
    file = str;
    file_path = getLastPrefix(str) + "/Index";
}

void BasicSearch::getPosFormTitle(const string& title)
{
    this->articlePos.clear();
    unsigned int slot = func::hash(title);

    string indexFile = file_path+"/title/title" + to_string(slot) + ".txt";
    fstream titleIndex(indexFile,ios::in);
    string infoSource;
    getline(titleIndex,infoSource);
    pair<string,string> info;
    while(!titleIndex.eof())
    {
        info = func::splitTitleInfo(infoSource);
        if(func::isNumeric(info.second)&&info.first == title)
        {
            articlePos.emplace_back(std::stoll(info.second));
            titleIndex.close();
            return;
        }
        getline(titleIndex,infoSource);
    }
    titleIndex.close();
}
void BasicSearch::getPosFormAuthor(const string &author)
{
    this->articlePos.clear();
    unsigned int slot = func::hash(author);

    string indexFile = file_path+"/author/author" + to_string(slot) + ".txt";
    fstream authorIndex(indexFile,ios::in);
    string infoSource;
    getline(authorIndex,infoSource);
    vector<string> info;
    while(!authorIndex.eof())
    {
        info = func::splitAuthorInfo(infoSource);
        if(info[0] == author)
        {
            for (int i = 1; i < info.size(); i++)
            {
                if (func::isNumeric(info[i]))
                {
                    articlePos.emplace_back(std::stoll(info[i]));
                }
            }
            authorIndex.close();
            return;
        }
        getline(authorIndex,infoSource);
    }
    authorIndex.close();
}
void BasicSearch::titleGetFromMemory(const string &_title, list<TitleList>::iterator it) {
    // 使用lower_bound找到第一个等于或大于目标值的元素
    auto it_2 = std::lower_bound(it->titlesName.begin(), it->titlesName.end(), _title);
    // 检查找到的元素是否就是目标值
    if (it_2 != it->titlesName.end() && *it_2 == _title)
    {
        unsigned int index = std::distance(it->titlesName.begin(), it_2); // 获取索引
        std::cout << "Element found at index: " << index << '\n';
        articlePos.push_back(it->titlesArticle[index]);
    }
    else
    {
        std::cout << "Element not found.\n";
    }
}
void BasicSearch::titleAddMemory(const unsigned int &slot, const string &_title,list<TitleList>::iterator it)
{
    auto new_it = this->titleList.insert(it,TitleList(slot));
    string indexFile = file_path+"/title/title" + to_string(slot) + ".txt";
    fstream titleIndex(indexFile,ios::in);
    string infoSource;
    getline(titleIndex,infoSource);
    pair<string,string> info;
    while(!titleIndex.eof())
    {
        info = func::splitTitleInfo(infoSource);
        if(func::isNumeric(info.second))
        {
            new_it->titlesName.emplace_back(info.first);
            new_it->titlesArticle.emplace_back(std::stoll(info.second));
        }
        getline(titleIndex,infoSource);
    }

    titleIndex.close();
    titleGetFromMemory(_title,new_it);
}
void BasicSearch::authorGetFromMemory(const string & _author, list<AuthorList>::iterator it)
{
    // 使用lower_bound找到第一个等于或大于目标值的元素
    auto it_2 = std::lower_bound(it->authorsName.begin(), it->authorsName.end(), _author);
    // 检查找到的元素是否就是目标值
    if (it_2 != it->authorsName.end() && *(it_2) == _author)
    {
        unsigned int index = std::distance(it->authorsName.begin(), it_2); // 获取索引
        std::cout << "Element found at index: " << index << '\n';
        articlePos=it->authorsArticle[index];
    }
    else
    {
        std::cout << "Element not found.\n";
    }
}
void BasicSearch::authorAddMemory(const unsigned int &slot, const string &_author, list<AuthorList>::iterator it)
{
    auto new_it = this->authorList.insert(it,AuthorList(slot));
    string indexFile = file_path+"/author/author" + to_string(slot) + ".txt";
    fstream authorIndex(indexFile,ios::in);
    string infoSource;
    getline(authorIndex,infoSource);
    vector<string> info;
    while(!authorIndex.eof())
    {
        info = func::splitAuthorInfo(infoSource);
        new_it->authorsName.emplace_back(info[0]);
        for(int i = 1; i<info.size();i++)
        {
            if (func::isNumeric(info[i]))
            {
                new_it->authorsArticle.emplace_back(stoi(info[i]));
            }
        }
        getline(authorIndex,infoSource);
    }

    authorIndex.close();

    authorGetFromMemory(_author,new_it);
}