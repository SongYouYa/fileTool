#include "mywidget.h"

#include<string>
#include<QFileDialog>
#include<QMessageBox>

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
{
  resize(360,180);
  setWindowIcon(QIcon("C:\\Users\\tingzhang\\Documents\\fileTool\\source\\fileTooTitle.jpg"));
  QVBoxLayout *mainLayout = new QVBoxLayout;
  QHBoxLayout *srcLayout  = new QHBoxLayout;


  QHBoxLayout *desLineLayout = new QHBoxLayout;
  QPushButton * openSrcDirButton = new QPushButton;
  openSrcDirButton->setText("...");
  openSrcDirButton->setObjectName("openSrcDirButton");

  QPushButton * openDesButton = new QPushButton;
  openDesButton->setText("...");
  QLabel *srcDirLabel = new QLabel(tr("srcPath"));
  QLabel *desDirLabel = new QLabel(tr("desPath"));
  _srcDirLine = new QLineEdit;
  _desDirLine = new QLineEdit;
  _srcDirLine->setPlaceholderText("input src path.....");
  _desDirLine->setPlaceholderText("input des path.....");
  srcLayout->addWidget(srcDirLabel);
  srcLayout->addWidget(_srcDirLine);
  srcLayout->addWidget(openSrcDirButton);

  desLineLayout->addWidget(desDirLabel);
  desLineLayout->addWidget(_desDirLine);
  desLineLayout->addWidget(openDesButton);

  QHBoxLayout *buttonLayout  =new QHBoxLayout;
  buttonLayout->addSpacing(78);
  QPushButton *startButton = new QPushButton;
  startButton->setText(tr("start"));
  buttonLayout->addWidget(startButton);

  mainLayout->addLayout(srcLayout);
  mainLayout->addSpacing(3);
  mainLayout->addLayout(desLineLayout);
  mainLayout->addLayout(buttonLayout);


  QHBoxLayout *bottomLayout  =new QHBoxLayout;
   QLabel *tipLabel = new QLabel("no match file num:");
    QLabel *tipMathLabel = new QLabel("no match file num:");
  _noMatchFileNumLabel = new QLabel("0");
  _matchFileNumLabel = new QLabel("0");

  bottomLayout->addWidget(tipLabel);
  bottomLayout->addWidget(_noMatchFileNumLabel);
  bottomLayout->addSpacing(3);
  bottomLayout->addWidget(tipMathLabel);
  bottomLayout->addWidget(_matchFileNumLabel);

  mainLayout->addLayout(bottomLayout);

  setLayout(mainLayout);

  connect(startButton,SIGNAL(clicked()),this,SLOT(start()));
  connect(openSrcDirButton,SIGNAL(clicked()),this,SLOT(on_openSrcDirPushButton_clicked()));
  connect(openDesButton,SIGNAL(clicked()),this,SLOT(on_openSrcDirPushButton_clicked()));
}


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
void MyWidget::start()
{
    //const std::string &srcPath, const std::string &desPath

     std::string srcPath =  _srcDirLine->text().toStdString();
     std::string desPath =  _desDirLine->text().toStdString();

    std::vector<std::string> vsrcFilenames;
    std::vector<std::string> vdesFilenames;
    std::vector<std::string> vnoMachFilenames;
//    std::string desPath= "D:\\Awork\\ford-kipawa\\ford-qt-hmi\\ford\\AppThemeData\\12inch\\Ford-MY20-nav\\assets\\images\\navigation\\assetsIcon\\trafficEventsListIcons";
//    std::string srcPath = "D:\\Awork\\photo\\nighetaskphoto";
    //std::getline(std::cin, path);
    getFiles(srcPath, vsrcFilenames);
    getFiles(desPath, vdesFilenames);
    int tempNumOfMatchFile=0;
    for (auto &pathName : vdesFilenames)
    {
        vector<std::string>::iterator it = std::find(vsrcFilenames.begin(), vsrcFilenames.end(), pathName);

        //find and save the file of not maching
        if (it != vsrcFilenames.end()){
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
    tempNumOfMatchFile=0;
}

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

MyWidget::~MyWidget()
{

}
