#include "stdafx.h"
#include "MySocket.h"


CMySocket::CMySocket()
{
}


CMySocket::~CMySocket()
{
}
/*
BOOL CMySocket::OnMessagePending()
{
	
	MSG msg;
	if (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
	{
		if (msg.wParam == (UINT)m_nTimerID)
		{
			MessageBox(NULL, "xxxxxx", "NULL", IDOK);
			// Remove the message and call CancelBlockingCall.
			::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_REMOVE);
			CancelBlockingCall();
			return FALSE;  // No need for idle time processing.
		};
	};

	return CSocket::OnMessagePending();
}

BOOL CMySocket::SetTimeOut(UINT uTimeOut)
{
	m_nTimerID = SetTimer(NULL, 0, uTimeOut, NULL);
	return m_nTimerID;
}

BOOL CMySocket::KillTimeOut()
{
	
	return KillTimer(NULL, m_nTimerID);
}*/
extern int isLineSafe[2];
extern int writelog(char* str);
void CMySocket::OnReceive(int nErrorCode)
{
	//MessageBox(NULL, "6666666666", NULL, IDOK);
	//m_pDoc->ProcessPendingRead();
	AsyncSelect(FD_READ); //����һ���������������¼�
	//CSocket::OnReceive(nErrorCode);
	
	char buff[20];
	int ret =Receive(buff,10);
	
	sscanf(buff, "<%d,%d>", &isLineSafe[0], &isLineSafe[1]);
	char tmp[100];
	switch (isLineSafe[0])
	{
	case 0:
		sprintf(tmp, "����:��·��·,");
		break;
	case 1:
		sprintf(tmp, "����:��·����,");
		break;
	case 2:
		sprintf(tmp, "����:��·�Ͽ�,");
		break;
	default:;
	}
	switch (isLineSafe[1])
	{
	case 0:
		sprintf(tmp, "%s���߶�·", tmp);
		break;
	case 1:
		sprintf(tmp, "%s��������", tmp);
		break;
	case 2:
		sprintf(tmp, "%s���߶Ͽ�", tmp);
		break;
	default:;
	}
	if (-1 == writelog(tmp)){
		;
	};
//	CString xx;
	//xx.Format("%d %d", addr, statu);
	//isLineSafe[addr] = statu;//0 ��· 1 ��ȫ 2 �Ͽ�
	//MessageBox(NULL,xx,NULL,IDOK);
}
/*
int CMySocket::bReceive(void* lpBuf, int nBufLen,int timeout)
{
	SetTimeOut(timeout);
	int nRecv = CSocket::Receive(lpBuf, nBufLen);
	KillTimeOut();
	return nRecv;
}

int CMySocket::bSend(const void* lpBuf, int nBufLen,int timeout)
{
	SetTimeOut(timeout);
	int nSend = CSocket::Send(lpBuf, nBufLen);
	KillTimeOut();
	return nSend;
}
BOOL CMySocket::bConnect(char* addr,int port,int timeout)//Connect(constvoid* lpBuf, int nBufLen, int nFlags, int timeout)
{
	SetTimeOut(timeout);
	BOOL xx = CSocket::Connect(addr, port);
	KillTimeOut();
	return xx;
}*/