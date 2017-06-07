/*
 * File name: ComPic.h.
 * Function:
 * Author: Ylonge from MCL-ZJU.
 * Date: 2015/10/6.
 */
#pragma once

#include <stdlib.h>

class ComPic
{
private:

  // Components storing the data of one yuv picture.
  unsigned char *m_pucCompY;
  unsigned char *m_pucCompU;
  unsigned char *m_pucCompV;

  // Basic parameters of one yuv picture.
  unsigned int m_uiWidth;
  unsigned int m_uiHeight;

public:

  ComPic();
  ~ComPic();

  // Basic parameters initialization.
  void ParamInit(unsigned int uiWidth, unsigned int uiHeight);

  // Memory control.
  void Create();
  void Destroy();
  void Clear();

  // Get address of yuv components.
  unsigned char *GetAddrCompY() { return m_pucCompY; };
  unsigned char *GetAddrCompU() { return m_pucCompU; };
  unsigned char *GetAddrCompV() { return m_pucCompV; };

  // Get basic parameters of one yuv picture.
  unsigned int GetPicWidth()  { return m_uiWidth; };
  unsigned int GetPicHeight() { return m_uiHeight; };

  // Functions.
};