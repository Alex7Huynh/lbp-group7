// InputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InputDlg.h"
#include "afxdialogex.h"
#include "resource.h"

UINT gLabelId = 0;
// CInputDlg dialog

IMPLEMENT_DYNAMIC(CInputDlg, CDialogEx)

CInputDlg::CInputDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInputDlg::IDD, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
}

CInputDlg::~CInputDlg()
{
}

void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TXT_LABEL, m_txtLabel);
	DDX_Control(pDX, IDOK, m_btnOK);
}


BEGIN_MESSAGE_MAP(CInputDlg, CDialogEx)
	ON_EN_CHANGE(IDC_TXT_LABEL, &CInputDlg::OnEnChangeTxtLabel)
END_MESSAGE_MAP()


// CInputDlg message handlers

UINT CInputDlg::GetLabelValue()
{
	return gLabelId;
}

void CInputDlg::OnEnChangeTxtLabel()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString tmp;
	m_txtLabel.GetWindowText(tmp);
	tmp = tmp.Trim();

	for (int i = 0; i < tmp.GetLength(); ++i)
	{
		if (isdigit(tmp.GetAt(i)) == 0)
		{
			m_btnOK.EnableWindow(FALSE);
			return;
		}
	}

	gLabelId = atoi(tmp);
	m_btnOK.EnableWindow(TRUE);
}
