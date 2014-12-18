#pragma once

enum BITRATE_DISPLAY_MODE
{
	BITRATE_DISPLAY_NORMAL,
	BITRATE_DISPLAY_FRAMESIZE,
	BITRATE_DISPLAY_FRAMESIZE_BIT,
	BITRATE_DISPLAY_FRAMESIZE_BYTE,
};

class CFrameInfoList : public CCheckListViewCtrl
{
public:
	BEGIN_MSG_MAP(CFrameInfoList)
		NOTIFY_CODE_HANDLER(LVN_GETDISPINFOW, OnGetDispInfo)
		NOTIFY_CODE_HANDLER(LVN_DELETEITEM, OnDeleteItem)
	END_MSG_MAP()
		
	LRESULT OnGetDispInfo(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnDeleteItem(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);

public:
	BOOL Init(HWND hListCtrl);
	BOOL Uninit();

	BOOL AddListItem(StreamInfo *pStreamInfo, const std::vector<FrameBitrate > *pFrameInfoList);

	void setBitrateDisplayMode(BITRATE_DISPLAY_MODE mode);

public:
	CFrameInfoList();
	virtual ~CFrameInfoList() {};
	
private:
	const std::vector<FrameBitrate >	*m_pFrameInfoList;
	StreamInfo							m_streamInfo;
	BITRATE_DISPLAY_MODE				m_bitrateDisplayMode;
};

