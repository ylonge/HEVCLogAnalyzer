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
 * Function name: main().
 * Function: main function of the entire project.
 */

int main(int argc, char *argv[])
{
	ComParam cParam;
	ComLogAnalyze cLogAnalyzer;
	
	cParam.init();
	if (!cParam.runParseParam(argc, argv))
	{
		std::cerr << "error in loading parameters.\n" << std::endl;
	}

	cLogAnalyzer.init(cParam);
	cLogAnalyzer.runReadSummary();

	return 0;
}