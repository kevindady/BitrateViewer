#pragma once

#define BT_NOTIFY_HANDLER(theChainMember, msgMapID) \
	if (uMsg == WM_NOTIFY && theChainMember.IsWindow() && theChainMember.GetDlgCtrlID() == ((LPNMHDR)lParam)->idFrom && theChainMember == ((LPNMHDR)lParam)->hwndFrom) \
	{ \
		if(theChainMember.ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult, msgMapID)) \
			return TRUE; \
	}

class CInfoPanelDlg : public CDialogImpl<CInfoPanelDlg>
{
public:
	enum { IDD = IDD_INFOPANEL };

	BEGIN_MSG_MAP(CInfoPanelDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDC_OPENBTN, OnOpenFile)
		COMMAND_ID_HANDLER(IDC_PARSE, OnParse)
		BT_NOTIFY_HANDLER(m_clFrameInfoList, 0)

	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOpenFile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnParse(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

public:
	BOOL Open(LPCTSTR lpszFileName);

private:
	void AddStreamInfo(CString strDesc, UINT index);

private:
	CGetVideoBitrate m_clGetVideoBitrate;
	CFrameInfoList m_clFrameInfoList;
};

