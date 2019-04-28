#ifndef MYWIDGET_H
#define MYWIDGET_H
#include "windows.h"
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <iostream>
#include <io.h>
#include <string>
#include <vector>
#include "string.h"

using namespace std;

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    MyWidget(QWidget *parent = 0);
    void getFiles(const std::string & path, std::vector<std::string> &files);

    std::wstring s2ws(const std::string& s);
    bool isDirExist(QString fullPath);
  ~MyWidget();

    public slots:
    void start();
    void on_openSrcDirPushButton_clicked();


private:
     QLineEdit *_srcDirLine;
     QLineEdit *_desDirLine;
     QString   _tempChangePath;
     QLabel *  _noMatchFileNumLabel;
      QLabel *  _matchFileNumLabel;

};

#endif // MYWIDGET_H
