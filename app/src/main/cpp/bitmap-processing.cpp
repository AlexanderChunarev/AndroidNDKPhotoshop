#include <jni.h>
#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <android/bitmap.h>
#include <cstring>
#include <unistd.h>


class JniBitmap {
public:
    uint32_t *storedPixels;
    AndroidBitmapInfo bitmapInfo;

    JniBitmap() {
        storedPixels = NULL;
    }
};

extern "C" JNIEXPORT jintArray JNICALL
Java_com_example_ndkeducation_filters_BlurFilter_invert(JNIEnv *env, jobject object,
                                                        jintArray rgb) {
    int length = env->GetArrayLength(rgb);
    jintArray processedPixels = env->NewIntArray(length);
    jint *pixels = env->GetIntArrayElements(rgb, NULL);
    jint *newPixels = env->GetIntArrayElements(processedPixels, NULL);
    for (int i = 0; i < length; i++) {
        int alpha = ((pixels[i] >> 24) & 0xFF);
        int red = 255 - ((pixels[i] >> 16) & 0xFF);
        int green = 255 - ((pixels[i] >> 8) & 0xFF);
        int blue = 255 - (pixels[i] & 0xFF);

        newPixels[i] = ((alpha & 0xFF) << 24) + ((red & 0xFF) << 16) + ((green & 0xFF) << 8)
                       + (blue & 0xFF);
    }

    return processedPixels;
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_jni_bitmap_1operations_JniBitmapHolder_jniStoreBitmapData(JNIEnv *env, jobject obj,
                                                                   jobject bitmap) {
    AndroidBitmapInfo bitmapInfo;
    uint32_t *_storedBitmapPixels = NULL;
    if ((AndroidBitmap_getInfo(env, bitmap, &bitmapInfo)) < 0) {
        return NULL;
    }
    if (bitmapInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        return NULL;
    }
    void *bitmapPixels;
    if ((AndroidBitmap_lockPixels(env, bitmap, &bitmapPixels)) < 0) {
        return NULL;
    }
    uint32_t *src = (uint32_t *) bitmapPixels;
    size_t pixelsCount = bitmapInfo.width * bitmapInfo.height;
    _storedBitmapPixels = new uint32_t[pixelsCount];
    memcpy(_storedBitmapPixels, src, pixelsCount);
    AndroidBitmap_unlockPixels(env, bitmap);
    JniBitmap *jniBitmap = new JniBitmap();
    jniBitmap->bitmapInfo = bitmapInfo;
    jniBitmap->storedPixels = _storedBitmapPixels;
    return env->NewDirectByteBuffer(jniBitmap, 0);
}
