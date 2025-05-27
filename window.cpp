//
// Created by 追影子的熊 on 2024/4/21.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "window.h"
#include "QColorDialog"
#include "ui_MainWindow.h"
#include <QInputDialog>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), sax(&init2)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->getFile, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(ui->file_path,&QLineEdit::textChanged,this,&MainWindow::checkInited);
    connect(ui->init,&QPushButton::clicked,this,&MainWindow::startInit);
    connect(ui->search,&QPushButton::clicked,this,&MainWindow::startSearch);
    connect(ui->topAuthor,&QPushButton::clicked,this,&MainWindow::startTopAuthor);
    connect(ui->initTopAuthor,&QPushButton::clicked,this,&MainWindow::initTopAuthorFile);
    connect(ui->getTopAuthor,&QPushButton::clicked,this,&MainWindow::showTopAuthorFile);
    connect(ui->topKeyword,&QPushButton::clicked,this,&MainWindow::startTopKeyword);
    connect(ui->initTopKeyword,&QPushButton::clicked,this,&MainWindow::initTopKeywordFile);
    connect(ui->getTopKeyword,&QPushButton::clicked,this,&MainWindow::showTopKeywordFile);
    connect(ui->fuzzy,&QPushButton::clicked,this,&MainWindow::startFuzzy);
    connect(ui->back,&QPushButton::clicked,this,&MainWindow::back);
    connect(ui->basicSearch,&QPushButton::clicked,this,&MainWindow::doSearch);

    mColor = {QColor(255,0,0,255),QColor(255,85,255,255),QColor(0,170,255,255),
            QColor(0,170,0,255),QColor(255,100,0,255),QColor(210,30,255,255),
            QColor(85,85,127,255),QColor(170,0,255,255)};

    doc = ui->information->document();

    ui->tip->hide();

    for(int i = 1936;i<=2024;i++)
    {
        ui->getYear->addItem(QString::fromStdString(to_string(i)));
    }


    ui->label->hide();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete doc;

}
void MainWindow::openFile()
{
    // 打开文件对话框
    QString filePath = QFileDialog::getOpenFileName(
        this,           // 这个参数是父窗口指针，一般传入this
        QStringLiteral("选择XML文件"),   // 对话框标题
        R"(E:\code\CLion project\dblp_xml\cmake-build-debug\dblp.xml)",   // 默认打开路径，这里是用户的家目录
        QStringLiteral("XML文件 (*.xml)")); // 文件类型筛选器，这里接受所有类型的文件

    if (!filePath.isEmpty()) {  // 检查用户是否真的选择了文件
        ui->file_path->setText(filePath);
    } else {
        QMessageBox::information(this, "提示", "您未选择任何文件");
    }
}
void MainWindow::checkInited(const QString &str)
{
    this->init2.initPath(str.toStdString());
    this->fuzzySearch.initPath(str.toStdString());
    inited = this->init2.initPath(str.toStdString());
    this->file = str.toStdString();
    if(!inited)
    {
        notInited();
    }
    else
    {
        haveInited();
    }
}
void MainWindow::startInit()
{
    if(file.empty())
    {
        QMessageBox::warning(this,"错误","文件路径错误");
    }
    if(inited)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(nullptr, "提示", "索引已建立，是否重新初始化索引", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) {
            return;
        }
    }
    initing();
    init2.ini();
    init2.set_inited_flag();
    haveInited();
}
void MainWindow::startSearch()
{
    this->bs.initPath(file);
    this->ui->stackedWidget->setCurrentIndex(1);
}
void MainWindow::articleSearch()
{
    const string input = ui->input->text().toStdString();
    if(input.empty()) return;
    bs.getPosFormTitle(input);
    if(bs.articlePos.empty())
    {
        QMessageBox::information(this,"提示","文献不存在");
        return;
    }
    vector<vector<string>> info = func::getArticleInformation(bs.articlePos[0],file);

    showText(info,-1);

}
void MainWindow::authorSearch()
{

    vector<vector<vector<string>>> infoTemp;
    const string input = ui->input->text().toStdString();
    if(input.empty()) return;
    bs.getPosFormAuthor(input);
    if(bs.articlePos.empty())
    {
        QMessageBox::information(this,"提示","作者不存在");
        return;
    }
    for(long long articlePo : bs.articlePos)
    {
        infoTemp.push_back(func::getArticleInformation(articlePo,file));
    }
    for(int i = 0;i<infoTemp.size();i++)
    {
        showText(infoTemp[i],i+1);
    }

}
void MainWindow::startTopAuthor()
{
    authorTop.initPath(this->file);
    this->ui->stackedWidget->setCurrentIndex(2);
}
void MainWindow::initTopAuthorFile()
{


    authorTop.processAuthorCountData();
    showAuthorProcess();

}
void MainWindow::startTopKeyword()
{
    keywordTop.initPath(this->file);
    this->ui->stackedWidget->setCurrentIndex(3);
}
void MainWindow::initTopKeywordFile()
{
    keywordTop.getTitle();
    keywordTop.countAndSortKeywords(50);

}
void MainWindow::startFuzzy()
{
    if(!isfuzzy)
    {
        ui->tip->show();
        ui->tip->raise();
        ui->tip->update();
    }
    vector<string> keyword;
    if(!ui->keyword1->text().isEmpty())
    {
        keyword.push_back(ui->keyword1->text().toStdString());
    }
    if(!ui->keyword2->text().isEmpty())
    {
        keyword.push_back(ui->keyword2->text().toStdString());
    }
    if(!ui->keyword3->text().isEmpty())
    {
        keyword.push_back(ui->keyword3->text().toStdString());
    }
    fuzzySearch.parallel_fuzzy_search(keyword);

    showFuzzyResult();
    ui->tip->hide();
}

