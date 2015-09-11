/* CTakDecoder -- interface to tak_deco_lib.dll for KbMedia Player

Copyright (c) 2008, Autch.

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you
	must not claim that you wrote the original software. If you use
	this software in a product, an acknowledgment in the product
	documentation would be appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and
	must not be misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	distribution.
*/

#pragma once

#include "CAbstractKpi.h"

class CTakDecoder: public CAbstractKpi
{
private:
	// �f�R�[�_�ւ̈���
	TtakSSDOptions                m_Options;
	// �f�R�[�_�̃C���X�^���X
	TtakSeekableStreamDecoder     m_Decoder;
	// �J�����X�g���[���̏��
	Ttak_str_StreamInfo           m_StreamInfo;
	// KMP �̋ȏ��
	SOUNDINFO                     m_SoundInfo;

public:
	CTakDecoder();
	~CTakDecoder();

	void Close();
	BOOL Open(LPSTR szFileName, SOUNDINFO* pInfo);
	DWORD SetPosition(DWORD dwPos);
	DWORD Render(BYTE* pBuffer, DWORD dwSize);

	static BOOL GetTagInfo(const char *cszFileName, IKmpTagInfo *pInfo);
};
