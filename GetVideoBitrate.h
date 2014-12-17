#pragma once

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
	BOOL Open(LPCTSTR lpszFileName);
	BOOL Parse(UINT video_selected_stream_index);
	BOOL Close();

public:
	UINT GetStreamCount() const;
	CString GetStreamInfo(UINT index) const;

public:
	LPCTSTR GetErrorMsg() const { return m_strErrorMsg; }

private:
	CString m_strErrorMsg;
	std::vector<FrameBitrate > m_frameInfoList;
	AVFormatContext *m_ifmt_ctx;
};

