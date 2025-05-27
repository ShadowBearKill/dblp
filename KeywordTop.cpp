//
// Created by 追影子的熊 on 2024/4/25.
//

#include "KeywordTop.h"

KeywordTop::KeywordTop()
{
    titles.resize(89);
    freqMap.resize(89);
}
void KeywordTop::getTitle()
{
    cout<<"start getTitle"<<endl;
    string yearIndex;
    fstream indexFile;
    string data;
    for(int i = 0;i<89;i++)
    {
        yearIndex = file_path + "/year/" + to_string(i+1936) + ".txt";
        indexFile.open(yearIndex,ios::in);
        if(indexFile.is_open())
        {
            getline(indexFile,data);
            while(!indexFile.eof())
            {
                string title = func::splitTitleInfo(data).first;
                titles[i].push_back(title);
                getline(indexFile,data);
            }
            indexFile.close();
        }
        else
        {
            cout<<"Can't open year index file author" + to_string(i+1936)<<endl;
        }
    }
}
void KeywordTop::countAndSortKeywords(int top)
{
    int topK = top + 20;
    cout<<"start countAndSortKeywords"<<endl;
    string file_name;
    fstream  f;
    for(int i = 0;i<89;i++)
    {
        for (const auto& title : titles[i]) {
            auto keywords = func::extractKeywords(title);
            for (const auto& keyword : keywords) {
                freqMap[i][keyword]++;
            }
        }

        std::priority_queue<KeywordCount, vector<KeywordCount>, CompareByKeywordCount > topKFrequencies;
        for (const auto& pair : freqMap[i]) {
            if (topKFrequencies.size() < topK || topKFrequencies.top().count < pair.second) {
                topKFrequencies.push({pair.first, pair.second});
                if (topKFrequencies.size() > topK) {
                    topKFrequencies.pop();
                }
            }
        }

        file_name = file_path + "/year/" + to_string(i+1936) + "top.txt";
        f.open(file_name,ios::out);
        if(f.is_open())
        {
            int count = 0;
            while (!topKFrequencies.empty()&&count<top)
            {
                const KeywordCount& kf = topKFrequencies.top();
                if(!func::findFrequentWords(kf.keyword))
                {
                    f<<kf.keyword<<"$$"<<kf.count<<endl;
                    ++count;
                }
                topKFrequencies.pop();
            }
        }
        f.close();
    }
    titles.clear();
    cout<<"done"<<endl;
}

void KeywordTop::initPath(const string &str)
{
    file = str;
    file_path = getLastPrefix(str) + "/Index";
}
void KeywordTop::getKeyWordTop(int year)
{
    string file_name=file_path+"/year/" + to_string(firstYear+year)+"top.txt";
    fstream keyWordsTopList(file_name,ios::in);
    string frequent;
    getline(keyWordsTopList,frequent);
    vector<string> info;
    while(!keyWordsTopList.eof())
    {
        keyWords .push_back(func::splitKeywordsCount(frequent));
        getline(keyWordsTopList,frequent);
    }
    keyWordsTopList.close();
}
