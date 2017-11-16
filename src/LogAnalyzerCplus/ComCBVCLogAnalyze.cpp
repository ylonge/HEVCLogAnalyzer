/*
 * File name: ComCBVCLogAnalyze.c.
 * Function:
 * Author: Ylonge from MCL-ZJU.
 * Date: 2015/11/18.
 */

#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <assert.h>

#include "ComCBVCLogAnalyze.h"

using namespace std;

ComCBVCLogAnalyze::ComCBVCLogAnalyze()
{
}
ComCBVCLogAnalyze::~ComCBVCLogAnalyze()
{
}

/************************************************************************/
/* Initialization
/************************************************************************/

void ComCBVCLogAnalyze::InitParam(ComParam & cParam)
{
	m_pcParam = &cParam;

  m_dFrameRate = cParam.m_dFrameRate;
  m_eTypeOfCodec = cParam.m_eTypeOfCodec;
  m_uiNumOfVidPic = cParam.m_uiNumOfVidPic;
  m_uiMaxIntraPeroid = cParam.m_uiMaxIntraPeroid;
}

void ComCBVCLogAnalyze::InitMemory()
{
}

void ComCBVCLogAnalyze::DestroyMemory()
{
  if (m_vvuiMaskOfVidPic.size() > 0)
  {
    std::vector<std::vector<unsigned int>> m_vvuiEmpty;
    m_vvuiMaskOfVidPic.swap(m_vvuiEmpty);
  }

  if (m_vvuiMapOfVidSceneChangePic.size() > 0)
  {
    std::vector<std::vector<unsigned int>> m_vvuiEmpty;
    m_vvuiMapOfVidSceneChangePic.swap(m_vvuiEmpty);
  }

  if (m_vdAverageRate.size() > 0)
  {
    std::vector<double> m_vvdEmpty;
    m_vdAverageRate.swap(m_vvdEmpty);
  }
  if (m_vdAveragePSNRY.size() > 0)
  {
    std::vector<double> m_vvdEmpty;
    m_vdAveragePSNRY.swap(m_vvdEmpty);
  }
  if (m_vdAveragePSNRU.size() > 0)
  {
    std::vector<double> m_vvdEmpty;
    m_vdAveragePSNRU.swap(m_vvdEmpty);
  }
  if (m_vdAveragePSNRV.size() > 0)
  {
    std::vector<double> m_vvdEmpty;
    m_vdAveragePSNRV.swap(m_vvdEmpty);
  }
}

/************************************************************************/
/* File operation.
/************************************************************************/

void ComCBVCLogAnalyze::ReadMapFile()
{
  ifstream ifMapFile(m_pcParam->m_sNameOfVidLibMapFile);
  char buffer[2048];

  unsigned int uiPOCOfVidPic = 0;
  unsigned int uiPOCOfLibPic = 0;

  if (! ifMapFile.good())
  {
    cerr << "Parameter file of library extraction " << m_pcParam->m_sNameOfVidLibMapFile << " could not be open!\n" << endl;
    exit(1);
  }

  while (ifMapFile.good()) // Read until the end of file or the end of given line number.
  {
    ifMapFile.getline(buffer, 2048);

    if (strlen ( buffer ) < 2)
    {
      // ignore this line - nothing to do
    }
    else
    {
      std::vector<unsigned int> vuiMaskOfSigleVidPic;

      sscanf_s(buffer, "%d %d", &uiPOCOfVidPic, &uiPOCOfLibPic);

#ifdef DEBUG_OUTPUT
      cout << "Reading Map File : " << uiPOCOfVidPic << " " << uiPOCOfLibPic << endl;
#endif

      vuiMaskOfSigleVidPic.push_back(uiPOCOfVidPic);
      vuiMaskOfSigleVidPic.push_back(uiPOCOfLibPic);
      vuiMaskOfSigleVidPic.push_back(0);
      m_vvuiMaskOfVidPic.push_back(vuiMaskOfSigleVidPic);
    }	// endif
  }
  ifMapFile.close();
}

