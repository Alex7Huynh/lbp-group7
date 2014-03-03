
// Demo2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Demo2.h"
#include "Demo2Dlg.h"
#include "afxdialogex.h"
#include <afxdlgs.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include "InputDlg.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


inline HBITMAP IplImage2DIB(const IplImage *Image)
{
	int bpp = Image->nChannels * 8;
	assert(Image->width >= 0 && Image->height >= 0 &&(bpp == 8 || bpp == 24 || bpp == 32));
	CvMat dst;
	void* dst_ptr = 0;
	HBITMAP hbmp = NULL;
	unsigned char buffer[sizeof(BITMAPINFO) + 255*sizeof(RGBQUAD)];
	BITMAPINFO* bmi = (BITMAPINFO*)buffer;
	BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);

	ZeroMemory(bmih, sizeof(BITMAPINFOHEADER));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biWidth = Image->width;
	bmih->biHeight = Image->origin ? abs(Image->height) :
		-abs(Image->height);
	bmih->biPlanes = 1;
	bmih->biBitCount = bpp;
	bmih->biCompression = BI_RGB;

	if (bpp == 8) {
		RGBQUAD* palette = bmi->bmiColors;
		int i;
		for (i = 0; i < 256; i++) {
			palette[i].rgbRed = palette[i].rgbGreen = palette[i].rgbBlue
				= (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}

	hbmp = CreateDIBSection(NULL, bmi, DIB_RGB_COLORS, &dst_ptr, 0, 0);
	cvInitMatHeader(&dst, Image->height, Image->width, CV_8UC3,
		dst_ptr, (Image->width * Image->nChannels + 3) & -4);
	cvConvertImage(Image, &dst, Image->origin ? CV_CVTIMG_FLIP : 0);

	return hbmp;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDemo2Dlg dialog




CDemo2Dlg::CDemo2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDemo2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_listFileName.clear();
	m_Index = 0;
	m_Images.clear();
	m_Lables.clear();

	m_labelsStore.clear();
	m_listFileNameTmp.clear();
	m_currentLabelID = -1;
}

CDemo2Dlg::~CDemo2Dlg()
{
	m_Lables.clear();

	for (vector<Mat>::iterator it = m_Images.begin(); it != m_Images.end(); ++it)
	{
		it->release();
	}
	m_Images.clear();

	m_listFileName.clear();
	m_listFileNameTmp.clear();
	m_labelsStore.clear();
}

void CDemo2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_PIC_BOX_TRAN, m_picBoxTran);
	DDX_Control(pDX, ID_PIC_BOX_TEST, m_picBoxTest);
	DDX_Control(pDX, IDC_EDIT1, m_labelEditBox);
}

BEGIN_MESSAGE_MAP(CDemo2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CDemo2Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDemo2Dlg::OnBnClickedButton2)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT1, &CDemo2Dlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON3, &CDemo2Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDemo2Dlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CDemo2Dlg message handlers

BOOL CDemo2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	SetTimer(m_timerID, 1000, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDemo2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDemo2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDemo2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/**
 *	Event when Train button is pushed.
 *	@note Notes
 *	
**/
void CDemo2Dlg::OnBnClickedButton1()
{
	// Call Input Label dialog
	CInputDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		// Get a label value
		UINT labelValue = dlg.GetLabelValue();

		// Show Open file dialog
		TCHAR szFilters[]= _T("All Files (*.*)|*.*||");
		// Create an Open dialog;
		CFileDialog fileDlg (TRUE, _T("All"), _T("*.*"),
			OFN_FILEMUSTEXIST| OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, szFilters, this);

		// Display the file dialog. When user clicks OK, fileDlg.DoModal()
		// returns IDOK.
		if (fileDlg.DoModal() == IDOK)
		{
			bool isNeedToStore = false;

			// Loop all selected file
			POSITION p = fileDlg.GetStartPosition();
			while (p)
			{
				CString m_strPath = fileDlg.GetNextPathName(p);
				m_listFileName.push_back(m_strPath);
				m_Images.push_back(imread(m_strPath.GetBuffer(), 0));

				m_Lables.push_back(labelValue);

				isNeedToStore = true;
			}

			//
			if (isNeedToStore)
			{
				// Check the labelValue was saved before or not.
				vector<int>::iterator it = ::find(m_labelsStore.begin(), m_labelsStore.end(), labelValue);
				if (it == m_labelsStore.end())
				{
					m_labelsStore.push_back(labelValue);
				}

				// Set current label to show train images
				m_currentLabelID = labelValue;
				FindAllImageNameByLabel(m_currentLabelID);

				// Update Label Edit-box on dilog
				char txtLabel[10] = {0};
				itoa(m_currentLabelID, txtLabel, 10);
				m_labelEditBox.SetWindowText(txtLabel);
			}
		}
	}
}


