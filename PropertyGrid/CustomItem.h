#pragma once
#include "PropertyGrid.h"

class ICustomItem
{
public:
  // basic required stuff
  virtual CPropertyGrid::EEditMode GetEditMode() = 0;
  virtual void DrawItem(CDC& dc, CRect rc, bool focused) = 0;

  // validation
  virtual void ValidateChanges() {}

  // mouse stuff
  virtual bool OnLButtonDown(CRect rc, CPoint pt) { return false; }
  virtual void OnMouseMove(CRect rc, CPoint pt) {}
  virtual void OnLButtonUp(CRect rc, CPoint pt) {}

  // in-place edit
  virtual wstring GetStringForInPlaceEdit() { return _T(""); }
  virtual bool OnItemEdited(wstring strNewValue) { return false; }

  // dropdown edit
  virtual void ShowDropDown(CRect rc) {}

  // modal edit
  virtual bool OnEditItem() { return false; }

protected:
  friend class CPropertyGrid;
  CPropertyGrid* m_pGrid;
};
