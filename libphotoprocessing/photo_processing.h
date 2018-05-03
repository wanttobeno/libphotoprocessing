#ifndef PHOTO_PROCESSING_H__
#define PHOTO_PROCESSING_H__ 1



#ifdef HAVE_INTTYPES_H
# include <inttypes.h>      /* C99 */
	typedef uint8_t         jboolean;       /* unsigned 8 bits */
	typedef int8_t          jbyte;          /* signed 8 bits */
	typedef uint16_t        jchar;          /* unsigned 16 bits */
	typedef int16_t         jshort;         /* signed 16 bits */
	typedef int32_t         jint;           /* signed 32 bits */
	typedef int64_t         jlong;          /* signed 64 bits */
	typedef float           jfloat;         /* 32-bit IEEE 754 */
	typedef double          jdouble;        /* 64-bit IEEE 754 */
#else
	typedef unsigned char   jboolean;       /* unsigned 8 bits */
	typedef signed char     jbyte;          /* signed 8 bits */
	typedef unsigned short  jchar;          /* unsigned 16 bits */
	typedef short           jshort;         /* signed 16 bits */
	typedef int             jint;           /* signed 32 bits */
	typedef long long       jlong;          /* signed 64 bits */
	typedef float           jfloat;         /* 32-bit IEEE 754 */
	typedef double          jdouble;        /* 64-bit IEEE 754 */
#endif

	/* "cardinal indices and sizes" */
	typedef jint            jsize;

#ifdef __cplusplus
	/*
	* Reference types, in C++
	*/
	class _jobject {};
	class _jclass : public _jobject {};
	class _jstring : public _jobject {};
	class _jarray : public _jobject {};
	class _jobjectArray : public _jarray {};
	class _jbooleanArray : public _jarray {};
	class _jbyteArray : public _jarray {};
	class _jcharArray : public _jarray {};
	class _jshortArray : public _jarray {};
	class _jintArray : public _jarray {};
	class _jlongArray : public _jarray {};
	class _jfloatArray : public _jarray {};
	class _jdoubleArray : public _jarray {};
	class _jthrowable : public _jobject {};

	typedef _jobject*       jobject;
	typedef _jclass*        jclass;
	typedef _jstring*       jstring;
	typedef _jarray*        jarray;
	typedef _jobjectArray*  jobjectArray;
	typedef _jbooleanArray* jbooleanArray;
	typedef _jbyteArray*    jbyteArray;
	typedef _jcharArray*    jcharArray;
	typedef _jshortArray*   jshortArray;
	typedef _jintArray*     jintArray;
	typedef _jlongArray*    jlongArray;
	typedef _jfloatArray*   jfloatArray;
	typedef _jdoubleArray*  jdoubleArray;
	typedef _jthrowable*    jthrowable;
	typedef _jobject*       jweak;


#else /* not __cplusplus */

	/*
	* Reference types, in C.
	*/
	typedef void*           jobject;
	typedef jobject         jclass;
	typedef jobject         jstring;
	typedef jobject         jarray;
	typedef jarray          jobjectArray;
	typedef jarray          jbooleanArray;
	typedef jarray          jbyteArray;
	typedef jarray          jcharArray;
	typedef jarray          jshortArray;
	typedef jarray          jintArray;
	typedef jarray          jlongArray;
	typedef jarray          jfloatArray;
	typedef jarray          jdoubleArray;
	typedef jobject         jthrowable;
	typedef jobject         jweak;

#endif /* not __cplusplus */

// ³õÊ¼»¯
int nativeInitBitmap(jint width, jint height);
void nativeDeleteBitmap();
int nativeResizeBitmap(jint newWidth, jint newHeight);

void nativeGetBitmapRow(jint y, int* pixels);
void nativeSetBitmapRow(jint y, int* pixels);

int nativeGetBitmapWidth();
int nativeGetBitmapHeight();

void nativeApplyInstafix();
void nativeApplyAnsel();
void nativeApplyTestino();
void nativeApplyXPro();
void nativeApplyRetro();
// Black & White
void nativeApplyBW();
void nativeApplySepia();
void nativeApplyCyano();
void nativeApplyGeorgia();
void nativeApplySahara();
void nativeApplyHDR();

int nativeLoadResizedJpegBitmap(jbyteArray bytes, jint jpegSize, jint maxPixels);

// Ðý×ª
int nativeRotate90();
void nativeRotate180();
void nativeFlipHorizontally();


#endif // !PHOTO_PROCESSING_H__

