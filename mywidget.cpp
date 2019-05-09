#include "mywidget.h"

#include<string>
#include<QFileDialog>
#include<QMessageBox>
#include <fstream>

#include <QTextStream >
#include <QDebug>
#include <QCheckBox>

#include "classcalculate.h"

MyWidget::MyWidget(std::wstring const &path,QWidget *parent)
    : QWidget(parent)
{
   _openfilePath =  path;

  resize(450,250);
  setWindowIcon(QIcon("C:\\Users\\tingzhang\\Documents\\fileTool\\source\\fileTooTitle.jpg"));
  setWindowTitle("神勘");
  QVBoxLayout *mainLayout = new QVBoxLayout;
  QHBoxLayout *srcLayout  = new QHBoxLayout;

  QHBoxLayout *desLineLayout = new QHBoxLayout;
  QPushButton * openSrcDirButton = new QPushButton;
  openSrcDirButton->setText("...");
  openSrcDirButton->setObjectName("openSrcDirButton");

  QPushButton * openDesButton = new QPushButton;
  openDesButton->setText("...");
  openDesButton->setObjectName("openSrcDirButton");


  QPushButton * openNoFindPhotoTxtButton = new QPushButton;
  openNoFindPhotoTxtButton->setText(tr("打开没匹配文件"));
  QPushButton * openPhotoNoSameSizetxtButton = new QPushButton;
  openPhotoNoSameSizetxtButton->setText("打开大小不一致文件");

  QHBoxLayout *topLayout = new QHBoxLayout;
  topLayout->addStretch();
  topLayout->addWidget(openNoFindPhotoTxtButton);
  topLayout->addWidget(openPhotoNoSameSizetxtButton);

  QLabel *srcDirLabel = new QLabel(tr("图源文件路径："));
  QLabel *desDirLabel = new QLabel(tr("目标文件路径："));
  _srcDirLine = new QLineEdit;
  _desDirLine = new QLineEdit;
  _srcDirLine->setPlaceholderText("请输入源路径.......");
  _desDirLine->setPlaceholderText("请输入目标路径.....");
  srcLayout->addWidget(srcDirLabel);
  srcLayout->addWidget(_srcDirLine);
  srcLayout->addWidget(openSrcDirButton);

  desLineLayout->addWidget(desDirLabel);
  desLineLayout->addWidget(_desDirLine);
  desLineLayout->addWidget(openDesButton);

  QHBoxLayout *buttonLayout  =new QHBoxLayout;
  QPushButton *startButton = new QPushButton;
  startButton->setText(tr("开始"));
  QCheckBox *_isFilterNightPhoto = new QCheckBox("过滤源目录夜晚图像");
  _isFilterNightPhoto->setChecked(true);
  QCheckBox *_isMatchPhotoSize = new QCheckBox("图片大小对比");
  buttonLayout->setMargin(0);
  buttonLayout->addWidget(_isFilterNightPhoto);
  buttonLayout->addWidget(_isMatchPhotoSize);
  buttonLayout->addStretch();
  buttonLayout->addWidget(startButton);

  mainLayout->addSpacing(3);
  mainLayout->addLayout(srcLayout);
  mainLayout->addSpacing(3);
  mainLayout->addLayout(desLineLayout);
  mainLayout->addLayout(buttonLayout);


  QHBoxLayout *bottomLayout  =new QHBoxLayout;
  QLabel *tipLabel = new QLabel("没有匹配的文件数:");
  QLabel *tipMathLabel = new QLabel("匹配的文件数:");
  _noMatchFileNumLabel = new QLabel("0");
  _matchFileNumLabel = new QLabel("0");

  bottomLayout->addWidget(tipLabel);
  bottomLayout->addWidget(_noMatchFileNumLabel);
  bottomLayout->addSpacing(3);
  bottomLayout->addWidget(tipMathLabel);
  bottomLayout->addWidget(_matchFileNumLabel);
  bottomLayout->addStretch();

  mainLayout->addLayout(bottomLayout);
  mainLayout->addLayout(topLayout);
  //main layolut
  //
  setLayout(mainLayout);
  connect(startButton,SIGNAL(clicked()),this,SLOT(start()));
  connect(openSrcDirButton,SIGNAL(clicked()),this,SLOT(on_openSrcDirPushButton_clicked()));
  connect(openDesButton,SIGNAL(clicked()),this,SLOT(on_openSrcDirPushButton_clicked()));
   //打开文档的按钮
  connect(openNoFindPhotoTxtButton,SIGNAL(clicked()),this,SLOT(on_openNoFindPhotoTxtButton_clicked()));
  connect(openPhotoNoSameSizetxtButton,SIGNAL(clicked()),this,SLOT(on_openPhotoNoSameSizetxtButton_clicked()));
  connect(openPhotoNoSameSizetxtButton,SIGNAL(clicked()),this,SLOT(slotOutPutDiffPhotoSize()));

}

