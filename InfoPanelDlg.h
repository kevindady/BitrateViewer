#pragma once

class CInfoPanelDlg : public CDialogImpl<CInfoPanelDlg>
{
public:
	enum { IDD = IDD_INFOPANEL };

	BEGIN_MSG_MAP(CInfoPanelDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDC_OPENBTN, OnOpenFile)
		COMMAND_ID_HANDLER(IDC_PARSE, OnParse)

	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOpenFile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnParse(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

public:
	BOOL Open(LPCTSTR lpszFileName);

private:
	void AddStreamInfo(CString strDesc, UINT index);

private:
	CGetVideoBitrate m_clGetVideoBitrate;
};

