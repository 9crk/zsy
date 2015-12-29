
// ZSYDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ZSY.h"
#include "ZSYDlg.h"
#include "loginDlg.h"
#include "changeName.h"
#include "afxdialogex.h"
#include"cv.h"
#include <opencv2\opencv.hpp>
#include <highgui.h>
#include"CvvImage.h"
#include "MySocket.h"
#include "CvxText.h"

#pragma comment(lib,"freetype.lib")

#pragma comment(lib, "libjpeg.lib")
#pragma comment(lib, "opencv_core249.lib")
#pragma comment(lib, "opencv_highgui249.lib")
#pragma comment(lib, "opencv_imgproc249.lib")
#pragma comment(lib, "zlib.lib")
#pragma comment(lib, "libtiff.lib")
#pragma comment(lib, "IlmImf.lib")
#pragma comment(lib, "libjasper.lib")

#pragma comment(lib, "libpng.lib")


//#pragma comment(lib,"freetype.def")


IplImage *img2;
CMySocket mskt;
int isLineSafe[2];
FILE* fp,*fp_set;
int isConnected = 0;
int mm_gain;
#define TIMEOUT 3000
CString devName[9];
CvPoint devPos[9];
int gAntHight;
//libjpeg.lib
//opencv_calib3d249.lib
//opencv_core249.lib
//opencv_highgui249.lib
//opencv_imgproc249.lib
//zlib.lib
//libtiff.lib
//libpng.lib
//IlmImf.lib
//libjasper.lib

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


// CZSYDlg 对话框



CZSYDlg::CZSYDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CZSYDlg::IDD, pParent)
	, m_gain(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CZSYDlg::~CZSYDlg( /*=NULL*/)
{
	mskt.Close();
}

void CZSYDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_gain);
	DDV_MinMaxInt(pDX, m_gain, 0, 30);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ip);
}

BEGIN_MESSAGE_MAP(CZSYDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CZSYDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CZSYDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CZSYDlg::OnBnClickedButton4)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON5, &CZSYDlg::OnBnClickedButton5)
	ON_EN_CHANGE(IDC_EDIT1, &CZSYDlg::OnEnChangeEdit1)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CZSYDlg 消息处理程序


