/*
 * Copyright (C) 2012 Lightbox
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "photo_processing.h"
//#include <jni.h>
#include <stdlib.h>
#include <bitmap.h>
#include <mem_utils.h>
#include <android/log.h>

#define  LOG_TAG    "PREVIEW_CACHE_IMAGE_PROCESSING"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)




static Bitmap bitmap;


// 给参数三赋值
void GetIntArrayRegion(int* arrSrc, int nStart, int nSize, int* arrDst)
{
	char* pDst = (char*)arrSrc;
	pDst = pDst + (nStart)* sizeof(int);
	char* pSrc = (char*)arrDst;
	memcpy(pSrc, nSize, pDst);
}

// 给参数一赋值
void SetIntArrayRegion(int arrDst[], int nStart, int nSize, int* arrSrc)
{
	char* pDst = (char*)arrDst;
	pDst = pDst + (nStart)* sizeof(int);
	char* pSrc = (char*)arrSrc;
	memcpy(pDst, nSize*sizeof(int), pSrc);
}

void*  GetPrimitiveArrayCritical(jarray arr, jboolean* bValue)
{
	return NULL;
}

void* ReleasePrimitiveArrayCritical(jarray arr, void* data, jint nValue)
{

	return NULL;
}
//////////////////////////////////////////////////////////////////////////

int nativeInitBitmap(jint width, jint height) {
	return initBitmapMemory(&bitmap, width, height);
}

void nativeGetBitmapRow(jint y, int* pixels) 
{
	//int cpixels[bitmap.width];
	//getBitmapRowAsIntegers(&bitmap, (int)y, &cpixels);
	//SetIntArrayRegion(pixels, 0, bitmap.width, cpixels);
	//getBitmapRowAsIntegers(&bitmap, (int)y, pixels);

	int* cpixels = (int*)malloc(bitmap.width*sizeof(int));
	memset(cpixels, 0, bitmap.width*sizeof(int));
	getBitmapRowAsIntegers(&bitmap, (int)y, cpixels);
	for (int i = 0; i < bitmap.width; i++)
	{
		char* rgba = (char*)&cpixels[i];
		char* rgb = (char*)pixels + i * 3;
		rgb[0] = *rgba;
		rgb[1] = *(rgba + 1);
		rgb[2] = *(rgba + 2);
	}
	free(cpixels);
}

void nativeSetBitmapRow(jint y, int* pixels) 
{
	//int cpixels[bitmap.width];
	//GetIntArrayRegion( pixels, 0, bitmap.width, cpixels);
	//setBitmapRowFromIntegers(&bitmap, (int)y, &cpixels);
	int* cpixels = (int*)malloc(bitmap.width*sizeof(int));
	memset(cpixels, 0, bitmap.width*sizeof(int));
	for (int i = 0; i < bitmap.width;i++)
	{
		char* rgba = (char*)&cpixels[i];
		char* rgb = (char*)pixels + i * 3; // 原图是RGB
		rgba[0] = *rgb;
		rgba[1] = *(rgb + 1);
		rgba[2] = *(rgb + 2);
		rgba[3] = 0;
	}
	setBitmapRowFromIntegers(&bitmap, (int)y, cpixels);
	free(cpixels);
}

int nativeGetBitmapWidth() {
	return bitmap.width;
}

int nativeGetBitmapHeight() {
	return bitmap.height;
}

void nativeDeleteBitmap() {
	deleteBitmap(&bitmap);
}

void nativeFlipHorizontally() {
	flipHorizontally(&bitmap, 1, 1, 1);
}

int nativeRotate90() {
	int resultCode = rotate90(&bitmap, 1, 1, 1);
	if (resultCode != MEMORY_OK) {
		return resultCode;
	}

	//All the component dimensions should have changed, so copy the correct dimensions
	bitmap.width = bitmap.redWidth;
	bitmap.height = bitmap.redHeight;
}

void nativeRotate180() {
	rotate180(&bitmap, 1, 1, 1);
}

void nativeApplyInstafix() {
	applyInstafix(&bitmap);
}

void nativeApplyAnsel() {
	applyAnselFilter(&bitmap);
}

void nativeApplyTestino() {
	applyTestino(&bitmap);
}

void nativeApplyXPro() {
	applyXPro(&bitmap);
}

void nativeApplyRetro() {
	applyRetro(&bitmap);
}

void nativeApplyBW() {
	applyBlackAndWhiteFilter(&bitmap);
}

void nativeApplySepia() {
	applySepia(&bitmap);
}

void nativeApplyCyano() {
	applyCyano(&bitmap);
}

void nativeApplyGeorgia() {
	applyGeorgia(&bitmap);
}

void nativeApplySahara() {
	applySahara(&bitmap);
}

void nativeApplyHDR() {
	applyHDR(&bitmap);
}

int nativeLoadResizedJpegBitmap(jbyteArray bytes, jint jpegSize, jint maxPixels) {
	char* jpegData = (char*) GetPrimitiveArrayCritical(bytes, NULL);

	if (jpegData == NULL) {
		LOGE("jpeg data was null");
		return JNI_GET_INT_ARRAY_ERROR;
	}

	int resultCode = decodeJpegData(jpegData, jpegSize, maxPixels, &bitmap);
	if (resultCode != MEMORY_OK) {
		deleteBitmap(&bitmap);
		LOGE("error decoding jpeg resultCode=%d", resultCode);
		return resultCode;
	}

	ReleasePrimitiveArrayCritical(bytes, jpegData, 0);

	return MEMORY_OK;
}

int nativeResizeBitmap(jint newWidth, jint newHeight) {
	unsigned char* newRed;
	int resultCode = newUnsignedCharArray(newWidth*newHeight, &newRed);
	if (resultCode != MEMORY_OK) {
		return resultCode;
	}
	resizeChannelBicubic(bitmap.red, bitmap.width, bitmap.height, newRed, (int)newWidth, (int)newHeight);
	freeUnsignedCharArray(&bitmap.red);
	bitmap.red = newRed;
	bitmap.redWidth = newWidth;
	bitmap.redHeight = newHeight;

	unsigned char* newGreen;
	resultCode = newUnsignedCharArray(newWidth*newHeight, &newGreen);
	if (resultCode != MEMORY_OK) {
		return resultCode;
	}
	resizeChannelBicubic(bitmap.green, bitmap.width, bitmap.height, newGreen, (int)newWidth, (int)newHeight);
	freeUnsignedCharArray(&bitmap.green);
	bitmap.green = newGreen;
	bitmap.greenWidth = newWidth;
	bitmap.greenHeight = newHeight;

	unsigned char* newBlue;
	resultCode = newUnsignedCharArray(newWidth*newHeight, &newBlue);
	if (resultCode != MEMORY_OK) {
		return resultCode;
	}
	resizeChannelBicubic(bitmap.blue, bitmap.width, bitmap.height, newBlue, (int)newWidth, (int)newHeight);
	freeUnsignedCharArray(&bitmap.blue);
	bitmap.blue = newBlue;
	bitmap.blueWidth = newWidth;
	bitmap.blueHeight = newHeight;

	bitmap.width = newWidth;
	bitmap.height = newHeight;
}
