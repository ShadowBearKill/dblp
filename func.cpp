//
// Created by 追影子的熊 on 2024/4/21.
//

#include "func.h"

namespace func
{
    //各种文章的开始标签
    vector<string> publicRecord = {"article","book","book ","incollection"
                                   ,"inproceedings","proceedings","mastersthesis","phdthesis","www"};
    //各种文章的结束标签
    vector<string> end = {"</article>","</book>","</incollection>","</inproceedings>",
                          "</proceedings>","</mastersthesis>","</phdthesis>","</www>"};
    //文章内的各种重要标签
    vector<string> data = {"</title>","</author>","</year>"};

    vector<string> frequentWord = {"of","and","a","the","to","for","on","in","at","an","toward","some","as",
     "with","by",""};

    //获取斜杠前的前面字符串
    string getLastPrefix(const std::string &inputStr)
    {
        int last_pos = inputStr.find_last_of('/');
        if (last_pos != std::string::npos) {
            // 返回最后一个斜杠之前的部分
            return inputStr.substr(0, last_pos);
        } else {
            // 如果没有找到斜杠，则返回整个字符串
            return inputStr;
        }
    }

    //哈希函数，5021是槽的数量
    unsigned hash(const string &str)
    {
        unsigned int hash = 5381;
        for (char c : str) {
            hash = ((hash << 5) + hash) + (c++);
        }
        return hash % 5021;
    }

    //从字符串中查找是否找到文章标签
    bool findPublicRecord(std::string& s)
    {
        return std::ranges::any_of(publicRecord, [&s](const std::string& record) {
            return record == s;
        });
    }

    bool findFrequentWords(string s)
    {
        for(const auto & i : frequentWord)
        {
            if(s==i)
            {
                return true;
            }
        }
        return false;
    }

    //查找字符串中是否包含重要信息标签
    int findData(string& s)
    {
        for(int i = 0;i<3;i++)
        {
            if(s.find(data[i])!=string::npos)
            {
                return i;
            }
        }
        return -1;
    }

    //获取作者信息
    string getAuthorData(string s)
    {
        size_t first = s.find("author");
        s = s.substr(first);
        first = s.find('>');
        size_t last = s.find("</author>"); // +7是为了跳过"<title>"的长度
        return s.substr(first + 1, last - first - 1);
    }

    //获取标题信息
    string getTitleData(string s)
    {
        size_t first = s.find("title");
        s = s.substr(first);
        first = s.find('>');
        size_t last = s.find("</title>"); // +7是为了跳过"<title>"的长度
        return s.substr(first + 1, last - first - 1);
    }

    //获取年份信息
    string getYearData(string s)
    {
        size_t first = s.find("year");
        s = s.substr(first);
        first = s.find('>');
        size_t last = s.find("</year>"); // +7是为了跳过"<title>"的长度
        return s.substr(first + 1, last - first - 1);
    }

    //字符串中是否包含文章结尾标签
    bool findEnd(const string & str)
    {
        for(int i = 0;i<8;i++)
        {
            if(str.find(end[i])!=string::npos)
            {
                return true;
            }
        }
        return false;
    }

    //确定一个字符串是否全由数字构成
    bool isNumeric(const std::string& str) {
        if(str.empty())
            return false;
        for (char const &c : str) {
            if (!std::isdigit(c)) return false; // 如果有一个字符不是数字，返回false
        }
        return true; // 所有字符都是数字，返回true
    }