BOOL CZSYDlg::OnInitDialog()
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
	this->GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	this->GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	this->GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	this->GetDlgItem(IDC_DB)->EnableWindow(FALSE);
	this->GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	lock = 1;
	img2 = NULL;
	m_ip.SetAddress(192,168,1,178);
	for (int i = 0; i < 2; i++)isLineSafe[i] = 0;
	SetTimer(12,1000,NULL);//1s	
	mm_gain = m_gain;
	//KillTimer(int nIDEvent);

	//log文件
	if (!PathFileExists("C:\\log.txt")){

		fp = fopen("C:\\log.txt","wb");
		if (fp == NULL)MessageBox("请右击以管理员权限运行此程序");
		fclose(fp);
	}
	else{
		fp = fopen("C:\\log.txt", "a");
		if (fp == NULL){
			MessageBox("请右击以管理员权限运行此程序");
			fclose(fp);
			OnOK();
		}
		else{
			fclose(fp);
		}
	}
	if (!PathFileExists("C:\\gain_cfg.txt")){

		fp_set = fopen("C:\\gain_cfg.txt", "wb");
		
		if (fp_set == NULL)MessageBox("请右击以管理员权限运行此程序");
		else{
			char iName[20];
			for (int i = 0; i < 9; i++){
				sprintf(iName, "%d号天线", i + 1);
				devName[i].Format("%s", iName);
				fwrite(iName, strlen(iName), 1, fp_set);
				fwrite("\r\n",strlen("\r\n"),1,fp_set);
			}
		}
		fclose(fp_set);
	}
	else{
		fp_set = fopen("C:\\gain_cfg.txt", "rb");
		char iName[20];
		for (int i = 0; i < 9; i++){
			fgets(iName, 20, fp_set);
			iName[strlen(iName)-2] = '\0';
			devName[i].Format("%s", iName);
		}
		fclose(fp_set);
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CZSYDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
#define BOX 50

void drawCell(IplImage*img, int x, int y, char*iName)
{
	 
	CvFont font;
	double hScale = 1;
	double vScale = 1;
	int lineWidth = 1;
	CvScalar color2 = CV_RGB(192,192,192);
	CvScalar color = CV_RGB(157, 157, 157);
	CvScalar color3 = CV_RGB(0, 0, 255);
	int add = 0;
	char name[20];
	if (strcmp("G1", iName) == 0){
		add += 35;
		sprintf(name, "%ddB", mm_gain);
	}
	else{
		sprintf(name, "%s", iName);
	}
	
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, hScale, vScale, 0, lineWidth);
	cvPutText(img, name, cvPoint(x+BOX/2-20, y+BOX/2+10), &font, color3);  
	cvDrawRect(img, cvPoint(x, y), cvPoint(x + BOX+add, y + BOX), color2, 4, 8);
	cvDrawRect(img, cvPoint(x+1, y+1), cvPoint(x + BOX-1+add, y + BOX-1), color2, 1, 8);
	cvDrawRect(img, cvPoint(x+3, y+3), cvPoint(x + BOX-3+add, y + BOX-3), color, 2, 8);
}
void drawStatu(IplImage*img, int x, int y,int cl)
{
	CvFont font;
	double hScale = 1;
	double vScale = 1;
	int lineWidth = 2;
	CvScalar color2;
	CvScalar clear;
	clear = CV_RGB(240, 240, 240);
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, hScale, vScale, 1, lineWidth);
	if (cl == 0){
		color2 = CV_RGB(255, 0, 0);
		cvPutText(img, "LINE BROKEN", cvPoint(x, y), &font, clear);
		cvPutText(img, "NORMAL", cvPoint(x, y), &font, clear);
		cvPutText(img, "LINE SHORT", cvPoint(x, y), &font, clear);

		cvPutText(img, "LINE SHORT", cvPoint(x, y), &font, color2);
	}
	else if (cl == 1){
		color2 = CV_RGB(0, 255, 0);
		cvPutText(img, "LINE BROKEN", cvPoint(x, y), &font, clear);
		cvPutText(img, "LINE SHORT", cvPoint(x, y), &font, clear);
		cvPutText(img, "NORMAL", cvPoint(x, y), &font, clear);
		cvPutText(img, "NORMAL", cvPoint(x, y), &font, color2);
	}
	else{
		color2 = CV_RGB(128, 128, 128);
		cvPutText(img, "NORMAL", cvPoint(x, y), &font, clear);
		cvPutText(img, "LINE SHORT", cvPoint(x, y), &font, clear);
		cvPutText(img, "LINE BROKEN", cvPoint(x, y), &font, clear);

		cvPutText(img, "LINE BROKEN", cvPoint(x, y), &font, color2);
	}
	
	
}
void drawOnLine(IplImage*img, int x, int y, int cl)
{
	CvFont font;
	double hScale = 1;
	double vScale = 1;
	int lineWidth = 2;
	CvScalar color2;
	CvScalar clear;
	clear = CV_RGB(240, 240, 240);

	CvxText text("C:\\WINDOWS\\Fonts\\msyh.ttf");
	float p = 1.0;

	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, hScale, vScale, 1, lineWidth);
	if (cl == 1){
		color2 = CV_RGB(0, 255, 0);
		text.setFont(NULL, &clear, NULL, &p);   // 透明处理

		DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, "设备离线", -1, NULL, 0);
		wchar_t *pwText;
		pwText = new wchar_t[dwNum];
		MultiByteToWideChar(CP_ACP, 0, "设备离线", -1, pwText, dwNum);

		text.putText(img, pwText, cvPoint(x, y), CV_RGB(0, 0, 0));

		dwNum = MultiByteToWideChar(CP_ACP, 0, "设备在线", -1, NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, "设备在线", -1, pwText, dwNum);

		text.setFont(NULL, &color2, NULL, &p);   // 透明处理
		text.putText(img, pwText, cvPoint(x, y), CV_RGB(0, 255, 0));
