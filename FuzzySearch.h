//
// Created by 追影子的熊 on 2024/4/26.
//

#ifndef DBLP_FUZZYSEARCH_H
#define DBLP_FUZZYSEARCH_H


#include "all_header.h"
#include "func.h"

class FuzzySearch
{
public:
    static vector<string> data;

private:
    mutex mtx;
    queue<string> filenames_queue;
    string file_path ;
    string file ;
    unsigned int num_threads;


    bool isInited = false;

    struct SearchTask {
        decltype(data)::const_iterator begin_, end_; // 使用迭代器表示数据子范围
        vector<string> keywords;
        unordered_set<int> local_matches;

        SearchTask(decltype(data)::const_iterator b, decltype(data)::const_iterator e,
                   const vector<string>& kws)
            : begin_(b), end_(e), keywords(kws) {}

        void operator()()
        {
            for (auto it = begin_; it != end_; ++it)
            {
                bool find = true;
                for (const auto& keyword : keywords)
                {
                    if (it->find(keyword) == std::string::npos)
                    {
                        find = false;
                        break;
                    }
                }
                if(find)
                {
                    local_matches.insert(std::distance(data.cbegin(), it));
                }
            }
        }
    };

public:

    unordered_set<int> matched_titles;

    FuzzySearch();
    void parse_line(const string &line);
    void process_file();
    void distribute_and_start_threads(unsigned int num_threads);
    void parallel_fuzzy_search(const vector<string>& keywords);
    void initPath(const string & str);
};

#endif//DBLP_FUZZYSEARCH_H
