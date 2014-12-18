#include "stdafx.h"

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

BOOL CFrameInfoList::AddListItem(const std::vector<FrameBitrate > *pFrameInfoList)
{
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
			}
			break;
			
			case 2:  //Bitrate
			{
			}
			break;

			case 3: // Key Frame
			{
			}
			break;

			case 4: // Frame Duration
			{
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



