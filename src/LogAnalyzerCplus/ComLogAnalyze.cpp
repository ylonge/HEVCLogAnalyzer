/*
 * File name: ComLogAnalyze.c.
 * Function:
 * Author: Ylonge from MCL-ZJU.
 * Date: 2015/11/17.
 */

#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <assert.h>

#include "ComLogAnalyze.h"

using namespace std;

ComLogAnalyze::ComLogAnalyze()
{
}
ComLogAnalyze::~ComLogAnalyze()
{
}

/************************************************************************/
/* Initialization
/************************************************************************/

void ComLogAnalyze::init(ComParam & cParam)
{
	m_pcParam = &cParam;
}

void ComLogAnalyze::InitParam(string sNameOfLogFile, string sNameOfOutFile, int iStartNum, int iEndNum, double dFrameRate, EnumTypeOfCodec eTypeOfCodec)
{
	m_sNameOfLogFile = sNameOfLogFile;
	m_sNameOfResultFile = sNameOfOutFile;
  m_uiStartNumOfLogLine = iStartNum;
  m_uiEndNumOfLogLine = iEndNum;
  m_uiNumOfLogLine = m_uiEndNumOfLogLine - m_uiStartNumOfLogLine + 1;
  m_dFrameRate = dFrameRate;
  m_eTypeOfCodec = eTypeOfCodec;

  // Default value set.

  m_dAverageRate = 0;
  m_dAveragePSNRY = 0;
  m_dAveragePSNRU = 0;
  m_dAveragePSNRV = 0;
}

void ComLogAnalyze::InitMemory()
{
}

void ComLogAnalyze::DestroyMemory()
{
  if (m_vdBit.size() > 0)
  {
    std::vector<double> m_vvdEmpty;
    m_vdBit.swap(m_vvdEmpty);
  }
  if (m_vdPSNRY.size() > 0)
  {
    std::vector<double> m_vvdEmpty;
    m_vdPSNRY.swap(m_vvdEmpty);
  }
  if (m_vdPSNRU.size() > 0)
  {
    std::vector<double> m_vvdEmpty;
    m_vdPSNRU.swap(m_vvdEmpty);
  }
  if (m_vdPSNRV.size() > 0)
  {
    std::vector<double> m_vvdEmpty;
    m_vdPSNRV.swap(m_vvdEmpty);
  }
}

/************************************************************************/
/* File operation.
/************************************************************************/

void ComLogAnalyze::ReadLogFile()
{
  ifstream ifLogFile(m_sNameOfLogFile);
  char buffer[2048];
  char chTemp0[1000], chTemp1[1000], chTemp2[1000], chTemp3[1000], chTemp4[1000];

  unsigned int uiCountOfLogLine = 0;
  unsigned int uiCountOfData = 0;

  double dPOC = -1, dBit = 0, dPSNRY = 0, dPSNRU = 0, dPSNRV = 0;

  if (! ifLogFile.good())
  {
    cerr << "Parameter file of library extraction " << m_sNameOfLogFile << " could not be open!\n" << endl;
    exit(1);
  }

  while (ifLogFile.good() || (m_uiEndNumOfLogLine != 0 && uiCountOfLogLine < m_uiEndNumOfLogLine)) // Read until the end of file or the end of given line number.
  {
    ifLogFile.getline(buffer, 2048);

    if (strlen ( buffer ) < 2)
    {
      // ignore this line - nothing to do
    }
    else
    {
      switch (m_eTypeOfCodec) // Switch between different types of codecs.
      {
      case CODEC_HM : // Log file of HM.
        {
          if (buffer[0] == 'P' && buffer[1] == 'O' && buffer[2] == 'C') // Check whether the line is the data line. 
          {
            ++uiCountOfLogLine;

            if (uiCountOfLogLine < m_uiStartNumOfLogLine) // Check whether the start line is reached.
            {              
              continue;
            }
            else
            {
              sscanf(buffer, "%*[^C]C%s%*[^)])%s%*[^Y]Y%s%*[^U]U%s%*[^V]V%s%*[^\t\n]", chTemp0, chTemp1, chTemp2, chTemp3, chTemp4);
							dPOC = atof(chTemp0);
              dBit = atof(chTemp1);
              dPSNRY = atof(chTemp2);
              dPSNRU = atof(chTemp3);
              dPSNRV = atof(chTemp4);

              //assert(uiCountOfData < m_uiNumOfLogLine);
							m_vdPOC.push_back(dPOC);
              m_vdBit.push_back(dBit);
              m_vdPSNRY.push_back(dPSNRY);
              m_vdPSNRU.push_back(dPSNRU);
              m_vdPSNRV.push_back(dPSNRV);

              ++uiCountOfData;

#ifdef DEBUG_OUTPUT
              cout << "Reading Log Line : " << uiCountOfLogLine << " " << dBit << " " << dPSNRY << " " << dPSNRU << " " << dPSNRV << endl;
#endif
            }
          }
        }

      //default:
       // {cerr << "Type of Codec is not given!" << endl; exit(1);}
      } // end switch
    }	// endif
  }
  ifLogFile.close();
}

