#pragma once
#include "afxsock.h"

class CMySocket :
	public CSocket
{
public:
	CMySocket();
	~CMySocket();
//	virtual BOOL OnMessagePending();
	void OnReceive(int nErrorCode);
//	int bReceive(void* lpBuf, int nBufLen, int timeout);
//	int bSend(const void* lpBuf, int nBufLen, int timeout);
//	BOOL bConnect(char* addr, int port, int timeout);
//	int m_nTimerID;
protected:
	
//	BOOL SetTimeOut(UINT uTimeOut);
//	BOOL KillTimeOut();
private:
	
};

