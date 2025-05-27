//
// Created by 追影子的熊 on 2024/4/26.
//

#include "FuzzySearch.h"

vector<std::string> FuzzySearch::data; // 这里进行了默认初始化

void FuzzySearch::parse_line(const string &line)
{
    string title;
    title = func::splitTitleInfo(line).first;

    std::lock_guard<std::mutex> lock(mtx);
    FuzzySearch::data.push_back(title);
}
void FuzzySearch::process_file()
{
    while (true)
    {
        string filename;
        int c;
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (!filenames_queue.empty())
            {
                filename = filenames_queue.front();
                filenames_queue.pop();
            }
            else
            {
                break;
            }
        }

        if (!filename.empty())
        {
            fstream titleFile(filename);
            if (titleFile.is_open())
            {
                string line;
                while (std::getline(titleFile, line))
                {
                    parse_line(line);
                }
                titleFile.close();
            }
            else
            {
                std::cerr << "Unable to open titleFile: " << filename << std::endl;
            }
        }
    }
}
void FuzzySearch::distribute_and_start_threads(unsigned int numthreads)
{
    cout<<"start distribute_and_start_threads"<<endl;
    string titleIndex1 = file_path + "/title/title";
    string titleIndex2;

    for (int i = 0; i < 5021; ++i)
    {
        titleIndex2 = titleIndex1 + to_string(i) + ".txt";
        filenames_queue.push(titleIndex2);
    }

    vector<thread> threads;

    while (threads.size() < numthreads)
    {
        threads.emplace_back(&FuzzySearch::process_file, this);
    }

    // 等待所有线程完成
    for (auto &t : threads)
    {
        t.join();
    }

    threads.clear();

    isInited = true;

    cout<<"end distribute_and_start_threads"<<endl;
}
FuzzySearch::FuzzySearch()
{
    num_threads = thread::hardware_concurrency();
}

void FuzzySearch::parallel_fuzzy_search(const vector<string> &keywords)
{
    if(!isInited)
    {
        this->distribute_and_start_threads(num_threads);
    }

    matched_titles.clear();
    cout<<"start parallel_fuzzy_search"<<endl;

    vector<SearchTask> tasks;
    const size_t data_size = data.size();
    const size_t chunk_size = data_size / num_threads + (data_size % num_threads == 0 ? 0 : 1);

    auto begin = data.cbegin();
    for (size_t i = 0; i < num_threads; ++i)
    {
        auto end = std::next(begin, chunk_size);
        tasks.emplace_back(begin, end, keywords);
        begin = end;
    }

    // 对于最后一块数据，确保包括剩余的所有元素
    tasks.back().end_ = this->data.cend();

    vector<std::thread> threads;

    threads.reserve(tasks.size());

    for (auto &task: tasks)
        threads.emplace_back(std::ref(task));

    for (auto& t : threads)
        t.join();

    for (const auto& task : tasks)
    {
        std::copy(task.local_matches.begin(), task.local_matches.end(),
                  std::inserter(matched_titles, matched_titles.end()));
    }

    cout<<"end parallel_fuzzy_search"<<endl;

}

void FuzzySearch::initPath(const string &str)
{
    file = str;
    file_path = getLastPrefix(str) + "/Index";
}
