#include "stdafx.h"

CFrameInfoList::CFrameInfoList() :
m_pFrameInfoList(NULL),
m_bitrateDisplayMode(BITRATE_DISPLAY_FRAMESIZE)
{
};

BOOL CFrameInfoList::Init(HWND hListCtrl)
{
	SubclassWindow(hListCtrl);
	
	if (IsWindow())
	{
		SetUnicodeFormat(TRUE);
		SetViewType(LVS_REPORT);
		DWORD dwExtStyle = GetExtendedListViewStyle();
		dwExtStyle |= LVS_EX_FULLROWSELECT;
		dwExtStyle &= ~LVS_EX_CHECKBOXES;
		SetExtendedListViewStyle(dwExtStyle);

		// Create list view columns
		InsertColumn(0, _T("PTS"), LVCFMT_LEFT, 150, 0);
		InsertColumn(1, _T("Time"), LVCFMT_RIGHT, 150, 1);
		InsertColumn(2, _T("Bitrate"), LVCFMT_LEFT, 150, 2);
		InsertColumn(3, _T("Key Frame"), LVCFMT_LEFT, 150, 3);
		InsertColumn(4, _T("Frame Duration"), LVCFMT_LEFT, 150, 3);

		return TRUE;
	}
  
	return FALSE;
}

BOOL CFrameInfoList::Uninit()
{
	DeleteAllItems();
	UnsubclassWindow();
	return TRUE;
}

BOOL CFrameInfoList::AddListItem(StreamInfo *pStreamInfo, const std::vector<FrameBitrate > *pFrameInfoList)
{
	m_streamInfo = *pStreamInfo;
	m_pFrameInfoList = pFrameInfoList;
	size_t count = (*m_pFrameInfoList).size();
	if (count < 0)
	{
		return FALSE;
	}

	for (int i = count - 1; i >= 0; --i)
	{
		LVITEM   lvItem;

		ZeroMemory(&lvItem, sizeof(lvItem));

		//fill in the TV_ITEM structure for this item
		lvItem.mask = LVIF_PARAM | LVIF_TEXT;

		lvItem.lParam = (LPARAM)i;

		//text and images are done on a callback basis
		lvItem.pszText = LPSTR_TEXTCALLBACK;

		// insert list item
		int iRet = InsertItem(&lvItem);

	}
	return TRUE;
}

LRESULT CFrameInfoList::OnGetDispInfo(int idCtrl, LPNMHDR pnmh, BOOL &bHandled)
{
	NMLVDISPINFO *pdi = (NMLVDISPINFO*)pnmh;
	LONG lItemIndex = (LONG)pdi->item.lParam;
	TCHAR str[MAX_PATH] = {0};
	
	if (pdi->item.mask & LVIF_TEXT)
	{
		switch (pdi->item.iSubItem)
		{
			case 0:  //PTS
			{
				_stprintf_s(str, MAX_PATH, _T("%I64d"), (*m_pFrameInfoList)[lItemIndex].pts);
				lstrcpyn(pdi->item.pszText, str, pdi->item.cchTextMax);
			}
			break;

			case 1:  //Time
			{
				LONGLONG time = (LONGLONG)((av_q2d(m_streamInfo.time_base) * ((*m_pFrameInfoList)[lItemIndex].pts - (*m_pFrameInfoList)[0].pts)) * 10000000L);
				_stprintf_s(str, MAX_PATH, _T("%I64d"), time);
				lstrcpyn(pdi->item.pszText, str, pdi->item.cchTextMax);
			}
			break;
			
			case 2:  //Bitrate
			{
				if (m_bitrateDisplayMode == BITRATE_DISPLAY_NORMAL)
				{
					int framerate = int((*m_pFrameInfoList)[lItemIndex].framesize * 8 * m_streamInfo.frame_rate);
					_stprintf_s(str, MAX_PATH, _T("%d"), framerate);
					lstrcpyn(pdi->item.pszText, str, pdi->item.cchTextMax);
				}
				else if (m_bitrateDisplayMode == BITRATE_DISPLAY_FRAMESIZE)
				{
					_stprintf_s(str, MAX_PATH, _T("%d (%d byte)"), (*m_pFrameInfoList)[lItemIndex].framesize * 8, (*m_pFrameInfoList)[lItemIndex].framesize);
					lstrcpyn(pdi->item.pszText, str, pdi->item.cchTextMax);
				}
				else if (m_bitrateDisplayMode == BITRATE_DISPLAY_FRAMESIZE_BIT)
				{
					_stprintf_s(str, MAX_PATH, _T("%d"), (*m_pFrameInfoList)[lItemIndex].framesize * 8);
					lstrcpyn(pdi->item.pszText, str, pdi->item.cchTextMax);
				}
				else if (m_bitrateDisplayMode == BITRATE_DISPLAY_FRAMESIZE_BYTE)
				{
					_stprintf_s(str, MAX_PATH, _T("%d"), (*m_pFrameInfoList)[lItemIndex].framesize);
					lstrcpyn(pdi->item.pszText, str, pdi->item.cchTextMax);
				}
			}
			break;

			case 3: // Key Frame
			{
				_stprintf_s(str, MAX_PATH, _T("%d"), (*m_pFrameInfoList)[lItemIndex].keyframe);
				lstrcpyn(pdi->item.pszText, str, pdi->item.cchTextMax);
			}
			break;

			case 4: // Frame Duration
			{
				LONGLONG time = (LONGLONG)((av_q2d(m_streamInfo.time_base) * (*m_pFrameInfoList)[lItemIndex].duration) * 10000000L);
				_stprintf_s(str, MAX_PATH, _T("%I64d"), time);
				lstrcpyn(pdi->item.pszText, str, pdi->item.cchTextMax);
			}
			break;
		}
	}
	return S_OK;
}

LRESULT CFrameInfoList::OnDeleteItem(int idCtrl, LPNMHDR pnmh, BOOL &bHandled)
{
	return S_OK;
}