void ComCBVCLogAnalyze::ReadSceneChangePOCFile()
{
  ifstream ifSceneChangePOCFile(m_pcParam->m_sNameOfSceneChangePOCFile);

  unsigned int uiPOCOfVidSceneChangePic;
  std::vector<unsigned int> vuiTempSceneChangePic;

  if (! ifSceneChangePOCFile.good())
  {
    cerr << "Parameter file of library extraction " << m_pcParam->m_sNameOfSceneChangePOCFile << " could not be open!\n" << endl;
    exit(1);
  }

  while (ifSceneChangePOCFile.good()) // Read until the end of file or the end of given line number.
  {
    ifSceneChangePOCFile >> uiPOCOfVidSceneChangePic;
    vuiTempSceneChangePic.push_back(uiPOCOfVidSceneChangePic);
  }

  KeyPicExpand(vuiTempSceneChangePic);
  assert(m_vvuiMaskOfVidPic.empty() == false);
  
  for (unsigned int i = 0; i != vuiTempSceneChangePic.size(); ++i)
  {
    std::vector<unsigned int> vuiMapOfSigleVidPic;
    vuiMapOfSigleVidPic.push_back(m_vvuiMaskOfVidPic[vuiTempSceneChangePic[i]][0]);
    vuiMapOfSigleVidPic.push_back(m_vvuiMaskOfVidPic[vuiTempSceneChangePic[i]][1]);
    m_vvuiMapOfVidSceneChangePic.push_back(vuiMapOfSigleVidPic);

#ifdef DEBUG_OUTPUT
    cout << "Reading Scene Change POC File : " << vuiMapOfSigleVidPic[0] << " " << vuiMapOfSigleVidPic[1] << endl;
#endif
  }

  ifSceneChangePOCFile.close();
}

void ComCBVCLogAnalyze::WriteResult()
{
  ofstream ofResult(m_pcParam->m_sNameOfOutResultFile, std::ios::app);

	ofResult << m_cVidLog.m_sNameOfLogFile << endl;
  for (int i = 0; i != m_vdAverageRate.size(); ++i)
  {
    ofResult << m_vdAverageRate[i] << "\t" << m_vdAveragePSNRY[i] << "\t" << m_vdAveragePSNRU[i] << "\t" << m_vdAveragePSNRV[i] << endl;
  }

  ofResult.close();
}

/************************************************************************/
/* Value Processing.
/************************************************************************/

void ComCBVCLogAnalyze::GenerateMapOfVidLib()
{
  ReadMapFile();
  ReadSceneChangePOCFile();
}

