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

void ComParam::init()
{	
}

void ComParam::destroy()
{
}

namespace po = df::program_options_lite;
Bool ComParam::runParseParam(int argc, char * argv[])
{
	bool do_help = false;
	int warnUnknowParameter = 0;
	int iTypeOfCodec, iTypeOfResult;

	po::Options opts;
	opts.addOptions()
		("help", do_help, false, "this help text")
		("c", po::parseConfigFile, "configuration file name")
		("WarnUnknowParameter,w", warnUnknowParameter, 0, "warn for unknown configuration parameters instead of failing")

		("LogFileOfLibPic", m_sNameOfLibLogFile, (string)"", "Name of log file recording the library picture encoding bitrate and PSNR")
		("LogFileOfVidPic", m_sNameOfVidLogFile, (string)"", "Name of log file recording the video picture encoding bitrate and PSNR")
		("LogFileOfVidLibMap", m_sNameOfVidLibMapFile, (string)"", "Name of log file recording the connection between video and library picture")
		("LogFileOfAnchorVid", m_sNameOfAnchorVidFile, (string)"", "Name of log file recording the anchor video picutre encoding bitrate and PSNR")
		("LogFileOfSCD", m_sNameOfSceneChangePOCFile, (string)"", "Name of log file recording the scene change poc in video")
		("ResultFile", m_sNameOfOutResultFile, (string)"", "Name of result file")
		("TypeOfCodec", iTypeOfCodec, int(NUM_CODEC), "Type of codec, 0-HM, 1-LibVC")
		("TypeOfResult", iTypeOfResult, int(NUM_RESULT), "Type of results, 0-total, 1-part, 2-random access")
		("FrameRate", m_dFrameRate, 0.0, "Frame rate")
		("MaxIntraPeriod", m_uiMaxIntraPeroid, (unsigned int)0, "Maximum intra period without scene change")
		;

	po::setDefaults(opts);
	po::ErrorReporter err;
	const list<const char*>& argv_unhandled = po::scanArgv(opts, argc, (const char**)argv, err);

	for (list<const char*>::const_iterator it = argv_unhandled.begin(); it != argv_unhandled.end(); it++)
	{
		fprintf(stderr, "Unhandled argument ignored: `%s'\n", *it);
	}

	if (argc == 1 || do_help)
	{
		/* argc == 1: no options have been specified */
		po::doHelp(cout, opts);
		return false;
	}

	if (err.is_errored)
	{
		if (!warnUnknowParameter)
		{
			/* error report has already been printed on stderr */
			return false;
		}
	}

	m_eTypeOfCodec = (EnumTypeOfCodec)iTypeOfCodec;
	m_eTypeOfResult = (EnumTypeOfResult)iTypeOfResult;

	return true;
}