// BitrateViewerView.cpp : implementation of the CBitrateViewerView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "BitrateViewerView.h"

BOOL CBitrateViewerView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

LRESULT CBitrateViewerView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);

	//TODO: Add your drawing code here
	return 0;
}

LRESULT CBitrateViewerView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	m_clInfoPanelDlg.Create(m_hWnd);
	m_clInfoPanelDlg.ShowWindow(SW_SHOW);
	bHandled = FALSE;
	return 0;
}

LRESULT CBitrateViewerView::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	m_clInfoPanelDlg.DestroyWindow();
	return 0;
}

void CBitrateViewerView::AddStreamInfo(CString strDesc, UINT index)
{
	m_clInfoPanelDlg.AddStreamInfo(strDesc, index);
}

