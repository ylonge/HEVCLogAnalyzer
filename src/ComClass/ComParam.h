/*
 * Name: TComParam.h.
 * Function: Defination of parameters that should be ajusted mannually.
 * Author: Ylonge from MCL-ZJU, zxg from MCL-ZJU, Yin Zhao from Huawei.
 */
#pragma once

#include "TypeDef.h"
#include "program_options_lite.h"

#include <vector>

class ComParam
{
public:

  // Common file names.
  std::string m_sNameOfVidLogFile;
	std::string m_sNameOfLibLogFile;
	std::string m_sNameOfVidLibMapFile;
	std::string m_sNameOfSceneChangePOCFile;
	std::string m_sNameOfOutResultFile;
	std::string m_sNameOfAnchorVidFile;

  // Parameters of Video.
  unsigned int m_uiNumOfVidPic;
  double m_dFrameRate;
  unsigned int m_uiMaxIntraPeroid;

  // Operating Parameters.
  unsigned int m_uiStartNumOfLogLine;
  unsigned int m_uiEndNumOfLogLine;
  unsigned int m_uiNumOfLogLine;
  EnumTypeOfCodec m_eTypeOfCodec;
	EnumTypeOfResult m_eTypeOfResult;

public:
  // Constructor function.
  ComParam();
  ~ComParam();

  // Memory Operation.
	void init();
	void destroy();

  // Parse the parameter.
	Bool runParseParam(int argc, char* argv[]);
};