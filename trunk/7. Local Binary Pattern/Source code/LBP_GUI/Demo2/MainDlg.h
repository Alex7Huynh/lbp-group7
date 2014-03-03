#pragma once
#include "resource.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "facerec.hpp"

using namespace cv;
using namespace std;
using namespace libfacerec;

// CMainDlg dialog

class CMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMainDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();

private:
	bool Train_AutoMode1();
	void Test_AutoMode1();

	bool Train_AutoMode2();
	void Test_AutoMode2();

private:
	vector<Mat> m_imagesTrains_mode1;
	vector<int> m_labelsTrains_mode1;

	vector<Mat> m_imagesTrains_mode2;
	vector<int> m_labelsTrains_mode2;
public:
	afx_msg void OnBnClickedButton4();
};
