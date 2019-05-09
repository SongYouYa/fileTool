#ifndef MYWIDGET_H
#define MYWIDGET_H
#include "windows.h"

#include <iostream>
#include <io.h>
#include <string>
#include <vector>
#include "string.h"

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class  QCheckBox;

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    MyWidget(std::wstring const &path,QWidget *parent =nullptr);
    void getFiles(const std::string & path, std::vector<std::string> &files);
    std::wstring s2ws(const std::string& s);
    bool isDirExist(QString fullPath);
    void createFile(QString filePath,QString fileName);
    void creatTxt(char* pathName,std::vector<std::string> &pathStrings);
    void filterSrcPhotoOfDay(std::vector<std::string> &tempSrc,\
                             std::vector<std::string> &tempOutSrcDay,\
                             std::vector<std::string> &tempOutSrcNight);
    ~MyWidget();

public slots:
    void start();
    void slotOutPutDiffPhotoSize();
    void on_openSrcDirPushButton_clicked();
    void on_openNoFindPhotoTxtButton_clicked();
    void on_openPhotoNoSameSizetxtButton_clicked();


private:
     QLineEdit*      _srcDirLine;
     QLineEdit*      _desDirLine;
     QString         _tempChangePath;
     QLabel *        _noMatchFileNumLabel;
     QLabel *        _matchFileNumLabel;
     std::wstring         _openfilePath;
     std::wstring    _noFindPhotoFindPath;
     std::wstring    _noFileMatchSizePath;
     QCheckBox *     _isFilterNightPhoto;
     QCheckBox *     _isMatchPhotoSize;

     std::vector<std::string> _vsrcDayFilenames; //day phtotos
     std::vector<std::string> _vsrcNightFilenames;//all file name of src path

};

#endif // MYWIDGET_H
