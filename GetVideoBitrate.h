#pragma once

struct FrameBitrate
{
	int64_t pts;
	int framesize;
	int duration;
	int keyframe;
};

struct StreamInfo
{
	CString strStreamName;
	AVMediaType codec_type;
	AVCodecID codec_id;
	int width;
	int height;
	int64_t duration;
	AVRational sample_aspect_ratio;
	double frame_rate;
	AVRational time_base;
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
	BOOL GetStreamInfo(UINT index, StreamInfo *pStreamInfo);
	const std::vector<FrameBitrate > &GetFrameList() const { return m_frameInfoList; };

public:
	LPCTSTR GetErrorMsg() const { return m_strErrorMsg; }

private:
	CString m_strErrorMsg;
	std::vector<FrameBitrate > m_frameInfoList;
	AVFormatContext *m_ifmt_ctx;
};

