// changeName.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZSY.h"
#include "changeName.h"
#include "afxdialogex.h"

extern char gDevName[100];
// changeName �Ի���

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


// changeName ��Ϣ�������


void changeName::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString xx;
	this->GetDlgItem(IDC_EDIT1)->GetWindowTextA(xx);
	sprintf(gDevName,"%s",LPCTSTR(xx));
	OnOK();
}


void changeName::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���
	CString xx;
	this->GetDlgItem(IDC_EDIT1)->GetWindowTextA(xx);
	sprintf(gDevName, "%s", LPCTSTR(xx));
	CDialogEx::OnOK();
}