void ComCBVCLogAnalyze::GetSceneAverageData()
{
  std::vector<unsigned int> vuiPOCOfLibPic;
  unsigned int uiTempPOC = 0;

  // Get Number of Library Pictures.
  for (int i = 0; i != m_vvuiMapOfVidSceneChangePic.size(); ++i)
  {
    vuiPOCOfLibPic.push_back(m_vvuiMapOfVidSceneChangePic[i][1]);
  }
  // erase the repeat ones.
  sort(vuiPOCOfLibPic.begin(), vuiPOCOfLibPic.end());
  std::vector<unsigned int>::iterator vuiPos = unique(vuiPOCOfLibPic.begin(), vuiPOCOfLibPic.end());
  vuiPOCOfLibPic.erase(vuiPos, vuiPOCOfLibPic.end());

  // For Each Library Picture, an average bit rate is computed.
  for (unsigned int j = 0; j != vuiPOCOfLibPic.size(); ++j)
  {
    unsigned int uiNumOfData = 0;
    double dAverageRate = 0;
    double dAveragePSNRY = 0;
    double dAveragePSNRU = 0;
    double dAveragePSNRV = 0;
    unsigned int uiPOCOfLibPic = vuiPOCOfLibPic[j];

    // Get bit rate of the library picture.
    m_cLibLog.InitParam(m_pcParam->m_sNameOfLibLogFile, m_pcParam->m_sNameOfOutResultFile, uiPOCOfLibPic + 1, uiPOCOfLibPic + 1, m_dFrameRate, m_eTypeOfCodec);
    m_cLibLog.InitMemory();
    m_cLibLog.ReadLogFile();
    m_cLibLog.CalcAverageData();
    dAverageRate += m_cLibLog.GetAverageRate();

    for (unsigned int i = 0; i != m_vvuiMapOfVidSceneChangePic.size(); ++i)
    {
      if (m_vvuiMapOfVidSceneChangePic[i][1] == uiPOCOfLibPic) // Check whether the library picture is the given one.
      {
        //unsigned int uiStartNumOfLogLine = m_vvuiMapOfVidSceneChangePic[i][0] + 1;
        unsigned int uiStartNumOfLogLine = m_vvuiMapOfVidSceneChangePic[i][0];
        unsigned int uiEndNumOfLogLine = uiStartNumOfLogLine;
        assert(uiEndNumOfLogLine <= m_uiNumOfVidPic);

        m_cVidLog.InitParam(m_pcParam->m_sNameOfVidLogFile, m_pcParam->m_sNameOfOutResultFile, uiStartNumOfLogLine, uiEndNumOfLogLine, m_dFrameRate, m_eTypeOfCodec);
        m_cVidLog.InitMemory();
        m_cVidLog.ReadSingleIntraLogFile();
        m_cVidLog.CalcAverageData();
        
        uiNumOfData += m_cVidLog.GetNumOfLogData();
        dAverageRate += m_cVidLog.GetAverageRate() * m_cVidLog.GetNumOfLogData();
        dAveragePSNRY += m_cVidLog.GetAveragePSNRY() * m_cVidLog.GetNumOfLogData();
        dAveragePSNRU += m_cVidLog.GetAveragePSNRU() * m_cVidLog.GetNumOfLogData();
        dAveragePSNRV += m_cVidLog.GetAveragePSNRV() * m_cVidLog.GetNumOfLogData();

        m_cVidLog.DestroyMemory();
      }
    }

    dAverageRate /= uiNumOfData;
    dAveragePSNRY /= uiNumOfData;
    dAveragePSNRU /= uiNumOfData;
    dAveragePSNRV /= uiNumOfData;

    m_vdAverageRate.push_back(dAverageRate);
    m_vdAveragePSNRY.push_back(dAveragePSNRY);
    m_vdAveragePSNRU.push_back(dAveragePSNRU);
    m_vdAveragePSNRV.push_back(dAveragePSNRV);

    m_cLibLog.DestroyMemory();
  }
}