void MyWidget::on_openNoFindPhotoTxtButton_clicked()
{
    _noFindPhotoFindPath = _openfilePath + L"noFindPhoto.txt";
  QString noFindMatchSizePhoto = QString::fromStdWString(_noFindPhotoFindPath);
  QString tempPath = QString::fromStdWString(_openfilePath);

  QFileInfo file(noFindMatchSizePhoto);
  if(file.exists()==false){
      createFile(tempPath,"noFindPhoto1.txt");

  }
  QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"),noFindMatchSizePhoto,tr("Text File (*.txt)"));


}
void MyWidget::on_openPhotoNoSameSizetxtButton_clicked()
{
    _noFindPhotoFindPath = _openfilePath + L"noMatchSizePhoto.txt";
  QString noFindMatchSizePhoto = QString::fromStdWString(_noFindPhotoFindPath);
   QString tempPath = QString::fromStdWString(_openfilePath);

  QFileInfo file(noFindMatchSizePhoto);
  if(file.exists()==false){
      createFile(tempPath,"noMatchSizePhoto.txt");

  }
  QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"),noFindMatchSizePhoto,tr("Text File (*.txt)"));

}

/**
 * @brief  Get all file names in a directory ，saved in vector<> file
 */
void MyWidget::getFiles(const std::string & path, std::vector<std::string> &files)
{
    //文件句柄
    long hFile = 0;
    //文件信息，_finddata_t需要io.h头文件
    struct _finddata_t fileinfo;
    std::string p;
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
    {
        do{
        //如果是目录,迭代之
        //如果不是,加入列表
        if ((fileinfo.attrib & _A_SUBDIR)) {
            if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                getFiles(p.append(fileinfo.name), files);
        }
        else
        {
        //	files.push_back(p.assign(path).append("\\").append(fileinfo.name));
            files.push_back(fileinfo.name);
        }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }

}
/**
 * @brief if destinantion not have the file create a new one
 */
void MyWidget::createFile(QString filePath,QString fileName)
{
    QDir tempDir;
    //临时保存程序当前路径
    QString currentDir = tempDir.currentPath();
    //如果filePath路径不存在，创建它
    if(!tempDir.exists(filePath))
    {
        qDebug()<<"不存在该路径"<<endl;
        tempDir.mkpath(filePath);
    }
    QFile *tempFile = new QFile;
    //将程序的执行路径设置到filePath下
    tempDir.setCurrent(filePath);
    qDebug()<<tempDir.currentPath();
    //检查filePath路径下是否存在文件fileName,如果停止操作。
    if(tempFile->exists(fileName))
    {
        qDebug()<<"文件存在";
        return ;
    }
    //此时，路径下没有fileName文件，使用下面代码在当前路径下创建文件
    tempFile->setFileName(fileName);
    if(!tempFile->open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qDebug()<<"打开失败";
    }
    tempFile->close();
    //将程序当前路径设置为原来的路径
    tempDir.setCurrent(currentDir);
    qDebug()<<tempDir.currentPath();
}
std::wstring MyWidget::s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}


void MyWidget::slotOutPutDiffPhotoSize()
{
    //两个数据都有
    //第一步先找到匹配的
   // std::vector<std::string> _vsrcDayFilenames; //day phtotos
    //std::vector<std::string> _vsrcNightFilenames;//all file name of src path
    std::string srcPath =  _srcDirLine->text().toStdString();
    for(auto &srcDayPhoto :_vsrcDayFilenames)
    {
       std::string tmpString =  srcDayPhoto.insert(srcDayPhoto.size()-4, "-Night");
       //在夜晚的模式下进行搜索
       std::vector<std::string>::iterator it = std::find(_vsrcNightFilenames.begin(), _vsrcNightFilenames.end(), tmpString);
       //find and save the file of not maching
       if (it != _vsrcNightFilenames.end()){
           //发现后
          std::string pathPhoto = srcPath + "\\" +srcDayPhoto;
            unsigned int unWidth=0, unHeight=0;
         // ClassImage::GetJPEGWidthHeight(srcPath.c_str(),unHeight,unHeight);
           // ClassImage::GetJPEGWidthHeight(srcPath.c_str(),&unWidth,&unHeight);
       }

    }


}
/**
 * @brief start button
 */
