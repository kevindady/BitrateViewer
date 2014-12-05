#pragma once

#define ERROR_MSG_SIZE	1024

struct FrameBitrate
{
	int64_t pts;
	int64_t dts;
	int framesize;
};

class CGetVideoBitrate
{
public:
	CGetVideoBitrate();
	~CGetVideoBitrate();

public:
	int GetVideoBitrate(const char *in_filename);
	
private:
	char m_strErrorMsg[ERROR_MSG_SIZE];
	std::vector<FrameBitrate > m_frameInfoList;
};

