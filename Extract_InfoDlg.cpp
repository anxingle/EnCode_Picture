
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
	, code_info(_T(""))
	
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_font.CreatePointFont(330,"楷体");
}

void CExtract_InfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT4, code_info);
}

BEGIN_MESSAGE_MAP(CExtract_InfoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OpenPic, &CExtract_InfoDlg::OnBnClickedOpenpic)
	ON_BN_CLICKED(IDC_ExtractInfo, &CExtract_InfoDlg::OnBnClickedExtractinfo)
	ON_BN_CLICKED(IDC_BUTTON1, &CExtract_InfoDlg::EnCode2)
	ON_BN_CLICKED(IDC_BUTTON2, &CExtract_InfoDlg::DeCode)
	ON_WM_CTLCOLOR()
//	ON_STN_CLICKED(IDC_Title, &CExtract_InfoDlg::OnStnClickedTitle)
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
		AfxMessageBox(_T("看看说明！你不知道要先选择图片吗？"));
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
	int width = sourceImg.cols;//图片宽度
	int height = sourceImg.rows;//图片高度
	int channle = (&IplImage(sourceImg))->nChannels;//图片通道
	int ori = (&IplImage(sourceImg))->origin;//图片信息起始点
	char temp1[8];
	sprintf_s(temp1, "%d", width);
	char temp2[8];
	sprintf_s(temp2, "%d", height);
	char size[25];
	sprintf_s(size, "%s * %s", temp1, temp2);
	GetDlgItem(IDC_EDIT1)->SetWindowText(size);
	char chan_str[8];
	char ori_str[8];
	sprintf_s(chan_str,"%d",channle);
	sprintf_s(ori_str,"%d",ori);
	GetDlgItem(IDC_EDIT2)->SetWindowText(ori_str);
	GetDlgItem(IDC_EDIT5)->SetWindowText(chan_str);
	// AfxMessageBox(size);
	/*IDC_EDIT5
	if (Edit_ID >= IDC_EDIT1 && Edit_ID <= IDC_EDIT9)
	GetDlgItem(Edit_ID)->SetWindowText(str);
	*/
}

//加密信息
void CExtract_InfoDlg::EnCode2()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//char info[40];
	int fnlen = code_info.GetLength(); //strcspn(str,".");
	char* codefile = new char[80];
	codefile = (LPSTR)(LPCTSTR)code_info;//正确，想办法去掉后缀
	
	//得到明文之后，要存入图片
	uchar code2num[80];
	for (int loop = 0; loop < 80; loop++)
	{
		code2num[loop] = codefile[loop];
	}
	IplImage * img = cvLoadImage(mPath, -1);//读入图像
	int count = 0;
	//for (int y = 0; y<img->height; y++) {
	for (int y = 1; y < 2; y++)
	{
		uchar* ptr = (uchar*)(img->imageData + y * img->widthStep/(sizeof(uchar)));
		//for (int x = img->width - 12; x < img->width - 1; x++){
		for (int x = 0; x < 80 ; x++){
    		ptr[x] = code2num[count++];
			//for (int x = 0; x<img->width; x++) {
			//ptr[3 * x + 1] = 255;
			//ptr[3 * x + 2] = 255;
		}
	}
	//ShowImage(img, IDC_SHOW1);
	TCHAR szFilter[] = _T("BMP文件(*.bmp)|*.bmp|PNG文件(*.png)|*.gif|所有文件(*.*)|*.*||");
	// 构造保存文件对话框   
	CFileDialog fileDlg(FALSE, _T("bmp"), _T("001"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString strFilePath;

	// 显示保存文件对话框   
	if (IDOK == fileDlg.DoModal())
	{
		// 如果点击了文件对话框上的“保存”按钮，则将选择的文件路径显示到编辑框里   
		strFilePath = fileDlg.GetPathName();
		//SetDlgItemText(IDC_SAVE_EDIT, strFilePath);
	}
	cvSaveImage(strFilePath,img);
	AfxMessageBox("加密完成！ copy right@安兴乐");
	UpdateData(FALSE);
}



//解密
void CExtract_InfoDlg::DeCode()
{
	
	char* codefile = new char[84];
	//codefile = (LPSTR)(LPCTSTR)code_info;//正确，想办法去掉后缀
	
	//AfxMessageBox(codefile);
	//得到明文之后，要存入图片
	uchar code2num[80];
	
	IplImage * img2 = cvLoadImage((LPCSTR)mPath, -1);//读入图像
	int count = 0;
	for (int loop = 0; loop < 80; loop++)
	{
		code2num[loop] ='0';
		codefile[loop] = '0';
	}
	codefile[79] = '\0';
	//for (int y = 0; y<img->height; y++) {
	for (int y =1; y< 2; y++) 
	{
		uchar* ptr2 = (uchar*)(img2->imageData + y * img2->widthStep/(sizeof(uchar)));
		for (int x = 0; x < 80; x++)
		{
//			ptr[3 * x + 1] = code2num[count++];
			code2num[count++] = ptr2[x];
		}
	}

	/*
	for (int y = 1; y < 2; y++)
	{
		uchar* ptr = (uchar*)(img->imageData + y * img->widthStep);
		//for (int x = img->width - 12; x < img->width - 1; x++){
		for (int x = 1; x < 11 ; x++){
    		ptr[x] = code2num[count++];
			//for (int x = 0; x<img->width; x++) {
			//ptr[3 * x + 1] = 255;
			//ptr[3 * x + 2] = 255;
		}
	
	*/
	for (int loop = 0; loop < 80; loop++)
	{
		codefile[loop] = code2num[loop];
	}

	GetDlgItem(IDC_EDIT3)->SetWindowText(codefile);
}


HBRUSH CExtract_InfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_Title)
	{
		pDC->SelectObject(&m_font);
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


//void CExtract_InfoDlg::OnStnClickedTitle()
//{
//	// TODO: Add your control notification handler code here
//}
