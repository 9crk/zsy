#pragma once


// loginDlg �Ի���

class loginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(loginDlg)

public:
	loginDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~loginDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString m_password;
};
