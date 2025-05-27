//
// Created by 追影子的熊 on 2024/4/21.
//

#ifndef DBLP_ALL_HEADER_H
#define DBLP_ALL_HEADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <utility>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <sstream>
#include <thread>
#include <mutex>
#include <unordered_set>
#include <memory>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextEdit>


using std::string;
using std::fstream;
using std::cout;
using std::endl;
using std::cerr;
using std::ios;
using std::vector;
using std::list;
using std::map;
using std::to_string;
using std::set;
using std::binary_search;
using std::find_if;
using std::lower_bound;
using std::pair;
using std::priority_queue;
using std::stringstream;
using std::unordered_map;
using std::mutex;
using std::queue;
using std::thread;
using std::unordered_set;
using std::shared_ptr;




struct AuthorCount
{
    string name;
    int articleCount;
    AuthorCount(string _name, int _count):name(std::move(_name)),articleCount(_count){}
};

struct CompareByArticleCount {
    bool operator()(const AuthorCount& lhs, const AuthorCount& rhs) const {
        return lhs.articleCount > rhs.articleCount; // 注意这里是 ">"
    }
};



struct KeywordCount {
    string keyword;
    int count;
    bool operator<(const KeywordCount& other) const {
        return count > other.count; // 注意这里是大于号，因为优先队列默认是最小堆，我们需要反转比较规则
    }

};

struct CompareByKeywordCount {
    bool operator()(const KeywordCount& lhs, const KeywordCount& rhs) const {
        return lhs.count > rhs.count; // 注意这里是 ">"
    }
};

#endif//DBLP_ALL_HEADER_H