delete pwText;
		//cvPutText(img, "OFF-LINE", cvPoint(x, y), &font, clear);
		//cvPutText(img, "ON-LINE", cvPoint(x, y), &font, color2);
	}
	else{
		color2 = CV_RGB(255, 0, 0);
		text.setFont(NULL, &clear, NULL, &p);   // 透明处理

		DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, "设备在线", -1, NULL, 0);
		wchar_t *pwText;
		pwText = new wchar_t[dwNum];
		MultiByteToWideChar(CP_ACP, 0, "设备在线", -1, pwText, dwNum);

		text.putText(img, pwText, cvPoint(x, y), CV_RGB(0, 0, 0));

		dwNum = MultiByteToWideChar(CP_ACP, 0, "设备离线", -1, NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, "设备离线", -1, pwText, dwNum);
		
		text.setFont(NULL, &color2, NULL, &p);   // 透明处理
		text.putText(img, pwText, cvPoint(x, y), CV_RGB(255, 0, 0));
	delete pwText;
		//cvPutText(img, "ON-LINE", cvPoint(x, y), &font, clear);
		//cvPutText(img, "OFF-LINE", cvPoint(x, y), &font, color2);
	}


}
void drawAnt(IplImage*img, int x, int y,int y2,int x3)
{
	CvScalar color = CV_RGB(192, 192, 192);
	CvScalar color2;
	if (isLineSafe[1] == 0){
		color2 = CV_RGB(255, 0, 0);
	}
	else if (isLineSafe[1] == 1){
		color2 = CV_RGB(0, 255, 0);
	}
	else{
		color2 = CV_RGB(128, 128, 128);
	}
/*	cvLine(img, cvPoint(x + BOX / 2, y), cvPoint(x + 10, y + BOX), color, 6, 8);
	cvLine(img, cvPoint(x+BOX/2, y), cvPoint(x + BOX-10, y + BOX), color, 6, 8);
	cvLine(img, cvPoint(x+10, y + BOX), cvPoint(x+BOX-10, y + BOX), color, 6, 8);
*/
	cvLine(img, cvPoint(x-10, y), cvPoint(x + BOX+10, y), color, 2, 8);
	cvLine(img, cvPoint(x -10, y), cvPoint(x + BOX/2, y + BOX), color, 2, 8);
	cvLine(img, cvPoint(x +BOX+ 10, y), cvPoint(x + BOX/2, y + BOX), color, 2, 8);

	cvLine(img2, cvPoint(x + BOX / 2, y + BOX), cvPoint(x+BOX/2 , y2), color2, 2, 8);
	cvLine(img2, cvPoint(x + BOX / 2, y2), cvPoint(x3, y2), color2, 2, 8);
}

