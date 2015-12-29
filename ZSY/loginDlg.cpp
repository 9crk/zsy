// loginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ZSY.h"
#include "loginDlg.h"
#include "afxdialogex.h"
#include "ZSYDlg.h"
#include "MySocket.h"

// loginDlg 对话框
#define TIMEOUT 3000
IMPLEMENT_DYNAMIC(loginDlg, CDialogEx)

loginDlg::loginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(loginDlg::IDD, pParent)
	, m_password(_T(""))
{

}

loginDlg::~loginDlg()
{
}

void loginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_password);
}


BEGIN_MESSAGE_MAP(loginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &loginDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// loginDlg 消息处理程序

extern CMySocket mskt;

void loginDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	//TCP登陆认证
	//认证成功
	char buff[200];
	CString xx;
	this->GetDlgItemText(IDC_EDIT1, xx);

	sprintf(buff,"<login,%s>",LPCTSTR(xx));
	mskt.Send(buff, strlen(buff));
	memset(buff, 0, 200);
	mskt.Receive(buff, 10);
	
	if (buff[1] == 'O'){//注意这个换行符，可能是没有的
		OnOK();
	}
	//if (strcmp(buff,"<OK>")==0){//注意这个换行符，可能是没有的
//		OnOK();
	//}
	else{
		MessageBox("Wrong Password!","TenChar Notice");
		//OnCancel();
	}
}
