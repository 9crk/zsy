
// ZSYDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CZSYDlg �Ի���
class CZSYDlg : public CDialogEx
{
// ����
public:
	CZSYDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CZSYDlg();
	int lock;

// �Ի�������
	enum { IDD = IDD_ZSY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();


	int m_gain;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CIPAddressCtrl m_ip;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
