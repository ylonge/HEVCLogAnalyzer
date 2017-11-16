/*
 * File name: ComCBVCLogAnalyze.h.
 * Function:
 * Author: Ylonge from MCL-ZJU.
 * Date: 2015/11/18.
 */
#pragma once

#include <stdlib.h>
#include <vector>

#include "ComClass/TypeDef.h"
#include "ComClass/ComParam.h"
#include "ComLogAnalyze.h"

class ComCBVCLogAnalyze
{
private:

  /************************************************************************/
  /* Common Data.
  /************************************************************************/

  ComLogAnalyze m_cVidLog;
  ComLogAnalyze m_cLibLog;
	ComLogAnalyze m_cAnchorVidLog;

  std::vector<std::vector<unsigned int>> m_vvuiMaskOfVidPic;
  std::vector<std::vector<unsigned int>> m_vvuiMapOfVidSceneChangePic;

  double m_dFrameRate;
  EnumTypeOfCodec m_eTypeOfCodec;
  unsigned int m_uiNumOfVidPic;
  unsigned int m_uiMaxIntraPeroid;

  std::vector<double> m_vdAverageRate;
  std::vector<double> m_vdAveragePSNRY;
  std::vector<double> m_vdAveragePSNRU;
  std::vector<double> m_vdAveragePSNRV;

	// access channel.
	ComParam* m_pcParam;

public:
  ComCBVCLogAnalyze();
  ~ComCBVCLogAnalyze();

  /************************************************************************/
  /* Initialization
  /************************************************************************/

  void InitParam(ComParam & cParam);
  void InitMemory();
  void DestroyMemory();

  /************************************************************************/
  /* Fetch value.
  /************************************************************************/

  std::vector<double> GetAverageRate() {return m_vdAverageRate;};
  std::vector<double> GetAveragePSNRY() {return m_vdAveragePSNRY;};
  std::vector<double> GetAveragePSNRU() {return m_vdAveragePSNRU;};
  std::vector<double> GetAveragePSNRV() {return m_vdAveragePSNRV;};

  /************************************************************************/
  /* File operation.
  /************************************************************************/

  void ReadMapFile();
  void ReadSceneChangePOCFile();
  void WriteResult();

  /************************************************************************/
  /* Value Processing.
  /************************************************************************/

  void GenerateMapOfVidLib();
  void GetSceneAverageData();
  void CalcAverageData();
  void KeyPicExpand(std::vector<unsigned int> & vuiSrcSceneChangePOC);

	void runCalculateRandomAccess();
	void runCalculateAnchorRandomAccess();
};