/*
 * File name: Test_Main.cpp.
 * Function:
 * Author: Ylonge from MCL-ZJU.
 * Date: 2015/9/20.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "ComClass/TypeDef.h"
#include "ComClass/ComParam.h"
#include "LogAnalyzerCplus/ComLogAnalyze.h"
#include "LogAnalyzerCplus/ComCBVCLogAnalyze.h"
#include "LogAnalyzerCplus/ComBDRate.h"
//#include "ComClass/ComPic.h"

/*
 * Function name: Usage();
 * Function: user usage to display.
 */
Void Usage()
{
  printf("\n");
  printf("-o		output result file name\n");
  printf("-n		number of video pictures\n");
  printf("-ip  	max intra peroid\n");
  printf("-t		type of log: HM_LOG\n");
  printf("-vid  input video log file name\n");
  printf("-lib	input lib log file name\n");
  printf("-map	input map file name\n");
  printf("-scn	input scene change poc file name\n");
  printf("-p    ParameterLibExtraction.txt");
  exit(0);
}

/*
 * Function name: InitialParameters().
 * Function: Initialization of parameters.
 * @param argc number of input arguments.
 * @param *argv[] string list of input arguments.
 * @param Param parameters after initialization.
 */
void InitialParameters(int argc, char* argv[], ComParam & cParam)
{
  // Usage is displayed while the first argument is "-help" or there is no argument or the argument do not come in pairs.

  if(argc <= 1 || strcmp("-help",argv[1]) == 0 || (argc - 1) % 2 != 0)
  {
    Usage();
  }

  // Assign parameters with given values.
  for(Int iArgIdx = 1; iArgIdx != argc; iArgIdx +=2)
  {
    
    if      (strcmp("-o",  argv[iArgIdx]) == 0)  strcpy_s(cParam.m_chNameOfOutResultFile,    argv[iArgIdx + 1]);
    else if (strcmp("-n",   argv[iArgIdx]) == 0)  cParam.m_uiNumOfVidPic     = atoi(argv[iArgIdx + 1]);
    else if (strcmp("-ip",   argv[iArgIdx]) == 0)  cParam.m_uiMaxIntraPeroid     = atoi(argv[iArgIdx + 1]);
    else if (strcmp("-vid", argv[iArgIdx]) == 0)  strcpy_s(cParam.m_chNameOfVidLogFile,    argv[iArgIdx + 1]);
    else if (strcmp("-lib", argv[iArgIdx]) == 0)  strcpy_s(cParam.m_chNameOfLibLogFile,    argv[iArgIdx + 1]);
    else if (strcmp("-map",   argv[iArgIdx]) == 0)  strcpy_s(cParam.m_chNameOfMapFile,    argv[iArgIdx + 1]);
    else if (strcmp("-scn",   argv[iArgIdx]) == 0)  strcpy_s(cParam.m_chNameOfSceneChangePOCFile,    argv[iArgIdx + 1]);
    else if (strcmp("-t",   argv[iArgIdx]) == 0)
    {
      if (strcmp(argv[iArgIdx + 1], "CODEC_HM") == 0)
      {
        cParam.m_eTypeOfCodec = CODEC_HM;
      }
      else
      {
        printf("The input command is not defined!");
        Usage();
        exit(0);
      }
    }
    else if (strcmp("-p",   argv[iArgIdx]) == 0)  cParam.ParseParam(argv[iArgIdx + 1]);
    else {printf("The input command is not defined!");  Usage();  exit(0);}
  }
}

/*
 * Function name: PrintParam().
 * Function: Print the given paramters on screen.
 */
void PrintParam(ComParam & cParam)
{
  printf("Encoding parameter confirm!\n");
  printf("Output Result         :           %s\n", cParam.m_chNameOfOutResultFile);
  printf("Video Log File        :           %s\n", cParam.m_chNameOfVidLogFile);
  printf("Library Log File      :           %s\n", cParam.m_chNameOfLibLogFile);
  printf("Map File              :           %s\n", cParam.m_chNameOfMapFile);
  printf("Scene Change POC File :           %s\n", cParam.m_chNameOfSceneChangePOCFile);
  printf("Frame Number          :           %d\n", cParam.m_uiNumOfVidPic);
  printf("Max Intra Period      :           %d\n", cParam.m_uiMaxIntraPeroid);
  printf("Frame Rate            :           %f\n", cParam.m_dFrameRate);
}

/*
 * Function name: main().
 * Function: main function of the entire project.
 */