void MyWidget::start()
{
    //const std::string &srcPath, const std::string &desPath
    std::string srcPath =  _srcDirLine->text().toStdString();
    std::string desPath =  _desDirLine->text().toStdString();
    std::vector<std::string> vsrcTmpSumNames;//all file name of src path
    std::vector<std::string> vdesFilenames;
    std::vector<std::string> vnoMachFilenames;
    getFiles(srcPath, vsrcTmpSumNames);
    getFiles(desPath, vdesFilenames);
    //过滤掉源目录的night文件
    if(_isFilterNightPhoto->isChecked()){
         filterSrcPhotoOfDay(vdesFilenames,_vsrcDayFilenames,_vsrcNightFilenames);
    }

    int tempNumOfMatchFile=0;
    for (auto &pathName : vdesFilenames)
    {
        std::vector<std::string>::iterator it = std::find(_vsrcDayFilenames.begin(), _vsrcDayFilenames.end(), pathName);
        //find and save the file of not maching
        if (it != _vsrcDayFilenames.end()){
        //发现以后
        tempNumOfMatchFile++;
        std::string absoluteSrcFilePath = srcPath + "\\" + pathName;
        pathName.insert(pathName.size()-4, "-Night");

        std::string absoluteDesFilePath = desPath + "\\" + pathName;
        //把这个文件放到目的地址
        std::wstring srcStemp = s2ws(absoluteSrcFilePath);
        LPCWSTR srcWin = srcStemp.c_str();

        std::wstring desStemp = s2ws(absoluteDesFilePath);
        LPCWSTR desWin = desStemp.c_str();
        //自动拷贝文件
        CopyFile(srcWin, desWin, FALSE);

        pathName = "";
        }
        else{
          vnoMachFilenames.push_back(pathName);
           std::cout<<pathName<<std::endl;
        }
    }
    _noMatchFileNumLabel->setText(QString::number(vnoMachFilenames.size()));
    _matchFileNumLabel->setText(QString::number(tempNumOfMatchFile));

    //把数据保存到文档当中
    std::string tempOutPut = QString::fromStdWString(_noFindPhotoFindPath).toStdString();
    creatTxt((char*)tempOutPut.c_str(),vnoMachFilenames);
    tempNumOfMatchFile=0;
}

void MyWidget::filterSrcPhotoOfDay(std::vector<std::string> &tempSrc,std::vector<std::string> &tempOutSrcDay,std::vector<std::string> &tempOutSrcNight)
{
   for(auto tempFile :tempSrc)
   {
       std::string tempSubStr = tempFile.substr(tempFile.size()-9,9);
       if(tempSubStr != "Night.png"){
           tempOutSrcDay.push_back(tempFile);
       }
       else{
            tempOutSrcNight.push_back(tempFile);
       }
   }

}
/**
 * @brief A default value for the measurement system from the system settings.
 */
void MyWidget::on_openSrcDirPushButton_clicked()
{
    //open a diagram of filePath
    _tempChangePath = "D:\\Awork\\ford-kipawa\\ford-qt-hmi\\ford\\AppThemeData\\12inch\\Ford-MY20-nav\\assets\\images\\navigation\\";
    QString dirPath = QFileDialog::getExistingDirectory(
               this, "choose src Directory",
                _tempChangePath);
    if (dirPath.isEmpty()){
        return;
    }
    _tempChangePath = dirPath;
    if(!isDirExist(dirPath)){
        QMessageBox::information(this,"you director is not existance!",QString::number(1));

    }

    QPushButton*btn = qobject_cast<QPushButton*>(sender());
     if("openSrcDirButton"==btn->objectName()) {
        _srcDirLine->setText(dirPath);
       }
    else{
        _desDirLine->setText(dirPath);
    }
}

bool MyWidget::isDirExist(QString fullPath)
{
    QFileInfo fileInfo(fullPath);
    if(fileInfo.isDir())
    {
      return true;
    }
    return false;
}

void MyWidget::creatTxt(char* pathName,std::vector<std::string> &pathStrings)//创建txt文件
{
  std::ofstream fout(pathName);
  if(!fout){
    return;
  }

  for(auto &pathstring:pathStrings){
     std::string tempString = pathstring.substr(pathstring.length() - 9 ,pathstring.length());
      if(tempString !="Night.png")
     fout <<pathstring<< std::endl; // 使用与cout同样的方式进行写入
  }
   fout.close();  // 执行完操作后关闭文件句柄
 }

MyWidget::~MyWidget()
{

}