void drawAng(IplImage*img, int x, int y)
{
	CvScalar color = CV_RGB(192, 192, 192);
/*	cvLine(img, cvPoint(x, y+15), cvPoint(x + BOX, y + BOX/2), color, 3, 8);
	cvLine(img, cvPoint(x, y+BOX-15), cvPoint(x + BOX, y + BOX/2), color, 3, 8);
	cvLine(img, cvPoint(x, y+15), cvPoint(x, y+BOX-15), color, 3, 8);
*/
	cvLine(img, cvPoint(x, y + BOX/2), cvPoint(x + BOX/2, y + BOX / 4), color, 2, 8);
	cvLine(img, cvPoint(x, y + BOX/2), cvPoint(x + BOX/2, y +BOX- BOX / 4), color, 2, 8);
	cvLine(img, cvPoint(x+BOX/2, y + BOX/4), cvPoint(x+BOX/2, y + BOX - BOX/4), color, 2, 8);
}
void drawConn(IplImage*img, int x1, int y1,int x2,int y2,int statu, char*name)
{
	CvFont font;
	double hScale = 1;
	double vScale = 1;
	int lineWidth = 2;

	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, hScale, vScale, 0, lineWidth);
	//cvPutText(img, name, cvPoint(x + 2, y + 25), &font, CV_RGB(255, 0, 0));

	CvPoint f1,f2,p1, p2;
	p1.x = (x2 - (x1 + BOX)) / 2 + x1 + BOX;
	if (y2 < y1){
		p1.y = y1 + BOX/4;
	}
	else{
		p1.y = y1 + BOX*3/4;
	}
	p2.x = p1.x;
	p2.y = y2 + BOX/2;
	
	f1.x = x1 + BOX+ 4;
	f1.y = p1.y;
	f2.x = x2-4;
	f2.y = p2.y;
	//CString xx;
	//xx.Format("p1.x=%d y=%d", p1.x, p1.y);
	//MessageBox(NULL,xx,NULL,IDOK);
	CvScalar sca;
	if (statu == 0){
		sca = CV_RGB(255, 0, 0);
	}
	else if(statu ==1){
		sca = CV_RGB(0, 255, 0);
	}
	else{
		sca = CV_RGB(128, 128, 128);
	}
	cvLine(img, f1, p1, sca, 2, 8);
	cvLine(img, p1, p2, sca, 2, 8);
	cvLine(img, p2, f2, sca, 2, 8);
}

void CZSYDlg::OnPaint()
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

	CDC* pDC = GetDlgItem(IDC_PIC1)->GetDC();        // 获得显示控件的 DC
	HDC hDC = pDC->GetSafeHdc();                // 获取 HDC(设备句柄) 来进行绘图操作

	CRect rect;
	GetDlgItem(IDC_PIC1)->GetClientRect(&rect);
	int rw = rect.right - rect.left;            // 求出图片控件的宽和高
	int rh = rect.bottom - rect.top;

	if(img2 == NULL)img2 = cvCreateImage(cvSize(rw, rh), 8, 3);
	cvRectangle(img2, cvPoint(0, 0), cvPoint(rw, rh), CV_RGB(240, 240, 240), -1);
	int antX, antY;
	int gpsX, gpsY;
	int cx[8], cy[8];
	int ax[9], ay[9];
//--------------------------------------------------------
	antX = rw * 1 / 12 - BOX / 2-30;
	antY = rh * 1 / 6-BOX/2;
	gpsX = rw * 2 / 12 - BOX / 2-40;
	gpsY = rh * 1 / 2 -BOX/2;
	
	drawAnt(img2, antX, antY, gpsY + BOX / 2,gpsX);
	
	drawCell(img2, gpsX, gpsY, "G1");
//------------------------------------------------------------
	cx[0] = rw *3/ 12 - BOX / 2;
	cy[0] = rh * 1 / 2-BOX/2;
	drawCell(img2, cx[0], cy[0], "C1");
	drawConn(img2, gpsX+30, gpsY, cx[0], cy[0], isLineSafe[0], "");

	cx[1] = rw * 4 / 12 - BOX / 2;
	cy[1] = rh * 1 / 5+4;
	drawCell(img2, cx[1], cy[1], "C2");
	drawConn(img2, cx[0], cy[0], cx[1], cy[1], isLineSafe[0], "");
	
	cx[2] = rw * 5 / 12 - BOX / 2;
	cy[2] = rh * 1 / 5-50;
	drawCell(img2, cx[2], cy[2], "C3");
	drawConn(img2, cx[1], cy[1], cx[2], cy[2], isLineSafe[0], "");

	cx[3] = rw * 5 / 12 - BOX / 2;
	cy[3] = rh * 3 / 5-10;
	drawCell(img2, cx[3], cy[3], "C4");
	drawConn(img2, cx[0], cy[0], cx[3], cy[3], isLineSafe[0], "");

	cx[4] = rw * 6 / 12 - BOX / 2;
	cy[4] = rh * 2 / 5;
	drawCell(img2, cx[4], cy[4], "C5");
	drawConn(img2, cx[3], cy[3], cx[4], cy[4], isLineSafe[0], "");

	cx[5] = rw * 6 / 12 - BOX / 2;
	cy[5] = rh * 4 / 6 ;
	drawCell(img2, cx[5], cy[5], "C6");
	drawConn(img2, cx[3], cy[3], cx[5], cy[5], isLineSafe[0], "");

	cx[6] = rw * 7 / 12 - BOX / 2;
	cy[6] = rh * 5 / 9+21;
	drawCell(img2, cx[6], cy[6], "C7");
	drawConn(img2, cx[5], cy[5], cx[6], cy[6], isLineSafe[0], "");

	cx[7] = rw * 7 / 12 - BOX / 2;
	cy[7] = rh * 7 /9 +10;
	drawCell(img2, cx[7], cy[7], "C8");
	drawConn(img2, cx[5], cy[5], cx[7], cy[7], isLineSafe[0], "");
