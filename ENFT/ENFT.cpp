////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2017 Zhejiang University
//  All Rights Reserved
//
//  For more information see <https://github.com/ZJUCVG/ENFT>
//  If you use this code, please cite the corresponding publications as 
//  listed on the above website.
//
//  Permission to use, copy, modify and distribute this software and its
//  documentation for educational, research and non-profit purposes only.
//  Any modification based on this work must be open source and prohibited
//  for commercial use.
//  You must retain, in the source form of any derivative works that you 
//  distribute, all copyright, patent, trademark, and attribution notices 
//  from the source form of this work.
//   
//
////////////////////////////////////////////////////////////////////////////
// ENFT.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FeatureTracker.h"
#include "TrackMatcher.h"
#include "ViewerSequenceSet.h"

typedef struct Video
{
	std::string imgFileName;
	int start, step, end;
} Video;

void RunENFT(const std::vector<Video> &videos, const char *paramDir, const char *outputFileName, const bool view = false)
{
	SequenceSet Vs;
	Vs.SetDirectory("");
	Vs.SetIntrinsicType(Sequence::INTRINSIC_CONSTANT);

	const int nVideos = int(videos.size());
	Vs.CreateSequences(SequenceIndex(nVideos));

	FeatureTracker ftrTracker;
	TrackMatcher trkMatcher1, trkMatcher2;
	for(int iVideo = 0; iVideo < nVideos; ++iVideo)
	{
		const Video &video = videos[iVideo];
		const std::string dir = IO::ExtractFileDirectory(video.imgFileName);
		const std::string imgFileName = IO::RemoveFileDirectory(video.imgFileName);
		const int iStart = video.start, iStep = video.step, iEnd = video.end;
		Sequence &V = Vs[iVideo];
		V.SetTag(dir, imgFileName, iStart, iStep, iEnd);
		V.SetIntrinsicType(Sequence::INTRINSIC_CONSTANT);
		V.LoadCalibrationFile(NULL);

		if(iVideo == 0)
		{
			ftrTracker.Initialize(V, std::string(paramDir) + "param_ftr_tracking.txt");
			trkMatcher1.Initialize(V, std::string(paramDir) + "param_trk_matching.txt");
		}

		ftrTracker.Run(V);
		trkMatcher1.Run(V);
	}

	if(nVideos > 1)
	{
		int iVideo1, iVideo2;
		SequenceIndexPairList iVideoPairs;
		for(iVideo1 = 0; iVideo1 < nVideos; ++iVideo1)
		for(iVideo2 = iVideo1 + 1; iVideo2 < nVideos; ++iVideo2)
			iVideoPairs.push_back(SequenceIndexPair(SequenceIndex(iVideo1), SequenceIndex(iVideo2)));
		trkMatcher2.Initialize(Vs, std::string(paramDir) + "param_trk_matching.txt");
		trkMatcher2.Run(Vs, iVideoPairs);
	}

	Vs.SaveTracks(outputFileName);

	if(view)
	{
		if(nVideos > 1)
		{
			ViewerSequenceSet viewer;
			viewer.Run(Vs);
		}
		else
		{
			ViewerSequence viewer;
			viewer.Run(Vs[0]);
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc == 2)
	{
		Configurator cfgor;
		cfgor.Load(argv[1]);
		
		char directive[MAX_LINE_LENGTH];
		std::vector<Video> videos;
		const int nVideos = cfgor.GetArgument("videos", 0);
		videos.resize(nVideos);
		for(int i = 0; i < nVideos; ++i)
		{
			Video &video = videos[i];
			sprintf(directive, "video_%d_image", i);
			video.imgFileName = cfgor.GetArgument(directive);
			sprintf(directive, "video_%d_start", i);
			video.start = cfgor.GetArgument(directive, 0);
			sprintf(directive, "video_%d_step", i);
			video.step = cfgor.GetArgument(directive, 0);
			sprintf(directive, "video_%d_end", i);
			video.end = cfgor.GetArgument(directive, 0);
			RunENFT(videos, cfgor.GetArgument("param_directory").c_str(), cfgor.GetArgument("output_file").c_str());
		}
	}
	else
	{
		std::vector<Video> videos(2);
		videos[0].imgFileName = "../data/0/0000.jpg";
		videos[0].start = 10;
		videos[0].step = 2;
		videos[0].end = 150;
		videos[1].imgFileName = "../data/1/0000.jpg";
		videos[1].start = 0;
		videos[1].step = 2;
		videos[1].end = 100;
		RunENFT(videos, "./param/", "../data/result.txt");
	}
	return 0;
}

