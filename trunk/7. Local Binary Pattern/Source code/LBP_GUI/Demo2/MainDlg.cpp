// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlg.h"
#include "afxdialogex.h"
#include "Demo2Dlg.h"

static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
	std::ifstream file(filename, ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line, path, classlabel;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, classlabel);
		if(!path.empty() && !classlabel.empty()) {
			images.push_back(imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
}


// CMainDlg dialog

IMPLEMENT_DYNAMIC(CMainDlg, CDialogEx)

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainDlg::IDD, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

CMainDlg::~CMainDlg()
{
	for (vector<Mat>::iterator it = m_imagesTrains_mode1.begin(); it != m_imagesTrains_mode1.end(); ++it)
	{
		it->release();
	}
	m_imagesTrains_mode1.clear();
	m_labelsTrains_mode1.clear();

	for (vector<Mat>::iterator it = m_imagesTrains_mode2.begin(); it != m_imagesTrains_mode2.end(); ++it)
	{
		it->release();
	}
	m_imagesTrains_mode2.clear();
	m_labelsTrains_mode2.clear();
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMainDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CMainDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CMainDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CMainDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CMainDlg message handlers


void CMainDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CDemo2Dlg dlg;
	dlg.DoModal();
}


void CMainDlg::OnBnClickedButton2()
{
	// Train
	MessageBox("Choice a train file!", NULL, 0);
	if (Train_AutoMode1())
	{
		MessageBox("Choice a test file!", NULL, 0);
		Test_AutoMode1();
	}
}


bool CMainDlg::Train_AutoMode1()
{
	for (vector<Mat>::iterator it = m_imagesTrains_mode1.begin(); it != m_imagesTrains_mode1.end(); ++it)
	{
		it->release();
	}
	m_imagesTrains_mode1.clear();
	m_labelsTrains_mode1.clear();

	// Show Open File Dialog
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

		 try {
			read_csv(m_strPathname.GetString(), m_imagesTrains_mode1, m_labelsTrains_mode1);
		} catch (cv::Exception& e) {
			MessageBox("Error opening train file!");
			return false;
		}

		return true;
	}

	return false;
}

void CMainDlg::Test_AutoMode1()
{
	// Show Open File Dialog
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

		vector<Mat> imagesTest;
		vector<int> labelsTest;

		try
		{
			read_csv(m_strPathname.GetString(), imagesTest, labelsTest);
		}
		catch (cv::Exception& e)
		{
			MessageBox("Error opening test file!");
			return;
		}

		// Train
		const clock_t begin_time = clock();
		Ptr<FaceRecognizer> model = createLBPHFaceRecognizer(1,8,8,8,123.0);
		model->train(m_imagesTrains_mode1, m_labelsTrains_mode1);

		// Test
		int numOfItems = imagesTest.size();
		int numOfRightItems = 0;
		for (int i = 0; i < numOfItems; ++i)
		{
			if (labelsTest.at(i) == model->predict(imagesTest.at(i)))
			{
				++numOfRightItems;
			}
		}

		int runningTime = float( clock () - begin_time ) / CLOCKS_PER_SEC;

		// Show result dialog
		string strTmp = format("Result:\tTotal time : %d seconds\n\tTotal : %d\n\tRight : %d\n\tWrong : %d\n\n\tLBPH(radius=%i, neighbors=%i, grid_x=%i, grid_y=%i, threshold=%.2f)",
			runningTime, numOfItems, numOfRightItems, numOfItems - numOfRightItems,
			model->getInt("radius"),
			model->getInt("neighbors"),
			model->getInt("grid_x"),
			model->getInt("grid_y"),
			model->getDouble("threshold"));

		MessageBox(strTmp.c_str(), NULL, 1);
	}
}



void CMainDlg::OnBnClickedButton3()
{
	// Train
	MessageBox("Choice a train file!", NULL, 0);
	if (Train_AutoMode2())
	{
		MessageBox("Choice a test file!", NULL, 0);
		Test_AutoMode2();
	}
}


bool CMainDlg::Train_AutoMode2()
{
	for (vector<Mat>::iterator it = m_imagesTrains_mode2.begin(); it != m_imagesTrains_mode2.end(); ++it)
	{
		it->release();
	}
	m_imagesTrains_mode2.clear();
	m_labelsTrains_mode2.clear();

	// Show Open File Dialog
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

		 try {
			read_csv(m_strPathname.GetString(), m_imagesTrains_mode2, m_labelsTrains_mode2);
		} catch (cv::Exception& e) {
			MessageBox("Error opening train file!");
			return false;
		}

		return true;
	}

	return false;
}


void CMainDlg::Test_AutoMode2()
{
	// Show Open File Dialog
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

		vector<Mat> imagesTest;
		vector<int> labelsTest;

		try
		{
			read_csv(m_strPathname.GetString(), imagesTest, labelsTest);
		}
		catch (cv::Exception& e)
		{
			MessageBox("Error opening test file!");
			return;
		}

		// Train
		const clock_t begin_time = clock();
		Ptr<FaceRecognizer> model = createLBPHFaceRecognizer(1,8,8,8,123.0);
		model->train(m_imagesTrains_mode2, m_labelsTrains_mode2);

		// Test
		int numOfItems = imagesTest.size();
		int numOfRightItems = 0;
		for (int i = 0; i < numOfItems; ++i)
		{
			int predictedLabel = model->predict(imagesTest.at(i));
			if (labelsTest.at(i) == predictedLabel)
			{
				++numOfRightItems;
			}

			// Train the image was tested before.
			vector<Mat> matTmp;
			vector<int> labelTmp;
			matTmp.push_back(imagesTest.at(i));
			labelTmp.push_back(predictedLabel);
			model->update(matTmp, labelTmp);

			//vector<Mat> histograms = model->getMatVector("histograms");
			//_CrtDbgReport(_CRT_WARN, __FILE__, __LINE__, "", "i = %d, histograms = %d\n", i, histograms.size());
		}

		int runningTime = float( clock () - begin_time ) / CLOCKS_PER_SEC;

		// Show result dialog
		string strTmp = format("Result:\t Total time : %d seconds\n\tTotal : %d\n\tRight : %d\n\tWrong : %d\n\n\tLBPH(radius=%i, neighbors=%i, grid_x=%i, grid_y=%i, threshold=%.2f)",
			runningTime, numOfItems, numOfRightItems, numOfItems - numOfRightItems,
			model->getInt("radius"),
			model->getInt("neighbors"),
			model->getInt("grid_x"),
			model->getInt("grid_y"),
			model->getDouble("threshold"));

		MessageBox(strTmp.c_str(), NULL, 1);
	}
}


void CMainDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	string tmp = "1. Do Dang Minh\t\t13 11 015\n2. Huynh Cong Toan\t\t13 11 026\n3. Duong Xuan Long\t\t13 11 048\n4. Ho Van Tan\t\t13 11 058";
	MessageBox(tmp.c_str(), "Nhom 7 - CHCSTT - K23 DH KHTN");
}

