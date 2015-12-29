// loginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZSY.h"
#include "loginDlg.h"
#include "afxdialogex.h"
#include "ZSYDlg.h"
#include "MySocket.h"

// loginDlg �Ի���
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


// loginDlg ��Ϣ�������

extern CMySocket mskt;

void loginDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//TCP��½��֤
	//��֤�ɹ�
	char buff[200];
	CString xx;
	this->GetDlgItemText(IDC_EDIT1, xx);

	sprintf(buff,"<login,%s>",LPCTSTR(xx));
	mskt.Send(buff, strlen(buff));
	memset(buff, 0, 200);
	mskt.Receive(buff, 10);
	
	if (buff[1] == 'O'){//ע��������з���������û�е�
		OnOK();
	}
	//if (strcmp(buff,"<OK>")==0){//ע��������з���������û�е�
//		OnOK();
	//}
	else{
		MessageBox("Wrong Password!","TenChar Notice");
		//OnCancel();
	}
}
