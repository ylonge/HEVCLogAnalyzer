/*
 * Name: TComParam.h.
 * Function: Defination of parameters that should be ajusted mannually.
 * Author: Ylonge from MCL-ZJU, zxg from MCL-ZJU, Yin Zhao from Huawei.
 */
#pragma once

#include "TypeDef.h"

class ComParam
{
public:

  // Common file names.
  char m_chNameOfVidLogFile[MAX_NUM_CHAR_NAME];
  char m_chNameOfLibLogFile[MAX_NUM_CHAR_NAME];
  char m_chNameOfMapFile[MAX_NUM_CHAR_NAME];
  char m_chNameOfSceneChangePOCFile[MAX_NUM_CHAR_NAME];
  char m_chNameOfOutResultFile[MAX_NUM_CHAR_NAME];

  // Parameters of Video.
  unsigned int m_uiNumOfVidPic;
  double m_dFrameRate;
  unsigned int m_uiMaxIntraPeroid;

  /************************************************************************/
  /* Operating Parameters.
  /************************************************************************/
  unsigned int m_uiStartNumOfLogLine;
  unsigned int m_uiEndNumOfLogLine;
  unsigned int m_uiNumOfLogLine;
  EnumTypeOfCodec m_eTypeOfCodec;

public:
  // Constructor function.
  ComParam();
  ~ComParam();

  // Memory Operation.
  void InitMemory();
  void DestroyMemory();

  // Parse the parameter.
  void InitParam();
  void ParseParam(Char cNameOfParamFile[]);
  int setParameter(char *paramName, char *paramValue);  
};