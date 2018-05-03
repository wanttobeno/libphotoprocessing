
#include <stdio.h>
#include <fstream>
#include <tchar.h>
#define  WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <wingdi.h>

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus
#include "../libphotoprocessing/photo_processing.h"
#ifdef __cplusplus
}
#endif //__cplusplus
#ifdef _DEBUG
#pragma comment(lib,"../debug/libphotoprocessing.lib")
#else
#pragma comment(lib,"../release/libphotoprocessing.lib")
#endif // _DEBUG


BITMAPFILEHEADER bitmapfileheader;
BITMAPINFOHEADER head;


void filterPhoto( int position)
{
	switch (position) {
	case 0: // Original
		break;
	case 1: // Instafix
		nativeApplyInstafix();
		break;
	case 2: // Ansel
		nativeApplyAnsel();
		break;
	case 3: // Testino
		nativeApplyTestino();
		break;
	case 4: // XPro
		nativeApplyXPro();
		break;
	case 5: // Retro
		nativeApplyRetro();
		break;
	case 6: // Black & White
		nativeApplyBW();
		break;
	case 7: // Sepia
		nativeApplySepia();
		break;
	case 8: // Cyano
		nativeApplyCyano();
		break;
	case 9: // Georgia
		nativeApplyGeorgia();
		break;
	case 10: // Sahara
		nativeApplySahara();
		break;
	case 11: // HDR
		nativeApplyHDR();
		break;
	}
}

int ChooseFilter(char * pColorData, int nPosition)
{
	int nRet = 0;
	nativeDeleteBitmap();
	nativeInitBitmap(head.biWidth, head.biHeight);
	int nLineSize = head.biWidth * 3;
	char* pixels = (char*)malloc(nLineSize);
	char *pWriteData = (char*)malloc(head.biSizeImage + 1);

	TCHAR szFile[MAX_PATH] = { 0 };
	_stprintf(szFile, _T("picture_Filter_%02d.bmp"), nPosition);
	FILE* pFileSave = _tfopen(szFile, _T("wb"));
	do 
	{
		if (!pixels||!pWriteData||!pFileSave)
		{
			break;
		}

		for (int y = 0; y < head.biHeight; y++)
		{
			memcpy(pixels, pColorData + y*nLineSize, nLineSize);
			nativeSetBitmapRow(y, (int*)pixels);
		}

		filterPhoto(nPosition);

		for (int y = 0; y < head.biHeight; y++)
		{
			memset(pixels, 0, nLineSize);
			nativeGetBitmapRow(y, (int*)pixels);
			memcpy(pWriteData + y*nLineSize, pixels, nLineSize);
		}

		fwrite(&bitmapfileheader, 1, sizeof(BITMAPFILEHEADER), pFileSave);
		fwrite(&head, 1, sizeof(BITMAPINFOHEADER), pFileSave);
		fwrite(pWriteData, 1, head.biSizeImage, pFileSave);
		nRet = 1;
	} while (0);
	free(pWriteData);
	free(pixels);
	fclose(pFileSave);
	return nRet;
}

int main(int agrc, char* agrv)
{
	FILE* pFile = _tfopen(_T("picture.bmp"), _T("rb"));
	if (pFile == NULL)
		return -1;

	fseek(pFile, 0, SEEK_END);
	int nLen = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	fread(&bitmapfileheader, 1, sizeof(BITMAPFILEHEADER), pFile);
	fread(&head, sizeof(BITMAPINFOHEADER), 1, pFile);

	char *pColorData = (char*)malloc(head.biSizeImage+1);
	if (!pColorData)
	{
		fclose(pFile);
		return -1;
	}

	memset(pColorData, 0, head.biSizeImage + 1);
	int nRead = fread(pColorData, 1, head.biSizeImage, pFile);
	fclose(pFile);
	pFile = NULL;

//////////////////////////////////////////////////////////////////////////
	//  图片大小不变的，滤镜效果
	for (int nPosition = 0; nPosition < 12; nPosition++)
	{
		ChooseFilter(pColorData, nPosition);
	}
//////////////////////////////////////////////////////////////////////////
	// 旋转略
//////////////////////////////////////////////////////////////////////////
	free(pColorData);
	return 0;
}