void ComLogAnalyze::ReadSingleIntraLogFile()
{
  ifstream ifLogFile(m_sNameOfLogFile);
  char buffer[2048];
  char chTemp1[1000], chTemp2[1000], chTemp3[1000], chTemp4[1000], chTemp5[1000];

  unsigned int uiCountOfData = 0;

  double dBit = 0, dPSNRY = 0, dPSNRU = 0, dPSNRV = 0;
  unsigned int uiPOC = 0;
  bool bMask = false;

  if (! ifLogFile.good())
  {
    cerr << "Parameter file of library extraction " << m_sNameOfLogFile << " could not be open!\n" << endl;
    exit(1);
  }

  while (ifLogFile.good()) // Read until the end of file or the end of given line number.
  {
    ifLogFile.getline(buffer, 2048);

    if (strlen ( buffer ) < 2)
    {
      // ignore this line - nothing to do
    }
    else
    {
      switch (m_eTypeOfCodec) // Switch between different types of codecs.
      {
      case CODEC_HM : // Log file of HM.
        {
          if (buffer[0] == 'P' && buffer[1] == 'O' && buffer[2] == 'C') // Check whether the line is the data line. 
          {
            sscanf(buffer, "POC %s%*[^\t\n]", chTemp1);
            uiPOC = atoi(chTemp1);

            if (!bMask && uiPOC != m_uiStartNumOfLogLine) // Check whether the start line is reached.
            {              
              continue;
            }
            else
            {
              sscanf(buffer, "%*[^(](%s%*[^)])%s%*[^Y]Y%s%*[^U]U%s%*[^V]V%s%*[^\t\n]", chTemp1, chTemp2, chTemp3, chTemp4, chTemp5);

              if (bMask && strcmp(chTemp1, "I-SLICE,") == 0) //Read until the next i-slice. The first break.
              {
                break;
              }

              dBit = atof(chTemp2);
              dPSNRY = atof(chTemp3);
              dPSNRU = atof(chTemp4);
              dPSNRV = atof(chTemp5);

              m_vdBit.push_back(dBit);
              m_vdPSNRY.push_back(dPSNRY);
              m_vdPSNRU.push_back(dPSNRU);
              m_vdPSNRV.push_back(dPSNRV);

              ++uiCountOfData;

#ifdef DEBUG_OUTPUT
              cout << "Reading Log : " << uiCountOfData << " " << chTemp1 << " " << dBit << " " << dPSNRY << " " << dPSNRU << " " << dPSNRV << endl;
#endif
            }
          }
        }

        //default:
        // {cerr << "Type of Codec is not given!" << endl; exit(1);}
      } // end switch

      if (bMask && strcmp(chTemp1, "I-SLICE,") == 0) // Read until the next i-slice. The second break.
      {
        break;
      }

      if(uiCountOfData > 0) // Mask is set to true only when the first I-slice has been read.
      {
        bMask = true;
      }      
    }	// endif    
  }
  m_uiNumOfLogLine = uiCountOfData;

  ifLogFile.close();
}

