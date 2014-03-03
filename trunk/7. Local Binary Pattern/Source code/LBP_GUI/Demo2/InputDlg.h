#pragma once
#include "resource.h"
#include "afxwin.h"


// CInputDlg dialog

class CInputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInputDlg)

public:
	CInputDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputDlg();

// Dialog Data
	enum { IDD = IDD_INPUT_LABEL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	UINT GetLabelValue();
	afx_msg void OnEnChangeTxtLabel();
private:
	CEdit m_txtLabel;
	UINT m_lableID;
protected:
	CButton m_btnOK;
};