/**
 *	Event when Test button is pushed.
 *	@note Notes
 *	
**/
void CDemo2Dlg::OnBnClickedButton2()
{
	TCHAR szFilters[]= _T("All Files (*.*)|*.*||");
	// Create an Open dialog
	CFileDialog fileDlg (TRUE, _T("All"), _T("*.*"),
		OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal()
	// returns IDOK.
	if (fileDlg.DoModal() == IDOK)
	{
		// Get path of the selected file.
		CString m_strPathname = fileDlg.GetPathName();

		// Display test image on Test Image picture-box
		IplImage* src = cvLoadImage(m_strPathname, CV_INTER_LINEAR);			// Load image
		m_picBoxTest.SetBitmap(IplImage2DIB(src));								// Display
		cvReleaseImage(&src);													// Release image

		// Train + Test
		Ptr<libfacerec::FaceRecognizer> model = libfacerec::createLBPHFaceRecognizer(1,8,8,8,123.0);
		model->train(m_Images, m_Lables);

		Mat imageTest = imread(m_strPathname.GetString(), 0);
		int predictedLabel = model->predict(imageTest);

		string model_info = format("\tLBPH(radius=%i, neighbors=%i, grid_x=%i, grid_y=%i, threshold=%.2f)",
			model->getInt("radius"),
			model->getInt("neighbors"),
			model->getInt("grid_x"),
			model->getInt("grid_y"),
			model->getDouble("threshold"));

		char tmp[1000] = {0};
		sprintf(tmp, "Predicted class = : %d\n%s", predictedLabel, model_info.c_str());

		MessageBox(tmp, NULL, 1);

		// Display all image of train images have a label = predictedLabel.
		FindAllImageNameByLabel(predictedLabel);

		char labelTxt[10] = {0};
		itoa(predictedLabel, labelTxt, 10);
		m_labelEditBox.SetWindowText(labelTxt);
	}
}


/**
 *	Play a slide show train images.
 *	@note Notes
 *	
**/
void CDemo2Dlg::OnTimer(UINT_PTR nIDEvent)
{
	++m_Index;

	if (m_Index <= m_listFileNameTmp.size())
	{
		IplImage* src = cvLoadImage(m_listFileNameTmp.at(m_Index - 1), CV_INTER_LINEAR);
		HBITMAP hBmp = IplImage2DIB(src);
		m_picBoxTran.SetBitmap(hBmp);
		BOOL a = DeleteObject(hBmp);
		cvReleaseImage(&src);
	}else{
		m_Index = 0;
	}

	CDialogEx::OnTimer(nIDEvent);
}


/**
 *	Find all train images have a label (class) same as parameter.
 *	@param	[in] inLabel : the label (class)
 *	@note Notes
 *	
**/
void CDemo2Dlg::FindAllImageNameByLabel(int inLabel)
{
	m_listFileNameTmp.clear();

	if (inLabel < 0)
	{
		m_listFileNameTmp = m_listFileName;
		return;
	}

	for (vector<int>::iterator it = m_Lables.begin(); it != m_Lables.end(); ++it)
	{
		if (*it == inLabel)
		{
			m_listFileNameTmp.push_back(m_listFileName.at(::distance(m_Lables.begin(), it)));
		}
	}
}



void CDemo2Dlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	CString tmp;
	m_labelEditBox.GetWindowText(tmp);
	

	m_currentLabelID = -1;
	int label = atoi(tmp.GetString());
	vector<int>::iterator it = ::find(m_labelsStore.begin(), m_labelsStore.end(), label);

	if (it != m_labelsStore.end())
	{
		m_currentLabelID = *it;
	}

	FindAllImageNameByLabel(m_currentLabelID);
}


void CDemo2Dlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	int size = m_labelsStore.size();

	if (size <= 1)
	{
		return;
	}

	::sort(m_labelsStore.begin(), m_labelsStore.end());

	vector<int>::iterator it = ::find(m_labelsStore.begin(), m_labelsStore.end(), m_currentLabelID);
	if (it == m_labelsStore.end())
	{
		m_currentLabelID = m_labelsStore.at(size - 1);
	}
	else
	{
		int dis = ::distance(m_labelsStore.begin(), it);
		if (dis == 0)
		{
			m_currentLabelID = m_labelsStore.at(size - 1);
		}
		else
		{
			m_currentLabelID = m_labelsStore.at(dis - 1);
		}
	}

	FindAllImageNameByLabel(m_currentLabelID);
	char labelTxt[10] = {0};
	itoa(m_currentLabelID, labelTxt, 10);
	m_labelEditBox.SetWindowTextA(labelTxt);
}


void CDemo2Dlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	int size = m_labelsStore.size();

	if (size <= 1)
	{
		return;
	}

	::sort(m_labelsStore.begin(), m_labelsStore.end());

	vector<int>::iterator it = ::find(m_labelsStore.begin(), m_labelsStore.end(), m_currentLabelID);
	if (it == m_labelsStore.end())
	{
		m_currentLabelID = m_labelsStore.at(0);
	}
	else
	{
		int dis = ::distance(m_labelsStore.begin(), it);
		if (dis == (size - 1))
		{
			m_currentLabelID = m_labelsStore.at(0);
		}
		else
		{
			m_currentLabelID = m_labelsStore.at(dis + 1);
		}
	}

	FindAllImageNameByLabel(m_currentLabelID);
	char labelTxt[10] = {0};
	itoa(m_currentLabelID, labelTxt, 10);
	m_labelEditBox.SetWindowTextA(labelTxt);
}
