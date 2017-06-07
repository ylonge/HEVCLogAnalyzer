/*
 * Name: ComParam.cpp.
 * Function: Defination of parameters that should be ajusted mannually.
 * Author: Ylonge from MCL-ZJU, zxg from MCL-ZJU, Yin Zhao from Huawei.
 */

#include "ComParam.h"

#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

ComParam::ComParam()
{
}

ComParam::~ComParam()
{
}

void ComParam::InitMemory()
{

}
void ComParam::DestroyMemory()
{

}

void ComParam::InitParam()
{
  // Common file names.
  strcpy_s(m_chNameOfMapFile, MAX_NUM_CHAR_NAME, "LibrarySimilarityResult.txt");
  strcpy_s(m_chNameOfSceneChangePOCFile, MAX_NUM_CHAR_NAME, "ScenePOC.txt");
  strcpy_s(m_chNameOfOutResultFile, MAX_NUM_CHAR_NAME, "Result.txt");

  // Parameters of Video.
  m_uiNumOfVidPic = 0;
  m_dFrameRate = 25;
  m_eTypeOfCodec = CODEC_HM;
  m_uiMaxIntraPeroid = 32;
}

Void ComParam::ParseParam(Char cNameOfParamFile[])
{
  ifstream fParam(cNameOfParamFile);
  char buffer[2048];
  char paramName [1000];
  char paramValue [1000];

  if (! fParam.good())
  {
    cerr << "Parameter file of library extraction " << cNameOfParamFile << " could not be open!\n" << endl;
    exit(0);
  }

  int ret = 0;
  int val = 0;

  while (fParam.good())
  {
    fParam.getline(buffer, 2048);
    if (strlen ( buffer ) < 2)
    {
      // ignore this line - nothing to do
    }
    else if (buffer[0] != '#')			// allow comments
    {
      sscanf_s(buffer, "%s = %[^#\t\n]", paramName, sizeof(paramName), paramValue, sizeof(paramValue));
      ret = setParameter(paramName, paramValue);
      if (ret)
        break;
    }	// endif
  }
  fParam.close();
}

Int ComParam::setParameter(Char *paramName, Char *paramValue)
{
  return 0;
}