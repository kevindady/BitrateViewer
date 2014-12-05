#include "stdafx.h"

static inline char *my_av_ts_make_string(int64_t ts)
{
	static char buf[AV_TS_MAX_STRING_SIZE] = {0};
	if (ts == AV_NOPTS_VALUE) _snprintf_s(buf, AV_TS_MAX_STRING_SIZE, 1024, "NOPTS");
	else                      _snprintf_s(buf, AV_TS_MAX_STRING_SIZE, 1024, "%"PRId64, ts);
	return buf;
}

static inline char *my_av_ts_make_time_string(int64_t ts, AVRational *tb)
{
	static char buf[AV_TS_MAX_STRING_SIZE] = {0};
	if (ts == AV_NOPTS_VALUE) _snprintf_s(buf, AV_TS_MAX_STRING_SIZE, 1024, "NOPTS");
	else                      _snprintf_s(buf, AV_TS_MAX_STRING_SIZE, 1024, "%.6g", av_q2d(*tb) * ts);
	return buf;
}

static inline char *my_av_make_error_string(size_t errbuf_size, int errnum)
{
	static char errbuf[AV_ERROR_MAX_STRING_SIZE] = {0};
	av_strerror(errnum, errbuf, errbuf_size);
	return errbuf;
}

#define my_av_ts2str(ts) my_av_ts_make_string(ts)
#define my_av_ts2timestr(ts, tb) my_av_ts_make_time_string(ts, tb)
#define my_av_err2str(errnum) my_av_make_error_string(AV_ERROR_MAX_STRING_SIZE, errnum)

CGetVideoBitrate::CGetVideoBitrate()
{
	memset(m_strErrorMsg, 0, sizeof(m_strErrorMsg));
}

CGetVideoBitrate::~CGetVideoBitrate()
{
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

int CGetVideoBitrate::GetVideoBitrate(const char *in_filename)
{
    AVFormatContext *ifmt_ctx = NULL;
    AVPacket pkt;
	int ret;
	unsigned int i;
	unsigned int video_selected_stream_index = 0;

    av_register_all();

    if ((ret = avformat_open_input(&ifmt_ctx, in_filename, 0, 0)) < 0)
	{
		sprintf_s(m_strErrorMsg, ERROR_MSG_SIZE, "Could not open input file '%s'", in_filename);
        goto end;
    }

    if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0)
	{
		sprintf_s(m_strErrorMsg, ERROR_MSG_SIZE, "Failed to retrieve input stream information");
        goto end;
    }

    av_dump_format(ifmt_ctx, 0, in_filename, 0);

    for (i = 0; i < ifmt_ctx->nb_streams; i++)
	{
        AVStream *in_stream = ifmt_ctx->streams[i];
		if (in_stream->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			video_selected_stream_index = i;
			break;
		}
    }

    while (1)
	{
        AVStream *in_stream;

        ret = av_read_frame(ifmt_ctx, &pkt);
        if (ret < 0)
            break;

        in_stream  = ifmt_ctx->streams[pkt.stream_index];

		if (pkt.stream_index == video_selected_stream_index)
		{
			log_packet(ifmt_ctx, &pkt, "in");
		}

        av_free_packet(&pkt);
    }

end:

    avformat_close_input(&ifmt_ctx);


    if (ret < 0 && ret != AVERROR_EOF)
	{
		sprintf_s(m_strErrorMsg, ERROR_MSG_SIZE, "Error occurred: %s\n", my_av_err2str(ret));
        return 1;
    }

    return 0;
}


