#include "stdafx.h"

static inline CString my_av_make_error_string(int errnum)
{
	char errbuf[AV_ERROR_MAX_STRING_SIZE] = {0};
	CString strRet;
	av_strerror(errnum, errbuf, AV_ERROR_MAX_STRING_SIZE);
	strRet = errbuf;
	return strRet;
}

#define my_av_err2str(errnum) my_av_make_error_string(errnum)

CGetVideoBitrate::CGetVideoBitrate() :
m_ifmt_ctx(NULL)
{
}

CGetVideoBitrate::~CGetVideoBitrate()
{
	Close();
}


static void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt, int64_t firstpts)
{
	AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;
	double video_frame_rate = (double)(fmt_ctx->streams[pkt->stream_index]->codec->framerate.num) / fmt_ctx->streams[pkt->stream_index]->codec->framerate.den;
	char szbuf[200];
	sprintf_s(szbuf, 200, "pts:%I64d time=%I64d time2=%I64d duration:%d bitrate=%d keyframe=%d\r\n",
		pkt->pts,
		(LONGLONG)((av_q2d(*time_base) * pkt->pts) * 10000000L),
		(LONGLONG)((av_q2d(*time_base) * (pkt->pts - firstpts)) * 10000000L),
		pkt->duration,
		int(pkt->size * 8 * video_frame_rate), (pkt->flags & AV_PKT_FLAG_KEY) ? 1 : 0);
	OutputDebugStringA(szbuf);
}

BOOL CGetVideoBitrate::Open(LPCTSTR lpszFileName)
{
	int ret;
	USES_CONVERSION;
	if ((ret = avformat_open_input(&m_ifmt_ctx, W2A(lpszFileName), 0, 0)) < 0)
	{
		m_strErrorMsg.Format(_T("Could not open input file '%s'"), lpszFileName);
		return FALSE;
	}
	if ((ret = avformat_find_stream_info(m_ifmt_ctx, 0)) < 0)
	{
		m_strErrorMsg.Format(_T("Failed to retrieve input stream information"));
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

	BOOL bFindFirstPTS = FALSE;
	int64_t firstpts = -1;
	while (1)
	{
        AVStream *in_stream;

        ret = av_read_frame(m_ifmt_ctx, &pkt);
        if (ret < 0)
            break;

		in_stream = m_ifmt_ctx->streams[pkt.stream_index];

		if (pkt.stream_index == video_selected_stream_index)
		{
			if (!bFindFirstPTS)
			{
				firstpts = pkt.pts;
				bFindFirstPTS = TRUE;
			}
			log_packet(m_ifmt_ctx, &pkt, firstpts);
		}

        av_free_packet(&pkt);
    }

    if (ret < 0 && ret != AVERROR_EOF)
	{
		m_strErrorMsg.Format(_T("Error occurred: %s\n"), my_av_err2str(ret));
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

CString CGetVideoBitrate::GetStreamInfo(UINT index) const
{
	CString strRet;
	if (m_ifmt_ctx && index >= 0 && index < m_ifmt_ctx->nb_streams)
	{
		const AVCodecDescriptor *desc = avcodec_descriptor_get(m_ifmt_ctx->streams[index]->codec->codec_id);
		if (desc)
		{
			strRet = desc->long_name;
		}
	}
	return strRet;
}