Void ComCBVCLogAnalyze::KeyPicExpand(std::vector<unsigned int> & vuiSrcSceneChangePOC)
{
  std::vector<UInt> viKeyFrameIndex;
  UInt iTempKeyFrameIndex;
  UInt iNumScene = vuiSrcSceneChangePOC.size();
  UInt iMaxIntraPeriod = m_uiMaxIntraPeroid;
	UInt iMaxPicIndex = m_cVidLog.m_vdPOC.size() - 1;

  // Main loop for expandation.
  UInt iTempIntraPeriod;
  UInt iTempCountIntra;

  // expand the scene change frame except the last one.
  for (UInt i = 0; i != iNumScene - 1; ++i)
  {
    // add the scene change frame index.
    iTempKeyFrameIndex = vuiSrcSceneChangePOC[i];
    viKeyFrameIndex.push_back(iTempKeyFrameIndex);

    // add frame between two scene change frame based on max intra period.
    iTempIntraPeriod = vuiSrcSceneChangePOC[i + 1] - vuiSrcSceneChangePOC[i];
    iTempCountIntra = (UInt)ceil((Double)(iTempIntraPeriod / iMaxIntraPeriod));
    for (UInt j = 0; j != iTempCountIntra; ++j)
    {
      iTempKeyFrameIndex = vuiSrcSceneChangePOC[i] + (j + 1) * iMaxIntraPeriod;
      viKeyFrameIndex.push_back(iTempKeyFrameIndex);
    }
  }
  // expand the last scene change frame.
  iTempKeyFrameIndex = vuiSrcSceneChangePOC[iNumScene - 1];
  viKeyFrameIndex.push_back(iTempKeyFrameIndex);
  iTempIntraPeriod = iMaxPicIndex - vuiSrcSceneChangePOC[iNumScene - 1] + 1;
  iTempCountIntra = (iTempIntraPeriod % iMaxIntraPeriod) ? (UInt)ceil((Double)(iTempIntraPeriod / iMaxIntraPeriod)) : ((UInt)(iTempIntraPeriod / iMaxIntraPeriod) - 1);
  for (UInt j = 0; j != iTempCountIntra; ++j)
  {
    iTempKeyFrameIndex = vuiSrcSceneChangePOC[iNumScene - 1] + (j + 1) * iMaxIntraPeriod;
    viKeyFrameIndex.push_back(iTempKeyFrameIndex);
  }
  // erase the repeat ones.
  sort(viKeyFrameIndex.begin(), viKeyFrameIndex.end());
  std::vector<UInt>::iterator vuiPos = unique(viKeyFrameIndex.begin(), viKeyFrameIndex.end());
  viKeyFrameIndex.erase(vuiPos, viKeyFrameIndex.end());  

  // Assign the key frame index to variable KeyFrame.
  vuiSrcSceneChangePOC.swap(viKeyFrameIndex);
}

void ComCBVCLogAnalyze::runCalculateRandomAccess()
{
	m_cLibLog.InitParam(m_pcParam->m_sNameOfLibLogFile, m_pcParam->m_sNameOfOutResultFile, 0, 0, m_pcParam->m_dFrameRate, m_pcParam->m_eTypeOfCodec);
	m_cLibLog.ReadLogFile();
	m_cLibLog.runSort();

	m_cVidLog.InitParam(m_pcParam->m_sNameOfVidLogFile, m_pcParam->m_sNameOfOutResultFile, 0, 0, m_pcParam->m_dFrameRate, m_pcParam->m_eTypeOfCodec);
	m_cVidLog.ReadLogFile();
	m_cVidLog.runSort();

	GenerateMapOfVidLib();

	// calculate average bitrate and psnr for random access.
	int iNumIPic = (int)m_vvuiMapOfVidSceneChangePic.size();
	int iNumPic = (int)m_cVidLog.m_vdPOC.size();
	int iNumLibPic = (int)m_cLibLog.m_vdPOC.size();
	double dTempBit = 0;
	double dTempPSNRY = 0;
	double dTempPSNRU = 0;
	double dTempPSNRV = 0;
	int iCount = 0;
	bool bStoreLib = true;
	vector<int> viTempLibPoc;

	for (int idxEndIPic = 0; idxEndIPic < iNumIPic; idxEndIPic++)
	{
		dTempBit = 0;
		dTempPSNRY = 0;
		dTempPSNRU = 0;
		dTempPSNRV = 0;
		iCount = 0;
		// for each intra picture, calculate the average results untill this picture.
		for (int idxStartIPic = 0; idxStartIPic <= idxEndIPic; idxStartIPic++)
		{
			viTempLibPoc.resize(0);

			for (int idxPic = m_vvuiMapOfVidSceneChangePic[idxStartIPic][0]; idxPic < iNumPic; idxPic++)
			{
				bStoreLib = true;

				for (int idxTempIpic = idxStartIPic; idxTempIpic < iNumIPic; idxTempIpic++)
				{
					if (m_vvuiMapOfVidSceneChangePic[idxTempIpic][0] == m_cVidLog.m_vdPOC[idxPic])
					{
						for (int idxTempLibPic = 0; idxTempLibPic < (int)viTempLibPoc.size(); idxTempLibPic++)
						{
							if (viTempLibPoc[idxTempLibPic] == m_vvuiMapOfVidSceneChangePic[idxTempIpic][1])
							{
								bStoreLib = false;
								break;
							}
						}

						if (bStoreLib)
						{
							dTempBit += m_cLibLog.m_vdBit[m_vvuiMapOfVidSceneChangePic[idxTempIpic][1]];
							viTempLibPoc.push_back(m_vvuiMapOfVidSceneChangePic[idxTempIpic][1]);
							break;
						}
					}
				}

				dTempBit += m_cVidLog.m_vdBit[idxPic];
				dTempPSNRY += m_cVidLog.m_vdPSNRY[idxPic];
				dTempPSNRU += m_cVidLog.m_vdPSNRU[idxPic];
				dTempPSNRV += m_cVidLog.m_vdPSNRV[idxPic];
				iCount++;
			}
		}

		m_vdAverageRate.push_back(dTempBit / iCount * m_pcParam->m_dFrameRate / 1000);
		m_vdAveragePSNRY.push_back(dTempPSNRY / iCount);
		m_vdAveragePSNRU.push_back(dTempPSNRU / iCount);
		m_vdAveragePSNRV.push_back(dTempPSNRV / iCount);
	}
}

