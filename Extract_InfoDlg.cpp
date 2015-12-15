
// Extract_InfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Extract_Info.h"
#include "Extract_InfoDlg.h"
#include "afxdialogex.h"
#include "CvvImage.h"
#include <opencv2/opencv.hpp>
#include "Preprocess.h"
#include <string>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CExtract_InfoDlg �Ի���



CExtract_InfoDlg::CExtract_InfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExtract_InfoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExtract_InfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CExtract_InfoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OpenPic, &CExtract_InfoDlg::OnBnClickedOpenpic)
	ON_BN_CLICKED(IDC_ExtractInfo, &CExtract_InfoDlg::OnBnClickedExtractinfo)
END_MESSAGE_MAP()


// CExtract_InfoDlg ��Ϣ�������

BOOL CExtract_InfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	//��ʼ��ͼƬ���Ŵ�С
	Pic_Size = 300;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CExtract_InfoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CExtract_InfoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CExtract_InfoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*
void CProPosalDlg::ShowImage(IplImage* img, unsigned int ID, unsigned int Edit_ID, CString str)
{
	if (!img)      // �ж��Ƿ�ɹ���ȡͼƬ,��TheImage==NULL�򷵻أ���ִ������Ĳ���
	{
		AfxMessageBox(_T("��ȡͼƬʧ��"));
		return;
	}
	CDC* pDC = GetDlgItem(ID)->GetDC();   // �����ʾ�ؼ��� DC
	HDC hDC = pDC->GetSafeHdc();			 // ��ȡ HDC(�豸���) �����л�ͼ����	

	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	int ID_size = rect.bottom>rect.right ? rect.right : rect.bottom;//���Picture�ؼ��ĳߴ�
	//int b=rect.right;
	pDC->FillSolidRect(&rect, RGB(240, 240, 240));//��һ���Ի�����ɫ��䣬��Ȼ����ͼƬ���еط�һֱ����
	int iw = img->width;
	int ih = img->height;
	if (iw >= ih)
		ratio = ((float)iw) / ID_size;
	else
		ratio = ((float)ih) / ID_size;
	SetRect(rect, 0, 0, floor((float)iw / ratio), floor((float)ih / ratio));//�ı�rect��С		


	CvvImage cimg;
	cimg.CopyOf(img);
	cimg.DrawToHDC(hDC, &rect);	// ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������
	if (feaflags == DENSESURF_PRO_VLAD)
	{
		if (Edit_ID == IDC_EDIT1 || Edit_ID == IDC_EDIT4 || Edit_ID == IDC_EDIT7)
			GetDlgItem(Edit_ID)->SetWindowText(str);
		else
		{
			if (Edit_ID >= IDC_EDIT1 && Edit_ID <= IDC_EDIT9)
				GetDlgItem(Edit_ID)->SetWindowTextA("              ");
		}
	}
	else
	{
		if (Edit_ID >= IDC_EDIT1 && Edit_ID <= IDC_EDIT9)
			GetDlgItem(Edit_ID)->SetWindowText(str);
	}
	ReleaseDC(pDC);

}
*/

void CExtract_InfoDlg::ShowImage(IplImage* img, unsigned int ID)
{
	if (!img)      // �ж��Ƿ�ɹ���ȡͼƬ,��TheImage==NULL�򷵻أ���ִ������Ĳ���
	{
		AfxMessageBox(_T("��ȡͼƬʧ��"));
		return;
	}
	CDC* pDC = GetDlgItem(ID)->GetDC();   // �����ʾ�ؼ��� DC
	HDC hDC = pDC->GetSafeHdc();			 // ��ȡ HDC(�豸���) �����л�ͼ����	

	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	int ID_size = rect.bottom > rect.right ? rect.right : rect.bottom;//���Picture�ؼ��ĳߴ�
	//int b=rect.right;
	pDC->FillSolidRect(&rect, RGB(240, 240, 240));//��һ���Ի�����ɫ��䣬��Ȼ����ͼƬ���еط�һֱ����
	int iw = img->width;
	int ih = img->height;
	if (iw >= ih)
		ratio = ((float)iw) / ID_size;
	else
		ratio = ((float)ih) / ID_size;
	SetRect(rect, 0, 0, floor((float)iw / ratio), floor((float)ih / ratio));//�ı�rect��С		


	CvvImage cimg;
	cimg.CopyOf(img);
	cimg.DrawToHDC(hDC, &rect);	// ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������

	//��ȡ��Ϣ
	/*
	if (Edit_ID >= IDC_EDIT1 && Edit_ID <= IDC_EDIT9)
		GetDlgItem(Edit_ID)->SetWindowText(str);
		*/
	ReleaseDC(pDC);
}


void CExtract_InfoDlg::OnBnClickedOpenpic()
{
	CFileDialog dlg(
		TRUE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("All Files (*.*) |*.*||"), NULL
		);
	dlg.m_ofn.lpstrTitle = _T("��ѡ��һ����������ͼ��");// ���ļ��Ի���ı�����

	if (dlg.DoModal() != IDOK)             // �ж��Ƿ���ͼƬ
	{
		AfxMessageBox(_T("��ѡ����ʵ��ļ�·��"));
		return;
	}

	mPath = dlg.GetPathName();	    // ��ȡͼƬ·��
	// ·��ת��Ϊ char*
	/*
	const size_t strsize = (mPath.GetLength() + 1) * 2;
	char *mPath_str = new char[strsize];
	size_t sz = 0;
	wcstombs_s(&sz, mPath_str, strsize, mPath, _TRUNCATE);
	*/

	sourceImg = cv::imread((LPCSTR)mPath);
	//sourceImg = cv::imread("K:\\anxingle\\anxingle.jpg");
	//sourceImg = cv::Mat(cvLoadImage((LPCSTR)mPath));

	//sourceImg = cv::imread(mPath_str.GetBuffer());
	//mPath(mPath_str)��Ϊ����·����("E://Programs/Images/Lena.jpg" )  

	if (sourceImg.empty())      // �ж��Ƿ�ɹ���ȡͼƬ,��TheImage==NULL�򷵻أ���ִ������Ĳ���
	{
		AfxMessageBox(_T(mPath));
		AfxMessageBox(_T("��ȡͼƬʧ��"));
		return;
	}

	cv::Mat Img = cv::Mat(zoomImgByMaxSide(&IplImage(sourceImg), Pic_Size), false);//��ԭͼ����ѹ����ͼƬ�ؼ��߳�Pic_Size

	ShowImage(&IplImage(Img), IDC_SHOW1);        // ������ʾͼƬ����
	

}

//��ȡͼƬ�����Ϣ
void CExtract_InfoDlg::OnBnClickedExtractinfo()
{
	// TODO: Add your control notification handler code here
	int width = sourceImg.cols;
	int height = sourceImg.rows;
	char temp1[8];
	sprintf_s(temp1, "%d", width);
	char temp2[8];
	sprintf_s(temp2,"%d");
	//AfxMessageBox(temp);
	/*
	if (Edit_ID >= IDC_EDIT1 && Edit_ID <= IDC_EDIT9)
	GetDlgItem(Edit_ID)->SetWindowText(str);
	*/
}
