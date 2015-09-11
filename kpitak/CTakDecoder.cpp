#include "stdafx.h"
#include "CTakDecoder.h"

CTakDecoder::CTakDecoder()
{
	m_Options.Cpu = tak_Cpu_Any;
	m_Options.Flags = 0;

	m_Decoder = NULL;
}

CTakDecoder::~CTakDecoder()
{
	Close();
}

void CTakDecoder::Close()
{
	if (m_Decoder != NULL)
	{
		tak_SSD_Destroy(m_Decoder);
		m_Decoder = NULL;
	}
}

BOOL CTakDecoder::Open(LPSTR szFileName, SOUNDINFO* pInfo)
{
	m_Decoder = tak_SSD_Create_FromFile(szFileName, &m_Options, NULL, NULL);
	if (m_Decoder == NULL || tak_SSD_Valid(m_Decoder) != tak_True)
		return FALSE;
	if (tak_SSD_GetStreamInfo(m_Decoder, &m_StreamInfo) != tak_res_Ok)
	{
		Close();
		return FALSE;
	}
	pInfo->dwBitsPerSample = m_StreamInfo.Audio.SampleBits;
	pInfo->dwChannels = m_StreamInfo.Audio.ChannelNum;
	pInfo->dwLength = (DWORD)(1000 * m_StreamInfo.Sizes.SampleNum / m_StreamInfo.Audio.SampleRate);
	pInfo->dwReserved1 = 0;
	pInfo->dwReserved2 = 0;
	pInfo->dwSamplesPerSec = m_StreamInfo.Audio.SampleRate;
	pInfo->dwSeekable = 1;
	pInfo->dwUnitRender = m_StreamInfo.Sizes.FrameSizeInSamples * m_StreamInfo.Audio.BlockSize;

	m_SoundInfo = *pInfo;

	return TRUE;
}

DWORD CTakDecoder::SetPosition(DWORD dwPos)
{
	TtakResult r;
	TtakInt64 qSeekTo = dwPos;
	TtakInt64 qSeekedTo;

	if (!m_Decoder || tak_SSD_Valid(m_Decoder) != tak_True)
		return 0;

	qSeekTo *= m_SoundInfo.dwSamplesPerSec;
	qSeekTo /= 1000;

	// シーク実行
	r = tak_SSD_Seek(m_Decoder, qSeekTo);
	if (r != tak_res_Ok && r != tak_res_ssd_FrameDamaged)
		return 0;

	// シーク後の位置を求める
	qSeekedTo = tak_SSD_GetReadPos(m_Decoder);
	if (qSeekedTo == -1) return 0;

	qSeekedTo *= 1000;
	qSeekedTo /= m_SoundInfo.dwSamplesPerSec;
	return (DWORD)qSeekedTo;
}

DWORD CTakDecoder::Render(BYTE* pBuffer, DWORD dwSize)
{
	TtakResult r;
	int nSamplesRead = 0;

	if (!m_Decoder || tak_SSD_Valid(m_Decoder) != tak_True)
		return 0;

	r = tak_SSD_ReadAudio(m_Decoder, pBuffer, m_StreamInfo.Sizes.FrameSizeInSamples, &nSamplesRead);
	if (r == tak_res_Ok || r == tak_res_ssd_FrameDamaged)
		return nSamplesRead * m_StreamInfo.Audio.BlockSize;
	else
		return 0;
}

BOOL CTakDecoder::GetTagInfo(const char *cszFileName, IKmpTagInfo *pInfo)
{
	TtakSeekableStreamDecoder decoder;
	TtakSSDOptions options = { tak_Cpu_Any, 0 };
	TtakAPEv2Tag apetag;

	decoder = tak_SSD_Create_FromFile(cszFileName, &options, NULL, NULL);
	if (decoder == NULL || tak_SSD_Valid(decoder) != tak_True)
		return FALSE;

	apetag = tak_SSD_GetAPEv2Tag(decoder);
	if (apetag == NULL || tak_APE_Valid(apetag) != tak_True)
	{
		tak_SSD_Destroy(decoder);
		return FALSE;
	}

	int itemCount = tak_APE_GetItemNum(apetag);
	for (int idx = 0; idx < itemCount; idx++)
	{
		TtakAPEv2ItemDesc itemDesc;
		char* itemKey = NULL;
		char* itemValue = NULL;
		int keySizeRequired, valueSizeRequired;

		tak_APE_GetItemDesc(apetag, idx, &itemDesc);

		itemKey = new char[itemDesc.KeySize + 1];
		::ZeroMemory(itemKey, itemDesc.KeySize + 1);
		itemValue = new char[itemDesc.ValueSize + 1];
		::ZeroMemory(itemValue, itemDesc.ValueSize + 1);

		tak_APE_GetItemKey(apetag, idx, itemKey, itemDesc.KeySize + 1, &keySizeRequired);
		tak_APE_GetItemValue(apetag, idx, itemValue, itemDesc.ValueSize + 1, &valueSizeRequired);

		pInfo->SetValueU8(itemKey, itemValue); // 超手抜きやん！

		delete[] itemKey;
		delete[] itemValue;
	}

	tak_SSD_Destroy(decoder);

	return TRUE;
}