void ComLogAnalyze::WriteResult()
{
	ofstream ofFileOfResult(m_sNameOfResultFile);

	ofFileOfResult << "POC\tBits\tY-PSNR\tU-PSNR\tV-PSNR\t" << endl;
	for ( int i = 0; i != m_uiNumOfLogLine; ++i )
	{
		ofFileOfResult << m_vdPOC[i] << "\t" << m_vdBit[ i ] << "\t" << m_vdPSNRY[ i ] << "\t" << m_vdPSNRU[ i ] << "\t" << m_vdPSNRV[ i ] << "\t\n";
	}
}

void ComLogAnalyze::runReadSummary()
{
	ifstream ifLogFile(m_pcParam->m_sNameOfLibLogFile);
	ofstream ofResFile(m_pcParam->m_sNameOfOutResultFile, ios::app);

	char buffer[2048];
	char chTemp0[1000], chTemp1[1000], chTemp2[1000], chTemp3[1000], chTemp4[1000], chTemp5[1000];

	int iNumOfPic = 0;
	double dBit = 0, dPSNRY = 0, dPSNRU = 0, dPSNRV = 0, dPSNRYUV = 0;

	ofResFile << m_pcParam->m_sNameOfLibLogFile;

	if (!ifLogFile.good())
	{
		cerr << "Parameter file of library extraction " << m_sNameOfLogFile << " could not be open!\n" << endl;
		exit(1);
	}

	while (ifLogFile.good()) // Read until the end of file or the end of given line number.
	{
		ifLogFile.getline(buffer, 2048);

		if (strlen(buffer) < 2)
		{
			// ignore this line - nothing to do
		}
		else
		{
			switch (m_pcParam->m_eTypeOfCodec) // Switch between different types of codecs.
			{
			case CODEC_HM: // Log file of HM.
				{
					if (buffer[0] == 'S' && buffer[1] == 'U' && buffer[2] == 'M') // Check whether the line is the data line. 
					{
						ifLogFile.getline(buffer, 2048); //skip the second line after summary.
						ifLogFile.getline(buffer, 2048);
						sscanf(buffer, "%s %*s %s %s %s %s %s %*[^\t\n]", chTemp0, chTemp1, chTemp2, chTemp3, chTemp4, chTemp5);
						iNumOfPic = atoi(chTemp0);
						dBit = atof(chTemp1);
						dPSNRY = atof(chTemp2);
						dPSNRU = atof(chTemp3);
						dPSNRV = atof(chTemp4);
						dPSNRYUV = atof(chTemp5);
						ofResFile << "\tsummary\t" << iNumOfPic << "\t" << dBit << "\t" << dPSNRY << "\t" << dPSNRU << "\t" << dPSNRV << "\t" << dPSNRYUV;
					}
					else if (buffer[0] == 'I' && buffer[1] == ' ' && buffer[2] == 'S')
					{
						ifLogFile.getline(buffer, 2048); //skip the second line after summary.
						ifLogFile.getline(buffer, 2048);
						sscanf(buffer, "%s %*s %s %s %s %s %s %*[^\t\n]", chTemp0, chTemp1, chTemp2, chTemp3, chTemp4, chTemp5);
						iNumOfPic = atoi(chTemp0);
						dBit = atof(chTemp1);
						dPSNRY = atof(chTemp2);
						dPSNRU = atof(chTemp3);
						dPSNRV = atof(chTemp4);
						dPSNRYUV = atof(chTemp5);
						ofResFile << "\tI-slice\t" << iNumOfPic << "\t" << dBit << "\t" << dPSNRY << "\t" << dPSNRU << "\t" << dPSNRV << "\t" << dPSNRYUV;
					}
					else if (buffer[0] == 'P' && buffer[1] == ' ' && buffer[2] == 'S')
					{
						ifLogFile.getline(buffer, 2048); //skip the second line after summary.
						ifLogFile.getline(buffer, 2048);
						sscanf(buffer, "%s %*s %s %s %s %s %s %*[^\t\n]", chTemp0, chTemp1, chTemp2, chTemp3, chTemp4, chTemp5);
						iNumOfPic = atoi(chTemp0);
						dBit = atof(chTemp1);
						dPSNRY = atof(chTemp2);
						dPSNRU = atof(chTemp3);
						dPSNRV = atof(chTemp4);
						dPSNRYUV = atof(chTemp5);
						ofResFile << "\tP-slice\t" << iNumOfPic << "\t" << dBit << "\t" << dPSNRY << "\t" << dPSNRU << "\t" << dPSNRV << "\t" << dPSNRYUV;
					}
					else if (buffer[0] == 'B' && buffer[1] == ' ' && buffer[2] == 'S')
					{
						ifLogFile.getline(buffer, 2048); //skip the second line after summary.
						ifLogFile.getline(buffer, 2048);
						sscanf(buffer, "%s %*s %s %s %s %s %s %*[^\t\n]", chTemp0, chTemp1, chTemp2, chTemp3, chTemp4, chTemp5);
						iNumOfPic = atoi(chTemp0);
						dBit = atof(chTemp1);
						dPSNRY = atof(chTemp2);
						dPSNRU = atof(chTemp3);
						dPSNRV = atof(chTemp4);
						dPSNRYUV = atof(chTemp5);
						ofResFile << "\tB-slice\t" << iNumOfPic << "\t" << dBit << "\t" << dPSNRY << "\t" << dPSNRU << "\t" << dPSNRV << "\t" << dPSNRYUV;
					}
					else if (buffer[0] == 'L' && buffer[1] == 'i' && buffer[2] == 'b')
					{
						ifLogFile.getline(buffer, 2048); //skip the second line after summary.
						ifLogFile.getline(buffer, 2048);
						sscanf(buffer, "%s %*s %s %s %s %s %s %*[^\t\n]", chTemp0, chTemp1, chTemp2, chTemp3, chTemp4, chTemp5);
						iNumOfPic = atoi(chTemp0);
						dBit = atof(chTemp1);
						dPSNRY = atof(chTemp2);
						dPSNRU = atof(chTemp3);
						dPSNRV = atof(chTemp4);
						dPSNRYUV = atof(chTemp5);
						ofResFile << "\tLib-slice\t" << iNumOfPic << "\t" << dBit << "\t" << dPSNRY << "\t" << dPSNRU << "\t" << dPSNRV << "\t" << dPSNRYUV;
					}
				}
			} // end switch
		}	// endif
	}
	ofResFile << "\n";

	ifLogFile.close();
	ofResFile.close();
}