int main(int argc, char *argv[])
{
  ///ComParam cParam;
  ComLogAnalyze cLogAnalyzer;

  //cParam.InitParam();
  //InitialParameters(argc, argv, cParam);
  //PrintParam(cParam);

  cLogAnalyzer.InitParam( argv[ 1 ], argv[ 2 ], atoi( argv[ 3 ] ) , atoi( argv[ 4 ] ), 24, CODEC_HM );
  cLogAnalyzer.InitMemory();
  cLogAnalyzer.ReadLogFile();
  cLogAnalyzer.WriteResult();

  cLogAnalyzer.DestroyMemory();
  //cParam.DestroyMemory();

  return 0;
}
//int main(int argc, char *argv)
//{
//  ComPic SinglePic;
//
//  unsigned int uiWidth = 352;
//  unsigned int uiHeight = 288;
//
//  unsigned char *pucCompY = NULL;
//  unsigned char *pucCompU = NULL;
//  unsigned char *pucCompV = NULL;
//
//  char cNameOfSrcYuv[100];
//  char cNameOfDesYuvColor[100];
//  
//  SinglePic.ParamInit( uiWidth, uiHeight );
//  SinglePic.Create();
//  pucCompY = SinglePic.GetAddrCompY();
//  pucCompU = SinglePic.GetAddrCompU();
//  pucCompV = SinglePic.GetAddrCompV();
//
//  sprintf_s( cNameOfDesYuvColor, "foreman_incomplete.yuv" );
//
//  FILE *fpDesYuvColor = NULL;
//  FILE *fpSrcYuv = NULL;
//
//  fopen_s( &fpDesYuvColor, cNameOfDesYuvColor, "wb" );
//
//  unsigned char *pucTempCompY;
//  unsigned char *pucTempCompU;
//  unsigned char *pucTempCompV;
//
//  for ( int i = 0; i != 250; ++i )
//  {
//    // Read yuv file.
//    sprintf_s( cNameOfSrcYuv, "foreman/foreman%03d.yuv", i );
//    printf_s("Reading %s.\n", cNameOfSrcYuv);
//    fopen_s( &fpSrcYuv, cNameOfSrcYuv, "rb" );
//
//    fread_s( pucCompY, uiWidth * uiHeight * sizeof( unsigned char ), sizeof( unsigned char ), uiWidth * uiHeight, fpSrcYuv );
//    fread_s( pucCompU, uiWidth * uiHeight * sizeof( unsigned char ), sizeof( unsigned char ), uiWidth * uiHeight >> 2, fpSrcYuv );
//    fread_s( pucCompV, uiWidth * uiHeight * sizeof( unsigned char ), sizeof( unsigned char ), uiWidth * uiHeight >> 2, fpSrcYuv );
//
//    if ( i % 20 == 10)
//    {
//      unsigned int j = ( unsigned int )( i / 20 );
//      unsigned int uiPosX = j * 8;
//      unsigned int uiPosY = j * 8;
//      unsigned int uiBlockWidth = j * 4;
//
//      assert( uiPosX <= 288 || uiPosY <= 224 );
//
//      for (unsigned int k = uiPosY; k != uiPosY + uiBlockWidth; ++k)
//      {
//        pucTempCompY = pucCompY + k * uiWidth + uiPosX;
//        pucTempCompU = pucCompU + ( ( k >> 1 ) * ( uiWidth >> 1 ) ) + ( uiPosX >> 1);
//        pucTempCompV = pucCompV + ( ( k >> 1 ) * ( uiWidth >> 1 ) ) + ( uiPosX >> 1);
//
//        memset( pucTempCompY, 255, uiBlockWidth * sizeof( unsigned char ) );
//        memset( pucTempCompU, 128, ( uiBlockWidth >> 1 ) * sizeof( unsigned char ) );
//        memset( pucTempCompV, 128, ( uiBlockWidth >> 1 ) * sizeof( unsigned char ) );
//      }
//    }
//
//    // Write all single yuv to one yuv sequence.
//    fwrite( pucCompY, sizeof( unsigned char ), uiWidth * uiHeight, fpDesYuvColor );
//    fwrite( pucCompU, sizeof( unsigned char ), uiWidth * uiHeight >> 2, fpDesYuvColor );
//    fwrite( pucCompV, sizeof( unsigned char ), uiWidth * uiHeight >> 2, fpDesYuvColor );
//
//    fclose(fpSrcYuv);
//  }
//
//  _fcloseall();
//
//  system("pause");
//  return 0;
//}