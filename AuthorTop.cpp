//
// Created by 追影子的熊 on 2024/4/25.
//

#include "AuthorTop.h"
void AuthorTop::processAuthorCountData()
{
    cout<<"start process"<<endl;
    string authorIndex;
    fstream indexFile;
    string data;
    for(int i = 0;i<5021;i++)
    {
        authorIndex = file_path + "/author/author" + to_string(i) + ".txt";
        indexFile.open(authorIndex,ios::in);
        if(indexFile.is_open())
        {
            getline(indexFile,data);
            while(!indexFile.eof())
            {
                AuthorCount temp = func::getArticleCount(data);
                if (pq.size() < 100) {
                    pq.push(temp);
                } else if (temp.articleCount > pq.top().articleCount) {
                    pq.pop();
                    pq.push(temp);
                }
                getline(indexFile,data);
            }
            indexFile.close();
        }
        else
        {
            cout<<"Can't open anthor index file author" + to_string(i)<<endl;
        }
    }
    cout<<"start saving"<<endl;
    saveAuthorCountData();
    cout<<"done"<<endl;
}

void AuthorTop::initPath(const string &str)
{
    file = str;
    file_path = getLastPrefix(str) + "/Index";
}

void AuthorTop::saveAuthorCountData()
{
    vector<AuthorCount> v;
    string file_name=file_path+"/TopAuthor.txt";
    fstream save(file_name,ios::out);
    if(save.is_open())
    {
        while (!pq.empty()) {
            v.push_back(pq.top());
            pq.pop();
        }
        std::sort(v.begin(), v.end(), [](const AuthorCount &a, const AuthorCount &b) {
            return a.articleCount > b.articleCount;
        });
        for (auto &i: v) {
            save << i.name << "$$" << i.articleCount << endl;
        }
        save.close();
    }
    else
    {
        cout<<"can't open file topAuthor"<<endl;
    }
}
void AuthorTop::getTopAuthor()
{
    this->authors.clear();
    string file_name=file_path+"/TopAuthor.txt";
    fstream authorTopList(file_name,ios::in);
    string frequent;
    getline(authorTopList,frequent);
    vector<string> info;
    while(!authorTopList.eof())
    {
        authors .push_back(func::splitAuthorCount(frequent));
        getline(authorTopList,frequent);
    }
    authorTopList.close();
}