//---------------------------------------------------------------------

	ax[0] = rw * 9/12;
	ay[0] = rh * 1 / 10 -BOX/2;
	drawAng(img2, ax[0], ay[0]);
	drawConn(img2, cx[2], cy[2], ax[0], ay[0], isLineSafe[0], "");

	ax[1] = rw * 9/12;
	ay[1] = rh * 2 / 10 - BOX / 2;
	drawAng(img2, ax[1], ay[1]);
	drawConn(img2, cx[2], cy[2], ax[1], ay[1], isLineSafe[0], "");

	ax[2] = rw * 9 / 12;
	ay[2] = rh * 3 / 10 - BOX / 2;
	drawAng(img2, ax[2], ay[2]);
	drawConn(img2, cx[1], cy[1], ax[2], ay[2], isLineSafe[0], "");

	ax[3] = rw * 9 / 12;
	ay[3] = rh * 4 / 10 - BOX / 2;
	drawAng(img2, ax[3], ay[3]);
	drawConn(img2, cx[4], cy[4], ax[3], ay[3], isLineSafe[0], "");

	ax[4] = rw * 9 / 12;
	ay[4] = rh * 5 / 10 - BOX / 2;
	drawAng(img2, ax[4], ay[4]);
	drawConn(img2, cx[4], cy[4], ax[4], ay[4], isLineSafe[0], "");

	ax[5] = rw * 9 / 12;
	ay[5] = rh * 6 / 10 - BOX / 2;
	drawAng(img2, ax[5], ay[5]);
	drawConn(img2, cx[6], cy[6], ax[5], ay[5], isLineSafe[0], "");

	ax[6] = rw * 9 / 12;
	ay[6] = rh * 7 / 10 - BOX / 2;
	drawAng(img2, ax[6], ay[6]);
	drawConn(img2, cx[6], cy[6], ax[6], ay[6], isLineSafe[0], "");

	ax[7] = rw * 9 / 12;
	ay[7] = rh * 8 / 10 - BOX / 2;
	drawAng(img2, ax[7], ay[7]);
	drawConn(img2, cx[7], cy[7], ax[7], ay[7], isLineSafe[0], "");

	ax[8] = rw * 9 / 12;
	ay[8] = rh * 9 / 10 - BOX / 2;
	drawAng(img2, ax[8], ay[8]);
	drawConn(img2, cx[7], cy[7], ax[8], ay[8], isLineSafe[0], "");
//------------------------------------------------------------------------
	drawStatu(img2, 300, 580, isLineSafe[0]);
	drawStatu(img2, 10, 580, isLineSafe[1]);
	drawOnLine(img2,10,30,isConnected);

	CvxText text("C:\\WINDOWS\\Fonts\\msyh.ttf");
	float p = 1.0;
	CvScalar clor;
	clor = CV_RGB(255, 0, 0);
	text.setFont(NULL, &clor, NULL, &p);   // 透明处理
	for (int i = 0; i < 9; i++){
		devPos[i].x = ax[i] + BOX / 2 + 20;
		devPos[i].y = ay[i] + BOX / 2 + 10;
		
		DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, LPCTSTR(devName[i]), -1, NULL, 0);
		wchar_t *pwText;
		pwText = new wchar_t[dwNum];
		MultiByteToWideChar(CP_ACP, 0, LPCTSTR(devName[i]), -1, pwText, dwNum);

		text.putText(img2, pwText/*LPCTSTR(devName[i])*/, devPos[i], CV_RGB(255, 0, 0));
		delete pwText;
	}

	gAntHight = rh / 10;

	CvvImage cimg;
	cimg.CopyOf(img2, 1);                            // 复制图片
	cimg.DrawToHDC(hDC, &rect);                // 将图片绘制到显示控件的指定区域内
 
	ReleaseDC(pDC);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CZSYDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

