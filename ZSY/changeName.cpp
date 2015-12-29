// changeName.cpp : 实现文件
//

#include "stdafx.h"
#include "ZSY.h"
#include "changeName.h"
#include "afxdialogex.h"

extern char gDevName[100];
// changeName 对话框

IMPLEMENT_DYNAMIC(changeName, CDialogEx)

changeName::changeName(CWnd* pParent /*=NULL*/)
	: CDialogEx(changeName::IDD, pParent)
{

}

changeName::~changeName()
{
}

void changeName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(changeName, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &changeName::OnBnClickedButton1)
END_MESSAGE_MAP()


// changeName 消息处理程序


void changeName::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	CString xx;
	this->GetDlgItem(IDC_EDIT1)->GetWindowTextA(xx);
	sprintf(gDevName,"%s",LPCTSTR(xx));
	OnOK();
}


void changeName::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
	CString xx;
	this->GetDlgItem(IDC_EDIT1)->GetWindowTextA(xx);
	sprintf(gDevName, "%s", LPCTSTR(xx));
	CDialogEx::OnOK();
}