    //从字符串中分离出标题和偏移量，使用pair保存
    pair<string,string> splitTitleInfo(const string & str)
    {
        // 查找标记的位置
        size_t signPos = str.find("$$");

        // 如果找到标记
        if (signPos != std::string::npos) {
            // 分割前后两部分
            string partBefore = str.substr(0, signPos);
            string partAfter = str.substr(signPos + 2); // "+2"是因为要跳过分隔符"$$"的两个字符

            return std::make_pair(partBefore, partAfter);
        } else {
            // 如果没有找到标记，则返回空的第二部分
            return std::make_pair(str, "");
        }
    }
    //从字符串中分离出作者和偏移量列表，使用vector保存，其中第一个为作者名字，其余为偏移量列表
    vector<string> splitAuthorInfo(string & str)
    {
        vector<string> v;
        v.reserve(6);

        // 查找标记的位置
        size_t signPos;
        signPos = str.find("$$");
        // 如果找到标记
        if (signPos!= std::string::npos) {
            // 分割前后两部分
            v.push_back(str.substr(0, signPos));
            str = str.substr(signPos + 2); // "+2"是因为要跳过分隔符"$$"的两个字符
            signPos = str.find(' ');
            while(signPos!=string::npos)
            {
                v.push_back(str.substr(0, signPos));
                str = str.substr(signPos + 1); // "+1"是因为要跳过分隔符" "的1个字符
                signPos = str.find(' ');
            }
            return v;
        } else {
            // 如果没有找到标记，则返回空的第二部分
            return {};
        }
    }

    int countSpaces(const std::string& str) {
        int spaceCount = 0;
        for (char c : str)
        {
            if (c == ' ') {
                spaceCount++;
            }
        }
        return spaceCount;
    }

    AuthorCount getArticleCount(const string & str)
    {
        // 查找标记的位置
        size_t signPos = str.find("$$");

        // 如果找到标记
        if (signPos != std::string::npos) {
            // 分割前后两部分
            string partBefore = str.substr(0, signPos);
            int count = countSpaces(str.substr(signPos + 2)); // "+2"是因为要跳过分隔符"$$"的两个字符

            return {partBefore,count};
        } else {
            // 如果没有找到标记，则返回空的第二部分
            return {str,0};
        }
    }

    string preprocessWord(const std::string& word) {
        string result;
        for (char c : word) {
            if (std::isalnum(c)) {
                result += std::tolower(c);
            }
        }
        return result;
    }

    std::vector<std::string> extractKeywords(const std::string& title) {
        std::vector<std::string> keywords;

        std::stringstream ss(title);
        std::string word;

        while (std::getline(ss, word, ' ')) { // 先按照空格分割
            size_t hyphenPos = word.find('-');
            while (hyphenPos != std::string::npos) {
                if (hyphenPos > 0) {
                    keywords.push_back(preprocessWord(word.substr(0, hyphenPos))); // 添加前面部分
                }
                word.erase(0, hyphenPos + 1); // 移除已处理的部分包括连字符
                hyphenPos = word.find('-'); // 继续查找下一个连字符
            }
            if (!word.empty()) {
                keywords.push_back(preprocessWord(word)); // 添加最后一个部分或不含连字符的单词
            }
        }
        return keywords;
    }

    vector<int> getEdges(const string& str)
    {
        std::istringstream iss(str);
        vector<int> result;

        int num;
        while (iss >> num)
        {
            result.push_back(num);
        }
        return result;
    }

    //此函数用来确认vec1和vec2如果只有一位不同，则返回true，并且把这两个数组中不相同的那一位设为d1和d2
    bool check(const std::vector<int>& vec1, const std::vector<int>& vec2,int& d1,int &d2)
    {
        if (vec1.size() != vec2.size()) {
            // 如果大小不相等，则直接返回false
            return false;
        }

        std::vector<int> difference1;
        std::vector<int> difference2;
        std::set_difference(vec1.begin(), vec1.end(),
                            vec2.begin(), vec2.end(),
                            std::back_inserter(difference1));

        std::set_difference(vec2.begin(), vec2.end(),
                            vec1.begin(), vec1.end(),
                            std::back_inserter(difference2));

        if(difference1.size()==1&& difference2.size()==1)
        {
            d1 = difference1[0];
            d2 = difference2[0];
            return true;
        }
        return false;
    }