/************************************************************************/
/* Value Processing.
/************************************************************************/

void ComLogAnalyze::CalcAverageData()
{
  for (int i = 0; i != m_uiNumOfLogLine; ++i)
  {
    m_dAverageRate += m_vdBit[i];
    m_dAveragePSNRY += m_vdPSNRY[i];
    m_dAveragePSNRU += m_vdPSNRU[i];
    m_dAveragePSNRV += m_vdPSNRV[i];
  }
  
  m_dAverageRate = m_dAverageRate / (1000 * m_uiNumOfLogLine) * m_dFrameRate;
  m_dAveragePSNRY /= m_uiNumOfLogLine;
  m_dAveragePSNRU /= m_uiNumOfLogLine;
  m_dAveragePSNRV /= m_uiNumOfLogLine;
}

void ComLogAnalyze::runSort()
{
	int iNumPic = (int)m_vdPOC.size();
	double dTemp;

	for (int i = iNumPic - 1; i > 0; i--)
	{
		for (int j = 0; j < i - 1; j++)
		{
			if (m_vdPOC[j] > m_vdPOC[j + 1])
			{
				dTemp = m_vdPOC[j];
				m_vdPOC[j] = m_vdPOC[j + 1];
				m_vdPOC[j + 1] = dTemp;

				dTemp = m_vdBit[j];
				m_vdBit[j] = m_vdBit[j + 1];
				m_vdBit[j + 1] = dTemp;

				dTemp = m_vdPSNRY[j];
				m_vdPSNRY[j] = m_vdPSNRY[j + 1];
				m_vdPSNRY[j + 1] = dTemp;

				dTemp = m_vdPSNRU[j];
				m_vdPSNRU[j] = m_vdPSNRU[j + 1];
				m_vdPSNRU[j + 1] = dTemp;

				dTemp = m_vdPSNRV[j];
				m_vdPSNRV[j] = m_vdPSNRV[j + 1];
				m_vdPSNRV[j + 1] = dTemp;
			}
		}
	}
}