#include "stdafx.h"

enum ShowMode
{
	SECOND_MODE,
	GOP_MODE,
	FRAME_MODE,
};

LRESULT CInfoPanelDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CComboBox clShowMode(GetDlgItem(IDC_SHOWMODE));
	clShowMode.AddString(_T("Second mode"));
	clShowMode.AddString(_T("GOP mode"));
	clShowMode.AddString(_T("Frame mode"));
	clShowMode.SetItemData(0, (DWORD_PTR)SECOND_MODE);
	clShowMode.SetItemData(1, (DWORD_PTR)GOP_MODE);
	clShowMode.SetItemData(2, (DWORD_PTR)FRAME_MODE);
	return TRUE;
}


