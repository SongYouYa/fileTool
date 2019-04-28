#include "mywidget.h"
#include <QApplication>
using namespace std;

int main(int argc, char *argv[])
{

   QApplication a(argc, argv);

   MyWidget widgetTool;
   widgetTool.show();

   return a.exec();

}