    //二分查找，不需要解释吧
    int binarySearch(const std::vector<int>& vec, int val)
    {
        int left = 0;
        int right = vec.size() - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (vec[mid] == val) {
                return mid;  // 找到值，返回下标
            } else if (vec[mid] < val) {
                left = mid + 1;  // 在右半部分继续查找
            } else {
                right = mid - 1;  // 在左半部分继续查找
            }
        }
        // 如果未在vector中找到val，则返回-1（或任何表示未找到的值）
        return -1;
    }

    vector<vector<string>> getArticleInformation(long long pos,const string & file)
    {
        fstream xml(file,ios::binary|ios::in);
        xml.seekg(pos);
        string source;
        getline(xml,source);
        vector<vector<string>> v;
        string tag,content;
        bool first = true;
        while(!(func::findEnd(source))||first)
        {
            if(first)
            {
                if(std::count(source.begin(), source.end(), '<')>1)
                {
                    size_t endTagNamePos = source.find('>');
                    source = source.substr(endTagNamePos+1);
                }

                size_t firstPos = source.find('<');
                size_t endPos1 = source.find(' ');
                v.push_back({source.substr(firstPos+1,endPos1-firstPos-1)});
                source = source.substr(endPos1 + 1);

                size_t middlePos;
                endPos1 = source.find(' ');
                while(endPos1 !=string::npos)
                {
                    middlePos = source.find('=');
                    v.push_back({source.substr(0,middlePos),
                    source.substr(middlePos+1,endPos1-middlePos-1)});
                    source = source.substr(endPos1 + 1);
                    endPos1 = source.find(' ');
                }
                size_t endPos2 = source.find('>');
                middlePos = source.find('=');
                v.push_back({source.substr(0,middlePos),
                             source.substr(middlePos+1,endPos2-middlePos-1)});
                first = false;
            }
            else
            {
                while(!source.empty())
                {
                    bool isSame =false;
                    size_t startPos = source.find('>');
                    size_t endPos = source.find("</");
                    content = source.substr(startPos + 1, endPos - startPos-1);
                    source = source.substr(endPos);
                    endPos = source.find("</");
                    size_t endPos2 = source.find('>');
                    tag = source.substr(endPos + 2, (endPos2 - endPos-2));
                    source = source.substr(endPos2+1);
                    for(auto & i : v)
                    {
                        if(i[0] == tag)
                        {
                            i.push_back(content);
                            isSame = true;
                            break;
                        }
                    }
                    if(!isSame)
                        v.push_back({tag,content});
                }

            }
            getline(xml,source);
        }
        xml.close();
        return v;
    }

    AuthorCount splitAuthorCount(const string & str)
    {
        // 查找标记的位置
        size_t signPos = str.find("$$");

        // 如果找到标记
        if (signPos != std::string::npos) {
            // 分割前后两部分
            string partBefore = str.substr(0, signPos);
            string partAfter = str.substr(signPos + 2); // "+2"是因为要跳过分隔符"$$"的两个字符

            return {partBefore,stoi(partAfter)};
        } else {
            // 如果没有找到标记，则返回空的第二部分
            return {str, 0};
        }
    }
    KeywordCount splitKeywordsCount(const string & str)
    {
        // 查找标记的位置
        size_t signPos = str.find("$$");

        // 如果找到标记
        if (signPos != std::string::npos) {
            // 分割前后两部分
            string partBefore = str.substr(0, signPos);
            string partAfter = str.substr(signPos + 2); // "+2"是因为要跳过分隔符"$$"的两个字符

            return {partBefore,stoi(partAfter)};
        } else {
            // 如果没有找到标记，则返回空的第二部分
            return {str, 0};
        }
    }

    pair<int,int> getClusterResult(const string & str)
    {
        // 查找标记的位置
        size_t signPos = str.find(" ");

        // 如果找到标记
        if (signPos != std::string::npos) {
            // 分割前后两部分
            int partBefore = stoi(str.substr(0, signPos));
            string t  = str.substr(signPos + 1);
            int partAfter = stoi(t); // "+2"是因为要跳过分隔符"$$"的两个字符

            return {partBefore,partAfter};
        } else {
            // 如果没有找到标记，则返回空的第二部分
            return {0, 0};
        }
    }
}