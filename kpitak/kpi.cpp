
#include "stdafx.h"
#include "kpi.h"
#include "CTakDecoder.h"

char g_szIniFileName[MAX_PATH];

// 本当に使いたいデコーダクラス
typedef CTakDecoder CKpiTargetDecoder;

// kmp_GetTestModule() は kpitak.cpp に

void WINAPI kpiInit()
{
	char* pDot;

	GetModuleFileName(::g_hModule, ::g_szIniFileName, MAX_PATH);
	pDot = strrchr(g_szIniFileName, '.');
	strncpy_s(pDot, MAX_PATH - (pDot - g_szIniFileName), ".ini", 4);
}

void WINAPI kpiDeinit()
{
}

HKMP WINAPI kpiOpen(const char* cszFileName, SOUNDINFO* pInfo)
{
	CKpiTargetDecoder* d = new CKpiTargetDecoder();
	if (d->Open((LPSTR)cszFileName, pInfo))
		return (HKMP)d;
	return NULL;
}

void WINAPI kpiClose(HKMP hKMP)
{
	if (hKMP)
		delete (CKpiTargetDecoder*)hKMP;
}

DWORD WINAPI kpiRender(HKMP hKMP, BYTE* Buffer, DWORD dwSize)
{
	CKpiTargetDecoder* d = (CKpiTargetDecoder*)hKMP;
	if (d)
		return d->Render(Buffer, dwSize);
	return 0;
}

DWORD WINAPI kpiSetPosition(HKMP hKMP, DWORD dwPos)
{
	CKpiTargetDecoder* d = (CKpiTargetDecoder*)hKMP;
	if (d)
		return d->SetPosition(dwPos);
	return 0;
}

UINT GetMyProfileInt(LPSTR szSectionName, LPSTR szKeyName, INT nDefault)
{
	return ::GetPrivateProfileInt(szSectionName, szKeyName, nDefault, ::g_szIniFileName);
}

BOOL WINAPI kpiGetTagInfo(const char *cszFileName, IKmpTagInfo *pTagInfo)
{
	return CKpiTargetDecoder::GetTagInfo(cszFileName, pTagInfo);
}
