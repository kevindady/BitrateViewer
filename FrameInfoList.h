#pragma once

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

	BOOL AddListItem(const std::vector<FrameBitrate > *pFrameInfoList);

public:
	CFrameInfoList() : m_pFrameInfoList(NULL) {};
	virtual ~CFrameInfoList() {};
	
private:
	const std::vector<FrameBitrate > *m_pFrameInfoList;
};

