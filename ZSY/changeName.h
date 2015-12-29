#pragma once


// changeName 对话框

class changeName : public CDialogEx
{
	DECLARE_DYNAMIC(changeName)

public:
	changeName(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~changeName();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual void OnOK();
};
