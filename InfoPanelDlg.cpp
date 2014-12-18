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
	clShowMode.ResetContent();
	clShowMode.AddString(_T("Second mode"));
	clShowMode.AddString(_T("GOP mode"));
	clShowMode.AddString(_T("Frame mode"));
	clShowMode.SetItemData(0, (DWORD_PTR)SECOND_MODE);
	clShowMode.SetItemData(1, (DWORD_PTR)GOP_MODE);
	clShowMode.SetItemData(2, (DWORD_PTR)FRAME_MODE);
	CComboBox clStreamInfo = GetDlgItem(IDC_STREAMINFO);
	clStreamInfo.ResetContent();

	m_clFrameInfoList.Init(GetDlgItem(IDC_PTSLIST));

	return TRUE;
}

LRESULT CInfoPanelDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	m_clFrameInfoList.Uninit();
	bHandled = FALSE;
	return 0;
}

LRESULT CInfoPanelDlg::OnOpenFile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CFileDialog clFileDialog(TRUE);
	if (clFileDialog.DoModal(m_hWnd) == IDOK)
	{
		Open(clFileDialog.m_ofn.lpstrFile);
	}
	return 0;
}

LRESULT CInfoPanelDlg::OnParse(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CComboBox clStreamInfo = GetDlgItem(IDC_STREAMINFO);
	LONG lSel = clStreamInfo.GetCurSel();
	if (lSel != CB_ERR)
	{
		UINT video_selected_stream_index = (UINT)clStreamInfo.GetItemData(lSel);
		if (m_clGetVideoBitrate.Parse(video_selected_stream_index))
		{
			m_clFrameInfoList.AddListItem(&m_clGetVideoBitrate.GetFrameList());
		}
	}
	else
	{
		MessageBox(_T("Please select a stream."), _T("Error"), MB_OK);
	}
	return 0;
}

BOOL CInfoPanelDlg::Open(LPCTSTR lpszFileName)
{
	if (!m_clGetVideoBitrate.Open(lpszFileName))
	{
		MessageBox(m_clGetVideoBitrate.GetErrorMsg(), _T("Error"), MB_OK);
		return FALSE;
	}
	else
	{
		UINT count = m_clGetVideoBitrate.GetStreamCount();
		if (count > 0)
		{
			for (UINT i = 0; i < count; ++i)
			{
				StreamInfo stStreamInfo;
				if (m_clGetVideoBitrate.GetStreamInfo(i, &stStreamInfo))
				{
					AddStreamInfo(stStreamInfo.strStreamName, i);
				}
			}
		}
	}
	return TRUE;
}

void CInfoPanelDlg::AddStreamInfo(CString strDesc, UINT index)
{
	CComboBox clStreamInfo = GetDlgItem(IDC_STREAMINFO);
	int nAddedIndex = clStreamInfo.AddString(strDesc);
	clStreamInfo.SetItemData(nAddedIndex, (DWORD_PTR)index);
}


