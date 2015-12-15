
// Extract_InfoDlg.cpp : 实现文件
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CExtract_InfoDlg 对话框



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


// CExtract_InfoDlg 消息处理程序

BOOL CExtract_InfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	//初始化图片缩放大小
	Pic_Size = 300;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CExtract_InfoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CExtract_InfoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*
void CProPosalDlg::ShowImage(IplImage* img, unsigned int ID, unsigned int Edit_ID, CString str)
{
	if (!img)      // 判断是否成功读取图片,若TheImage==NULL则返回，不执行下面的操作
	{
		AfxMessageBox(_T("读取图片失败"));
		return;
	}
	CDC* pDC = GetDlgItem(ID)->GetDC();   // 获得显示控件的 DC
	HDC hDC = pDC->GetSafeHdc();			 // 获取 HDC(设备句柄) 来进行绘图操作	

	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	int ID_size = rect.bottom>rect.right ? rect.right : rect.bottom;//检查Picture控件的尺寸
	//int b=rect.right;
	pDC->FillSolidRect(&rect, RGB(240, 240, 240));//加一个对话框颜色填充，不然多张图片会有地方一直出现
	int iw = img->width;
	int ih = img->height;
	if (iw >= ih)
		ratio = ((float)iw) / ID_size;
	else
		ratio = ((float)ih) / ID_size;
	SetRect(rect, 0, 0, floor((float)iw / ratio), floor((float)ih / ratio));//改变rect大小		


	CvvImage cimg;
	cimg.CopyOf(img);
	cimg.DrawToHDC(hDC, &rect);	// 将图片绘制到显示控件的指定区域内
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
	if (!img)      // 判断是否成功读取图片,若TheImage==NULL则返回，不执行下面的操作
	{
		AfxMessageBox(_T("读取图片失败"));
		return;
	}
	CDC* pDC = GetDlgItem(ID)->GetDC();   // 获得显示控件的 DC
	HDC hDC = pDC->GetSafeHdc();			 // 获取 HDC(设备句柄) 来进行绘图操作	

	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	int ID_size = rect.bottom > rect.right ? rect.right : rect.bottom;//检查Picture控件的尺寸
	//int b=rect.right;
	pDC->FillSolidRect(&rect, RGB(240, 240, 240));//加一个对话框颜色填充，不然多张图片会有地方一直出现
	int iw = img->width;
	int ih = img->height;
	if (iw >= ih)
		ratio = ((float)iw) / ID_size;
	else
		ratio = ((float)ih) / ID_size;
	SetRect(rect, 0, 0, floor((float)iw / ratio), floor((float)ih / ratio));//改变rect大小		


	CvvImage cimg;
	cimg.CopyOf(img);
	cimg.DrawToHDC(hDC, &rect);	// 将图片绘制到显示控件的指定区域内

	//提取信息
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
	dlg.m_ofn.lpstrTitle = _T("请选择一幅待检索的图像");// 打开文件对话框的标题名

	if (dlg.DoModal() != IDOK)             // 判断是否获得图片
	{
		AfxMessageBox(_T("请选择合适的文件路径"));
		return;
	}

	mPath = dlg.GetPathName();	    // 获取图片路径
	// 路径转化为 char*
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
	//mPath(mPath_str)即为这种路径：("E://Programs/Images/Lena.jpg" )  

	if (sourceImg.empty())      // 判断是否成功读取图片,若TheImage==NULL则返回，不执行下面的操作
	{
		AfxMessageBox(_T(mPath));
		AfxMessageBox(_T("读取图片失败"));
		return;
	}

	cv::Mat Img = cv::Mat(zoomImgByMaxSide(&IplImage(sourceImg), Pic_Size), false);//将原图长边压缩至图片控件边长Pic_Size

	ShowImage(&IplImage(Img), IDC_SHOW1);        // 调用显示图片函数
	

}

//提取图片相关信息
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