IplImage *img;
int writelog(char* str)
{
	CString xx;
	CTime tm;
	tm = CTime::GetCurrentTime();
	xx = tm.Format("%Y-%m-%d %X:");
	char buff[100];
	sprintf(buff, "%s%s\r\n", LPCTSTR(xx),str);
	fp = fopen("C:\\log.txt", "a");
	if (fp == NULL){
		MessageBox(NULL, "请右击以管理员权限运行此程序", NULL, IDOK);
		return -1;
	}
	else{
		fwrite(buff, strlen(buff), 1, fp);
		fclose(fp);
	}
	return 0;
}
void CZSYDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	loginDlg dlg;
	KillTimer(13);
	if (lock == 1){
		if (IDOK == dlg.DoModal()){
			lock = 0;
			if (-1 == writelog("登陆")){
				OnOK();
			};
		}
		else{
			lock = 1;
		}
	}
	else{
		lock = 1;
	}
	
	if (lock == 0){
		this->GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_DB)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_BUTTON1)->SetWindowText("锁定");
	}
	else{
		this->GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_DB)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_BUTTON1)->SetWindowText("登陆");
	}


}
 
	 
 
void CZSYDlg::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
		char buff[40],buff2[10];
		UpdateData(TRUE);
		if (m_gain <= 30 && m_gain >= 0){
			mm_gain = m_gain;
		
			sprintf(buff, "<setgain,%d>", 30 - m_gain);
			mskt.Send(buff, strlen(buff));
			memset(buff2, 0, 10);
			mskt.Receive(buff2, 10);

			
			if (strcmp(buff2, "<OK>") == 0){//注意这个换行符，可能是没有的
				if (-1 == writelog(buff)){
					OnOK();
				};
				MessageBox("OK！");
			}
			UpdateData(TRUE);
		}
		else{
			MessageBox("请输入0-30之间的数字");
		}
}




void CZSYDlg::OnBnClickedButton4()
{
	// TODO:  在此添加控件通知处理程序代码
	ShellExecute(this->m_hWnd, "open", "C:\\log.txt", "", "", SW_SHOW);
}


void CZSYDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
//	if (nIDEvent == mskt.m_nTimerID){
	//	MessageBox("LLLLLLLLL");
//	}
	if (nIDEvent == 12){
		OnPaint();
	}
	if (nIDEvent == 13){
		//查询
		mskt.Send("<check>", strlen("<check>"));
		char buff[20];
		memset(buff, 0, 20);

		int ret = mskt.Receive(buff,20);

		if (ret == -1){
			if (isConnected){
				if (-1 == writelog("所有设备离线！")){
					OnOK();
				};
			}
			for (int i = 0; i < 2; i++)isLineSafe[i] = 0;
			isConnected = 0;
		}
		else{
			sscanf(buff, "<%d,%d>", &isLineSafe[0], &isLineSafe[1]);
			char tmp[100];
			sprintf(tmp,"警报:%s",buff);
			if (-1 == writelog(tmp)){
				OnOK();
			};
		}
	}	
	CDialogEx::OnTimer(nIDEvent);
}

