//
// Created by 追影子的熊 on 2024/4/21.
//

#ifndef DBLP_WINDOW_H
#define DBLP_WINDOW_H

#include "AuthorTop.h"
#include "BasicSearch.h"
#include "FuzzySearch.h"
#include "KeywordTop.h"
#include <QGraphicsOpacityEffect>
#include <QMainWindow>
#include <QTreeWidgetItem>
#include "newInit.h"


QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
//    Init init;
    newInit init2;
    SAXHandler sax;
    BasicSearch bs;
    bool inited = false;
    string file;
    AuthorTop authorTop;
    KeywordTop keywordTop;
    FuzzySearch fuzzySearch;
    QTextDocument* doc;

    void notInited();
    void haveInited();
    void initing();
    void articleSearch();
    void authorSearch();
    void coAuthorSearch();
    void showCoAuthor(vector<vector<vector<string>>> & v);
    void showText(vector<vector<string>> str,int i);

    void showAuthorProcess();

    void showFuzzyResult();

    vector<QColor> mColor;

    bool isfuzzy = false;




public slots:
    void openFile();
    void checkInited(const QString & str);
    void startInit();
    void startSearch();


    void startTopAuthor();
    void initTopAuthorFile();
    void startTopKeyword();
    void initTopKeywordFile();
    void startFuzzy();

    void back();
    void doSearch();

    void showTopAuthorFile();
    void showTopKeywordFile();

};


#endif//DBLP_WINDOW_H
