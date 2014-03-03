
// Demo2Dlg.h : header file
//

#pragma once
#include "afxwin.h"
#include <vector>
#include <map>
#include "facerec.hpp"

using namespace cv;
using namespace std;
using namespace libfacerec;


// CDemo2Dlg dialog
class CDemo2Dlg : public CDialogEx
{
// Construction
public:
	CDemo2Dlg(CWnd* pParent = NULL);	// standard constructor
	~CDemo2Dlg();

// Dialog Data
	enum { IDD = IDD_DEMO2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CStatic m_picBoxTran;
	CStatic m_picBoxTest;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	vector<CString> m_listFileName;
	UINT m_Index;
	UINT m_timerID;

	vector<Mat> m_Images;
	vector<int> m_Lables;

	vector<int> m_labelsStore;
	vector<CString> m_listFileNameTmp;
	int m_currentLabelID;

	void FindAllImageNameByLabel(int inLabel);
public:
	afx_msg void OnEnUpdateEdit1();
	afx_msg void OnEnChangeEdit1();
private:
	CEdit m_labelEditBox;
public:
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};
