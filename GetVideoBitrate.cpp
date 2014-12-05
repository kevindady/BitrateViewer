#include "stdafx.h"

static inline TCHAR *my_av_ts_make_string(int64_t ts)
{
	static TCHAR buf[AV_TS_MAX_STRING_SIZE] = {0};
	if (ts == AV_NOPTS_VALUE) _sntprintf_s(buf, AV_TS_MAX_STRING_SIZE, 1024, _T("NOPTS"));
	else                      _sntprintf_s(buf, AV_TS_MAX_STRING_SIZE, 1024, _T("%I64d"), ts);
	return buf;
}

static inline TCHAR *my_av_ts_make_time_string(int64_t ts, AVRational *tb)
{
	static TCHAR buf[AV_TS_MAX_STRING_SIZE] = {0};
	if (ts == AV_NOPTS_VALUE) _sntprintf_s(buf, AV_TS_MAX_STRING_SIZE, 1024, _T("NOPTS"));
	else                      _sntprintf_s(buf, AV_TS_MAX_STRING_SIZE, 1024, _T("%.6g"), av_q2d(*tb) * ts);
	return buf;
}

static inline TCHAR *my_av_make_error_string(size_t errbuf_size, int errnum)
{
	char errbuf[AV_ERROR_MAX_STRING_SIZE] = {0};
	static TCHAR errbufW[AV_ERROR_MAX_STRING_SIZE] = {0};
	av_strerror(errnum, errbuf, errbuf_size);
	USES_CONVERSION;
	lstrcpy(errbufW, A2W(errbuf));
	return errbufW;
}

#define my_av_ts2str(ts) my_av_ts_make_string(ts)
#define my_av_ts2timestr(ts, tb) my_av_ts_make_time_string(ts, tb)
#define my_av_err2str(errnum) my_av_make_error_string(AV_ERROR_MAX_STRING_SIZE, errnum)

CGetVideoBitrate::CGetVideoBitrate() :
m_ifmt_ctx(NULL)
{
	memset(m_strErrorMsg, 0, sizeof(m_strErrorMsg));
}

CGetVideoBitrate::~CGetVideoBitrate()
{
	Close();
}


static void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt, const char *tag)
{
	AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;
	double video_frame_rate = (double)(fmt_ctx->streams[pkt->stream_index]->codec->framerate.num) / fmt_ctx->streams[pkt->stream_index]->codec->framerate.den;
	char szbuf[200];
	sprintf_s(szbuf, 200, "pts:%s pts_time:%s duration:%s duration_time:%s bitrate=%d keyframe=%d\r\n",
		my_av_ts2str(pkt->pts), my_av_ts2timestr(pkt->pts, time_base),
		my_av_ts2str(pkt->duration), my_av_ts2timestr(pkt->duration, time_base),
		int(pkt->size * 8 * video_frame_rate), (pkt->flags & AV_PKT_FLAG_KEY) ? 1 : 0); // 30 frame rate
	OutputDebugStringA(szbuf);
}

BOOL CGetVideoBitrate::Open(LPCTSTR lpszFileName)
{
	int ret;
	USES_CONVERSION;
	if ((ret = avformat_open_input(&m_ifmt_ctx, W2A(lpszFileName), 0, 0)) < 0)
	{
		_stprintf_s(m_strErrorMsg, ERROR_MSG_SIZE, _T("Could not open input file '%s'"), lpszFileName);
		return FALSE;
	}
	if ((ret = avformat_find_stream_info(m_ifmt_ctx, 0)) < 0)
	{
		_stprintf_s(m_strErrorMsg, ERROR_MSG_SIZE, _T("Failed to retrieve input stream information"));
		return FALSE;
	}
	return TRUE;
}

BOOL CGetVideoBitrate::Close()
{
	if (m_ifmt_ctx)
	{
		avformat_close_input(&m_ifmt_ctx);
		m_ifmt_ctx = NULL;
	}
	m_frameInfoList.clear();
	return TRUE;
}

BOOL CGetVideoBitrate::Parse(UINT video_selected_stream_index)
{
    AVPacket pkt;
	int ret;

	while (1)
	{
        AVStream *in_stream;

        ret = av_read_frame(m_ifmt_ctx, &pkt);
        if (ret < 0)
            break;

		in_stream = m_ifmt_ctx->streams[pkt.stream_index];

		if (pkt.stream_index == video_selected_stream_index)
		{
			log_packet(m_ifmt_ctx, &pkt, "in");
		}

        av_free_packet(&pkt);
    }

    if (ret < 0 && ret != AVERROR_EOF)
	{
		_stprintf_s(m_strErrorMsg, ERROR_MSG_SIZE, _T("Error occurred: %s\n"), my_av_err2str(ret));
        return FALSE;
    }

    return TRUE;
}

UINT CGetVideoBitrate::GetStreamCount() const
{
	if (m_ifmt_ctx)
	{
		return m_ifmt_ctx->nb_streams;
	}
	return 0;
}

