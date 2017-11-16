/*
 * File name: ComLogAnalyze.h.
 * Function:
 * Author: Ylonge from MCL-ZJU.
 * Date: 2015/11/17.
 */
#pragma once

#include <stdlib.h>
#include <vector>

#include "ComClass\TypeDef.h"
#include "ComClass\ComParam.h"

class ComLogAnalyze
{
public:

  /************************************************************************/
  /* Common Data.
  /************************************************************************/
  std::vector<double> m_vdPOC;
  std::vector<double> m_vdBit;
  std::vector<double> m_vdPSNRY;
  std::vector<double> m_vdPSNRU;
  std::vector<double> m_vdPSNRV;
  double m_dAverageRate;
  double m_dAveragePSNRY;
  double m_dAveragePSNRU;
  double m_dAveragePSNRV;
  double m_dFrameRate;

  /************************************************************************/
  /* Operating Parameters.
  /************************************************************************/
  unsigned int m_uiStartNumOfLogLine;
  unsigned int m_uiEndNumOfLogLine;
  unsigned int m_uiNumOfLogLine;
  EnumTypeOfCodec m_eTypeOfCodec;
  std::string m_sNameOfLogFile;
  std::string m_sNameOfResultFile;

	// access channel.
	ComParam *m_pcParam;

public:
  ComLogAnalyze();
  ~ComLogAnalyze();

  /************************************************************************/
  /* Initialization
  /************************************************************************/
	void init(ComParam &cParam);
  void InitParam(std::string sNameOfLogFile, std::string sNameOfOutFile, int iStartNum, int iEndNum, double dFrameRate, EnumTypeOfCodec eTypeOfCodec);
  void InitMemory();
  void DestroyMemory();

  /************************************************************************/
  /* Fetch value.
  /************************************************************************/

  double GetAverageRate() {return m_dAverageRate;};
  double GetAveragePSNRY() {return m_dAveragePSNRY;};
  double GetAveragePSNRU() {return m_dAveragePSNRU;};
  double GetAveragePSNRV() {return m_dAveragePSNRV;};
  unsigned int GetNumOfLogData() {return m_uiNumOfLogLine;};

  /************************************************************************/
  /* File operation.
  /************************************************************************/

  void ReadLogFile();
  void ReadSingleIntraLogFile();
  void WriteResult();
	void runReadSummary();

  /************************************************************************/
  /* Value Processing.
  /************************************************************************/

  void CalcAverageData();
	void runSort();
};