void ComCBVCLogAnalyze::runCalculateAnchorRandomAccess()
{
	m_vdAveragePSNRU.resize(0);
	m_vdAveragePSNRV.resize(0);
	m_vdAveragePSNRY.resize(0);
	m_vdAverageRate.resize(0);

	m_cAnchorVidLog.InitParam(m_pcParam->m_sNameOfAnchorVidFile, m_pcParam->m_sNameOfOutResultFile, 0, 0, m_pcParam->m_dFrameRate, m_pcParam->m_eTypeOfCodec);

	m_cAnchorVidLog.ReadLogFile();
	m_cAnchorVidLog.runSort();

	// calculate average bitrate and psnr for random access.
	int iNumIPic = (int)m_vvuiMapOfVidSceneChangePic.size();
	int iNumPic = (int)m_cAnchorVidLog.m_vdPOC.size();
	double dTempBit = 0;
	double dTempPSNRY = 0;
	double dTempPSNRU = 0;
	double dTempPSNRV = 0;
	int iCount = 0;

	for (int idxEndIPic = 0; idxEndIPic < iNumIPic; idxEndIPic++)
	{
		dTempBit = 0;
		dTempPSNRY = 0;
		dTempPSNRU = 0;
		dTempPSNRV = 0;
		iCount = 0;
		// for each intra picture, calculate the average results untill this picture.
		for (int idxStartIPic = 0; idxStartIPic <= idxEndIPic; idxStartIPic++)
		{
			for (int idxPic = m_vvuiMapOfVidSceneChangePic[idxStartIPic][0]; idxPic < iNumPic; idxPic++)
			{
				dTempBit += m_cAnchorVidLog.m_vdBit[idxPic];
				dTempPSNRY += m_cAnchorVidLog.m_vdPSNRY[idxPic];
				dTempPSNRU += m_cAnchorVidLog.m_vdPSNRU[idxPic];
				dTempPSNRV += m_cAnchorVidLog.m_vdPSNRV[idxPic];
				iCount++;
			}
		}

		m_vdAverageRate.push_back(dTempBit / iCount * m_pcParam->m_dFrameRate / 1000);
		m_vdAveragePSNRY.push_back(dTempPSNRY / iCount);
		m_vdAveragePSNRU.push_back(dTempPSNRU / iCount);
		m_vdAveragePSNRV.push_back(dTempPSNRV / iCount);
	}
}
