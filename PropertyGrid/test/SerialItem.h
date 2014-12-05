#pragma once
#include "..\CustomItem.h"

class CSerialItem : public ICustomItem
{
public:
  CSerialItem(void);
  ~CSerialItem(void);
  virtual CPropertyGrid::EEditMode GetEditMode();
  virtual void DrawItem(CDC& dc, CRect rc, bool focused);
  virtual wstring GetStringForInPlaceEdit();
  virtual bool OnItemEdited(wstring strNewValue);

protected:
  wstring m_serial;
};
