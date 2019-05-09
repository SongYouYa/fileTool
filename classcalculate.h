#ifndef CLASSCALCULATE_H
#define CLASSCALCULATE_H

#include <QWidget>

class ClassImage
{

public:
  ClassImage()
  {
  }
  static int GetPNGWidthHeight(const char* path, unsigned int* punWidth, unsigned int* punHeight);
  static int GetJPEGWidthHeight(const char* path, unsigned int *punWidth, unsigned int *punHeight);
  static void GetPicWidthHeight(const char* path, unsigned int *punWidth, unsigned int *punHeight);
  static long  m_Width;
  static long  m_Height;

 static int get_extension(std::string fname);
};

#endif // CLASSCALCULATE_H
