//
// Created by 追影子的熊 on 2025/5/25.
//

#include "newInit.h"


// 检查文件是否存在并且能够成功打开
bool newInit::is_inited()
{
    string flat_path = file_path + "/inited.txt";
    std::ifstream flagFile(flat_path);
    return flagFile.good();
}

// 设置初始化标志文件
void newInit::set_inited_flag()
{
    string flat_path = file_path + "/inited.txt";
    std::ofstream flagFile(flat_path);
    if (flagFile.is_open()) {
        flagFile << "newInitialization completed.";
        flagFile.close();
        cout << "newInitialization completed!" << endl;
    } else {
        std::cerr << "Error: Unable to create initialization flag file." << std::endl;
    }
}

// 创建目录
void newInit::creat_dir(const string &subDir)
{
    string dir = file_path + subDir;
    if (!std::filesystem::exists(dir)) {
        std::filesystem::create_directories(dir);
        if (!std::filesystem::exists(dir)) {
            cerr << "Create dir '" << dir << "' fail!" << endl;
        } else {
            cout << "Dir '" << dir << "' create success。" << endl;
        }
    } else {
        cout << "Dir '" << dir << "' existed。" << endl;
    }
}

// 索引初始化基函数
void newInit::ini()
{
    // 创建索引文件子目录
    creat_dir("/author");
    creat_dir("/title");
    creat_dir("/year");
    creat_dir("/graph");
    cout << "start parseXml" << endl;
    // 解析XML文件
    parseXml();
    cout << "save file " << endl;
    // 保存索引文件
    save_file();
}

// Parse XML file
void newInit::parseXml() {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(file.c_str(), pugi::parse_default | pugi::parse_fragment);

    if (!result) {
        std::cerr << "Error: Unable to load XML file: " << result.description() << std::endl;
        return;
    }

    SAXHandler handler(this);
    doc.traverse(handler);

    cout << "The count is " << handler.pos_last << endl;
}

// SAXHandler implementation
bool SAXHandler::for_each(pugi::xml_node &node) {

    string nodeName = node.name();
    string content = node.child_value();

    if (nodeName == "title") {
        if (content.find('&') != string::npos) {
            init->getHtmlData(content);
        }
        init->titleCache = content;
        unsigned int slot = hash(content);
        init->pushTitle(slot, content, init->current_offset); // 使用当前偏移量
        if (init->yearCache != 0) {
            init->pushYear(init->yearCache, init->titleCache, init->current_offset);
        }
    } else if (nodeName == "author") {
        if (content.find('&') != string::npos) {
            init->getHtmlData(content);
        }
        unsigned int slot = hash(content);
        init->pushAuthor(slot, content, init->current_offset); // 使用当前偏移量
    } else if (nodeName == "year") {
        if (content.find('&') != string::npos) {
            init->getHtmlData(content);
        }
        if (func::isNumeric(content)) {
            init->yearCache = std::stoi(content);
            if (init->titleCache != "\0") {
                init->pushYear(init->yearCache, init->titleCache, init->current_offset);
            }
        } else {
            cout << "The year is " << content << " and the title is " << init->titleCache << endl;
        }
    } else if (func::findPublicRecord(nodeName)) {
        // Reset caches for the next article
        init->titleCache = "\0";
        init->yearCache = 0;
        init->current_offset = node.offset_debug()-1; // 更新当前偏移量
    }

    return true;
}

void newInit::pushAuthor(unsigned int slot, const string &author, long long pos)
{
    authors.all_author[slot].slot_author[author].push_back(pos);
}

void newInit::pushTitle(unsigned int slot, const string &title, long long int pos)
{
    titles.all_title[slot].slot_title.emplace(title, pos);
}

void newInit::pushYear(unsigned int year, const string &title, long long int pos)
{
    years.all_year[year - 1936].slot_year.emplace(title, pos);
}

// Save index files
void newInit::save_file() {
    fstream save;
    string file_name;
    for (int i = 0; i < 5021; i++) {
        file_name = file_path + "/author/author" + to_string(i) + ".txt";
        save.open(file_name, ios::out);
        if (save.is_open()) {
            for (const auto &entry : authors.all_author[i].slot_author) {
                save << entry.first << "$$";
                for (const auto &pos : entry.second) {
                    save << pos << " ";
                }
                save << "\n";
            }
            save.close();
        } else {
            std::cerr << "Unable to open the author save!" << std::endl;
        }
    }
    for (int i = 0; i < 5021; i++) {
        file_name = file_path + "/title/title" + to_string(i) + ".txt";
        save.open(file_name, ios::out);
        if (save.is_open()) {
            for (const auto &entry : titles.all_title[i].slot_title) {
                save << entry.first << "$$" << entry.second << "\n";
            }
            save.close();
        } else {
            std::cerr << "Unable to open the title save!" << std::endl;
        }
    }
    int year = 1936;
    for (const auto &entry : years.all_year) {
        if (entry.slot_year.empty()) {
            ++year;
            continue;
        } else {
            file_name = file_path + "/year/" + to_string(year) + ".txt";
            save.open(file_name, ios::out);
            if (save.is_open()) {
                for (const auto &e : entry.slot_year) {
                    save << e.first << "$$" << e.second << "\n";
                }
                save.close();
            } else {
                std::cerr << "Unable to open the year save!" << std::endl;
            }
        }
        ++year;
    }
}

bool newInit::initPath(const string &str)
{
    file = str;
    file_path = getLastPrefix(str) + "/Index";
    return is_inited();
}

newInit::newInit()
{
    html.setAcceptRichText(true);
}

void newInit::getHtmlData(string &str)
{
    QString textWithEntities = QString::fromStdString(str);
    html.setHtml(textWithEntities);
    QString plainText = html.toPlainText();
    str = plainText.toStdString();
}