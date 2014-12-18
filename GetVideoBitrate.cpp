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

	StreamInfo streamInfo;
	GetStreamInfo(video_selected_stream_index, &streamInfo);
	LONG frameNumber = (LONG)(streamInfo.duration * streamInfo.frame_rate / 10000000L);
	m_frameInfoList.clear();
	m_frameInfoList.reserve(frameNumber);
	while (1)
	{
        AVStream *in_stream;

        ret = av_read_frame(m_ifmt_ctx, &pkt);
        if (ret < 0)
            break;

		in_stream = m_ifmt_ctx->streams[pkt.stream_index];

		if (pkt.stream_index == video_selected_stream_index)
		{
// 			int len, got_frame;
// 			AVCodec *dec = NULL;
// 			AVDictionary *opts = NULL;
// 			AVCodecContext *avctx;
// 			AVFrame *aframe;
// 			aframe = av_frame_alloc();
// 
// 			dec = avcodec_find_decoder(m_ifmt_ctx->streams[video_selected_stream_index]->codec->codec_id);
// 			if (avcodec_open2(m_ifmt_ctx->streams[video_selected_stream_index]->codec, dec, &opts) >= 0)
// 			{
// 				int wyg = 0;
// 			}
// 			len = avcodec_decode_video2(m_ifmt_ctx->streams[video_selected_stream_index]->codec, aframe, &got_frame, &pkt);
// 			if (len < 0)
// 			{
// 				return len;
// 			}
// 			if (got_frame)
// 			{
// 			}
			FrameBitrate frame;
			frame.pts = pkt.pts;
			frame.framesize = pkt.size;
			frame.duration = pkt.duration;
			frame.keyframe = (pkt.flags & AV_PKT_FLAG_KEY) ? 1 : 0;
			m_frameInfoList.push_back(frame);
//			log_packet(m_ifmt_ctx, &pkt, firstpts);
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

BOOL CGetVideoBitrate::GetStreamInfo(UINT index, StreamInfo *pStreamInfo)
{
	if (pStreamInfo && m_ifmt_ctx && index >= 0 && index < m_ifmt_ctx->nb_streams)
	{
		pStreamInfo->codec_type = m_ifmt_ctx->streams[index]->codec->codec_type;
		pStreamInfo->codec_id = m_ifmt_ctx->streams[index]->codec->codec_id;
		pStreamInfo->width = m_ifmt_ctx->streams[index]->codec->width;
		pStreamInfo->height = m_ifmt_ctx->streams[index]->codec->height;
		pStreamInfo->duration = m_ifmt_ctx->streams[index]->duration;// -m_ifmt_ctx->streams[index]->start_time;
		pStreamInfo->duration = (LONGLONG)((av_q2d(m_ifmt_ctx->streams[index]->time_base) * pStreamInfo->duration) * 10000000L);
		pStreamInfo->sample_aspect_ratio = m_ifmt_ctx->streams[index]->codec->sample_aspect_ratio;
		pStreamInfo->frame_rate = ((double)m_ifmt_ctx->streams[index]->r_frame_rate.num) / m_ifmt_ctx->streams[index]->r_frame_rate.den;
		pStreamInfo->time_base = m_ifmt_ctx->streams[index]->time_base;
		const AVCodecDescriptor *desc = avcodec_descriptor_get(m_ifmt_ctx->streams[index]->codec->codec_id);
		if (desc)
		{
			pStreamInfo->strStreamName = desc->long_name;
		}
		return TRUE;
	}
	return FALSE;
}




