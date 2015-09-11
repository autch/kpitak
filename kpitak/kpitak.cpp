// kpitak.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "kpi.h"

#define KPITAK_VERSION   ((1 << 8) | 2)      // 1.02
#ifdef _DEBUG
#define KPITAK_DESC      "TAK audio decoder for KbMedia Player [DEBUG]"
#else
#define KPITAK_DESC      "TAK audio decoder for KbMedia Player"
#endif
#define KPITAK_COPYRIGHT "Copyright (c) 2008, 2015 Autch.  TAK Copyright 2007 by Thomas Becker, D-49080 Osnabr\"uck.  All rights reserved."

extern "C"
KMPMODULE* APIENTRY kmp_GetTestModule()
{
	static const char* pszExts[] = { ".tak", NULL };
	static KMPMODULE kpiModule =
	{
		  KMPMODULE_VERSION,				// DWORD dwVersion;
		  KPITAK_VERSION,    			  // DWORD dwPluginVersion;
		  KPITAK_COPYRIGHT, 		    // const char	*pszCopyright;
		  KPITAK_DESC,							// const char	*pszDescription;
		  pszExts,									// const char	**ppszSupportExts;
		  1,												// DWORD dwReentrant;
		  kpiInit,									// void (WINAPI *Init)(void);
		  kpiDeinit,								// void (WINAPI *Deinit)(void);
		  kpiOpen,									// HKMP (WINAPI *Open)(const char *cszFileName, SOUNDINFO *pInfo);
		  NULL,	            				// HKMP (WINAPI *OpenFromBuffer)(const BYTE *Buffer, DWORD dwSize, SOUNDINFO *pInfo);
		  kpiClose,									// void (WINAPI *Close)(HKMP hKMP);
		  kpiRender,								// DWORD (WINAPI *Render)(HKMP hKMP, BYTE* Buffer, DWORD dwSize);
		  kpiSetPosition						// DWORD (WINAPI *SetPosition)(HKMP hKMP, DWORD dwPos);
	};
	return &kpiModule;
}

extern "C"
BOOL WINAPI kmp_GetTestTagInfo(const char *cszFileName, IKmpTagInfo *pTagInfo)
{
	return kpiGetTagInfo(cszFileName, pTagInfo);
}