int bConnect = 0;
void CZSYDlg::OnBnClickedButton5()
{
	//test
#if 0
	CvxText text("wqy-zenhei.ttf");

	const char *msg = "在OpenCV中输出汉字！";

	float p = 1.0;
	CvScalar clor;
	clor = CV_RGB(255,0,0);
	text.setFont(NULL, &clor, NULL, &p);   // 透明处理
	text.putText(img2, msg, cvPoint(100, 150), CV_RGB(255, 0, 0));
#endif
	//end
	// TODO:  在此添加控件通知处理程序代码
	char ipbuf[100];
	UpdateData(TRUE);
	m_ip.GetWindowTextA(ipbuf, 100);
	if (bConnect == 1){
		bConnect = 0;
		this->GetDlgItem(IDC_BUTTON5)->SetWindowTextA("连接");
		mskt.Close();
		isLineSafe[0] = 0;
		isLineSafe[1] = 0;
		isConnected = 0;
		lock = 1;
		
			this->GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
			this->GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
			this->GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
			this->GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
			this->GetDlgItem(IDC_DB)->EnableWindow(FALSE);
			this->GetDlgItem(IDC_BUTTON1)->SetWindowText("登陆");
		
	}
	else{
		this->GetDlgItem(IDC_BUTTON5)->SetWindowTextA("连接中...");
		bConnect = 1;
	

		mskt.Create();
		BOOL a = mskt.Connect(ipbuf, 4001);

		if (a == FALSE){
			MessageBox("设备无反应！","警告");
			this->GetDlgItem(IDC_BUTTON5)->SetWindowTextA("连接");
			bConnect = 0;
		}
		else{
		
			this->GetDlgItem(IDC_BUTTON5)->SetWindowTextA("断开");
			mskt.Send("<readgain>", strlen("<readgain>"));
			char buff[200];
			memset(buff, 0, 200);

			int ret = mskt.Receive(buff, 10);

			if (ret == -1)MessageBox("模块没反应");
			else{
				char*p = buff;
				char tmp[10];
				memset(tmp, 0, 10);
				char*pt = tmp;
				p++;
				while (*p != '>'){
					if (*p == ',')return;
					*pt = *p;
					p++;
					pt++;
				}
				m_gain = atoi(tmp);
				m_gain = 30 - m_gain;
				mm_gain = m_gain;
				UpdateData(FALSE);
				isConnected = 1;
				SetTimer(13, 10000, NULL);

				this->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
				//check
				mskt.Send("<check>", strlen("<check>"));

				memset(buff, 0, 20);
				int ret = mskt.Receive(buff, 20);
				if (ret == -1){
					for (int i = 0; i < 2; i++)isLineSafe[i] = 0;
					isConnected = 0;
				}
				else{
					sscanf(buff, "<%d,%d>", &isLineSafe[0], &isLineSafe[1]);
					//for (int i = 0; i < 2; i++)isLineSafe[i] = 1;
				}
			}

		}
	}

}


void CZSYDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

char gDevName[100];

void CZSYDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	this->GetDlgItem(IDC_PIC1)->GetWindowRect(&rect);
	
//	CString xx;

	//xx.Format("pt:%d-%d left=%d-%d-%d-%d width =%d width=%d",point.x,point.y, rect.left,rect.right,rect.top,rect.bottom,img2->width,(rect.right-rect.left));
	//MessageBox(xx);
	if (point.x > rect.left + devPos[0].x - 3*BOX){
		int i = (point.y-gAntHight*2/3) / gAntHight;
		//CString xx;
		//xx.Format("%d", i);
		//MessageBox(xx);
		
		changeName dlg;
		sprintf(gDevName, "");
		dlg.DoModal();
		if (strcmp(gDevName, "") == 0){

		}
		else{
			devName[i].Format("%s", gDevName);
			fp_set = fopen("C:\\gain_cfg.txt", "wb");

			if (fp_set == NULL)MessageBox("请右击以管理员权限运行此程序");
			else{
				for (int i = 0; i < 9; i++){
					fwrite(LPCTSTR(devName[i]), devName[i].GetLength(), 1, fp_set);
					fwrite("\r\n",strlen("\r\n"),1,fp_set);
				}
			}
			fclose(fp_set);
		}
		
	}
	//GetWindowRect
	CDialogEx::OnLButtonDown(nFlags, point);
}