void MainWindow::back()
{
    ui->stackedWidget->setCurrentIndex(0);
}
void MainWindow::notInited()
{
    ui->label->hide();
}
void MainWindow::haveInited()
{
    ui->label->show();
    ui->label->raise();
}
void MainWindow::initing()
{
    ui->label->hide();
}
void MainWindow::doSearch()
{
    switch(ui->comboBox->currentIndex())
    {
        case 0:
        {
            articleSearch();
            break;
        }
        case 1:
        {
            authorSearch();
            break;
        }
        case 2:
        {
            coAuthorSearch();
            break;
        }
    }
}
void MainWindow::showText(vector<vector<string>> str,int i)
{
    doc = ui->information->document();
    if(i==-1||i==1)
    {
        doc->clear();
    }
    doc->setParent(this);
    QTextCursor cursor(doc);
    cursor.movePosition(QTextCursor::End);
    QTextCharFormat format;
    if(i != -1)
    {
        format.setFontPointSize(24);
        format.setFontWeight(QFont::Bold|QFont::Black);
        cursor.insertText("文献" + QString::fromStdString(to_string(i)) +  "\n", format);
    }
    format.setForeground(mColor[0]);
    format.setFontPointSize(18);
    format.setFontWeight(QFont::Bold);
    cursor.insertText(QString::fromStdString(str[0][0]) + "\n", format);
    for(int i = 1;i<str.size();i++)
    {
        format.setForeground(mColor[i%8]);
        format.setFontPointSize(14);
        format.setFontWeight(QFont::Bold);
        cursor.insertText( QString::fromStdString(str[i][0]) + "\n", format);
        format.setFontWeight(QFont::Normal);
        for(int j = 1;j<str[i].size();j++)
        {
            cursor.insertText(QString::fromStdString(str[i][j]) + "\n", format);
        }
    }
    ui->information->setTextCursor(cursor);
}
void MainWindow::coAuthorSearch()
{
    vector<vector<vector<string>>> infoTemp;
    const string input = ui->input->text().toStdString();
    if(input.empty()) return;
    bs.getPosFormAuthor(input);
    if(bs.articlePos.empty())
    {
        QMessageBox::information(this,"提示","作者不存在");
        return;
    }
    for(long long articlePo : bs.articlePos)
    {
        infoTemp.push_back(func::getArticleInformation(articlePo,file));
    }

    showCoAuthor(infoTemp);
}
void MainWindow::showCoAuthor(vector<vector<vector<string>>> &v)
{
    doc = ui->information->document();
    doc->clear();
    QTextCursor cursor(doc);
    cursor.movePosition(QTextCursor::End);
    QTextCharFormat format;
    for(int i = 0;i<v.size();i++)
    {
        format.setFontPointSize(24);
        format.setFontWeight(QFont::Bold|QFont::Black);
        format.setForeground(mColor[i%8]);
        cursor.insertText("文献" + QString::fromStdString(to_string(i)) +  "\n", format);
        format.setFontPointSize(18);
        format.setFontWeight(QFont::Bold);
        cursor.insertText(QString::fromStdString(v[i][0][0]) + "\n", format);
        format.setFontWeight(QFont::Normal);
        format.setFontPointSize(14);
        for(int j = 1; j<v[i].size();j++)
        {
            if(v[i][j][0] == "title")
            {
                cursor.insertText("合作文章标题：\n", format);
                cursor.insertText(QString::fromStdString(v[i][j][1]) + "\n\n", format);
            }
            else if(v[i][j][0] == "author")
            {
                cursor.insertText("合作作者：\n", format);
                for(int k = 1;k<v[i][j].size();k++)
                {
                    cursor.insertText(QString::fromStdString(v[i][j][k]) + "\n", format);
                }
                cursor.insertText("\n", format);
            }
        }
    }
    ui->information->setTextCursor(cursor);
}
void MainWindow::showTopAuthorFile()
{
    this->authorTop.getTopAuthor();
    doc = ui->author->document();
    doc->clear();
    doc->setParent(this);
    QTextCursor cursor(doc);
    cursor.movePosition(QTextCursor::End);
    QTextCharFormat format;

    format.setFontPointSize(16);
    format.setFontWeight(QFont::Bold);

    int size = authorTop.authors.size();

    for(int i = 0;i<authorTop.authors.size();i++)
    {
        format.setForeground(mColor[i%8]);
        cursor.insertText("Top" + QString::fromStdString(to_string(i)) + " Name: " +
                                  QString::fromStdString(authorTop.authors[size - i-1].name)+
                                  " Frequent: "+ QString::fromStdString(to_string(authorTop.authors[i].articleCount))
                                  + "\n", format);
    }
    ui->information->setTextCursor(cursor);
}
void MainWindow::showTopKeywordFile()
{
    this->keywordTop.getKeyWordTop(ui->getYear->currentIndex());
    doc = ui->keywords->document();
    doc->clear();
    doc->setParent(this);
    QTextCursor cursor(doc);
    cursor.movePosition(QTextCursor::End);
    QTextCharFormat format;

    format.setFontPointSize(16);
    format.setFontWeight(QFont::Bold);

    int size = keywordTop.keyWords.size();

    for(int i = 0;i<keywordTop.keyWords.size();i++)
    {
        format.setForeground(mColor[i%8]);
        cursor.insertText("Top" + QString::fromStdString(to_string(i)) + " Keyword: " +
                                  QString::fromStdString(keywordTop.keyWords[size -1-i].keyword)+
                                  " Frequent: "+ QString::fromStdString(to_string(keywordTop.keyWords[size -1-i].count))
                                  + "\n", format);
    }
    ui->information->setTextCursor(cursor);
}
void MainWindow::showFuzzyResult()
{
    doc = ui->information->document();
    doc->clear();
    doc->setParent(this);
    QTextCursor cursor(doc);
    cursor.movePosition(QTextCursor::End);
    QTextCharFormat format;
    format.setFontPointSize(14);
    format.setFontWeight(QFont::Bold);
    cursor.insertText("一共有" + QString::fromStdString(to_string(fuzzySearch.matched_titles.size())) + "个匹配结果\n");
    format.setFontWeight(QFont::Normal);
    int i = 1;
    for(auto j : fuzzySearch.matched_titles)
    {
        cursor.insertText(QString::fromStdString(to_string(i++) + "  " + FuzzySearch::data[j])+ "\n\n", format);
    }
    ui->information->setTextCursor(cursor);
}

void MainWindow::showAuthorProcess()
{
    this->authorTop.getTopAuthor();
    doc = ui->author->document();
    doc->clear();
    doc->setParent(this);
    QTextCursor cursor(doc);
    cursor.movePosition(QTextCursor::End);
    QTextCharFormat format;

    format.setFontPointSize(24);
    format.setFontWeight(QFont::Bold);
    cursor.insertText("初始化完成");
    ui->information->setTextCursor(cursor);
    ui->information->update();
}

