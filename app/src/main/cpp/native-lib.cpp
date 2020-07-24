#include <jni.h>
#include <cmath>
#include <android/bitmap.h>
#include <cstring>

const int MAX_COLOR_VALUE = 255;
const int MIN_COLOR_VALUE = 0;


int setValidColorValue(int value);

double gaussian(int x, int y, int variance);

int setValidColorValue(int value) {
    if (value > MAX_COLOR_VALUE) {
        return MAX_COLOR_VALUE;
    } else if (value < MIN_COLOR_VALUE) {
        return MIN_COLOR_VALUE;
    }
    return value;
}

double gaussian(int x, int y, int variance) {
    return (1 / (2 * M_PI * pow(variance, 2))
            * exp(-(pow(x, 2) + pow(y, 2)) / (2 * pow(variance, 2))));
}

class JniBitmap {
public:
    uint32_t *storedPixels;
    AndroidBitmapInfo bitmapInfo;

    JniBitmap() {
        storedPixels = NULL;
    }
};

extern "C"
JNIEXPORT jobject JNICALL
Java_com_example_ndkeducation_filters_BaseFilter_jniStoreBitmapData(JNIEnv *env, jobject thiz,
                                                                    jobject bitmap) {
    AndroidBitmapInfo bitmapInfo;
    uint32_t *_storedBitmapPixels = NULL;
    if ((AndroidBitmap_getInfo(env, bitmap, &bitmapInfo)) < 0) {
        return NULL;
    }
    void *bitmapPixels;
    if ((AndroidBitmap_lockPixels(env, bitmap, &bitmapPixels)) < 0) {
        return NULL;
    }
    uint32_t *src = (uint32_t *) bitmapPixels;
    int pixelsCount = bitmapInfo.width * bitmapInfo.height;
    _storedBitmapPixels = new uint32_t[pixelsCount];
    memcpy(_storedBitmapPixels, src, (sizeof(uint32_t)) * pixelsCount);
    AndroidBitmap_unlockPixels(env, bitmap);
    JniBitmap *jniBitmap = new JniBitmap();
    jniBitmap->bitmapInfo = bitmapInfo;
    jniBitmap->storedPixels = _storedBitmapPixels;
    return env->NewDirectByteBuffer(jniBitmap, 0);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_ndkeducation_filters_BlurFilter_jniBlur(JNIEnv *env,
                                                         jobject obj,
                                                         jobject byteBuffer,
                                                         jint radius) {
    JniBitmap *jniBitmap = (JniBitmap *) env->GetDirectBufferAddress(byteBuffer);
    uint32_t *basePixels = jniBitmap->storedPixels;
    AndroidBitmapInfo bitmapInfo = jniBitmap->bitmapInfo;
    uint32_t *newPixels = new uint32_t[bitmapInfo.height * bitmapInfo.width];
    double weights[radius][radius];
    double sum = 0;
    for (int i = 0; i < radius; ++i) {
        for (int j = 0; j < radius; ++j) {
            weights[i][j] = gaussian(i - radius / 2, j - radius / 2, 2);
            sum += weights[i][j];
        }
    }

    for (int i = 0; i < radius; ++i) {
        for (int j = 0; j < radius; ++j) {
            weights[i][j] /= sum;
        }
    }

    int index = 0;
    for (int x = 0; x < bitmapInfo.height; ++x) {
        for (int y = 0; y < bitmapInfo.width; ++y) {
            uint32_t red = 0, green = 0, blue = 0;
            if (x < bitmapInfo.height - radius && y < bitmapInfo.width - radius) {
                for (int i = 0; i < radius; ++i) {
                    for (int j = 0; j < radius; ++j) {
                        double currWeight = weights[i][j];
                        uint32_t currPixel = basePixels[((x + i) * (bitmapInfo.width) + y) + j];
                        red += currWeight * ((currPixel >> 16) & 0xFF);
                        green += currWeight * ((currPixel >> 8) & 0xFF);
                        blue += currWeight * (currPixel & 0xFF);

                    }
                }
                newPixels[index] =
                        (((basePixels[index] >> 24 & 0xFF) & 0xFF) << 24) +
                        ((setValidColorValue(red) & 0xFF) << 16) +
                        ((setValidColorValue(green) & 0xFF) << 8) +
                        (setValidColorValue(blue) & 0xFF);
            }
            index++;
        }
    }
    delete[] basePixels;
    jniBitmap->storedPixels = newPixels;
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_ndkeducation_filters_SharpnessFilter_sharpness(JNIEnv *env,
                                                                jobject obj,
                                                                jobject byteBuffer) {
    JniBitmap *jniBitmap = (JniBitmap *) env->GetDirectBufferAddress(byteBuffer);
    uint32_t *basePixels = jniBitmap->storedPixels;
    AndroidBitmapInfo bitmapInfo = jniBitmap->bitmapInfo;
    uint32_t *newPixels = new uint32_t[bitmapInfo.height * bitmapInfo.width];
    int radius = 3;
    double weights[3][3] = {{0,  -1, 0},
                            {-1, 5,  -1},
                            {0,  -1, 0}};

    int index = 0;
    for (int x = 0; x < bitmapInfo.height; ++x) {
        for (int y = 0; y < bitmapInfo.width; ++y) {
            int red = 0, green = 0, blue = 0;
            if (x < bitmapInfo.height - radius && y < bitmapInfo.width - radius) {
                for (int i = 0; i < radius; ++i) {
                    for (int j = 0; j < radius; ++j) {
                        double currWeight = weights[i][j];
                        uint32_t currPixel = basePixels[((x + i) * (bitmapInfo.width) + y) + j];
                        red += currWeight * ((currPixel >> 16) & 0xFF);
                        green += currWeight * ((currPixel >> 8) & 0xFF);
                        blue += currWeight * (currPixel & 0xFF);
                    }
                }
                newPixels[index] =
                        (((basePixels[index] >> 24 & 0xFF) & 0xFF) << 24) +
                        ((setValidColorValue(red) & 0xFF) << 16) +
                        ((setValidColorValue(green) & 0xFF) << 8) +
                        (setValidColorValue(blue) & 0xFF);
            }
            index++;
        }
    }
    delete[] basePixels;
    jniBitmap->storedPixels = newPixels;
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_ndkeducation_filters_InvertFilter_invert(JNIEnv *env,
                                                          jobject obj,
                                                          jobject handle) {
    JniBitmap *jniBitmap = (JniBitmap *) env->GetDirectBufferAddress(handle);
    uint32_t *basePixels = jniBitmap->storedPixels;
    AndroidBitmapInfo bitmapInfo = jniBitmap->bitmapInfo;
    uint32_t *newPixels = new uint32_t[bitmapInfo.height * bitmapInfo.width];
    for (int i = 0; i < bitmapInfo.height * bitmapInfo.width; ++i) {
        uint32_t t = basePixels[i];
        uint32_t alpha = ((t >> 24) & 0xFF);
        uint32_t red = 255 - ((t >> 16) & 0xFF);
        uint32_t green = 255 - ((t >> 8) & 0xFF);
        uint32_t blue = 255 - (t & 0xFF);

        newPixels[i] = ((alpha & 0xFF) << 24) + ((red & 0xFF) << 16) + ((green & 0xFF) << 8)
                       + (blue & 0xFF);
    }

    delete[]basePixels;
    jniBitmap->storedPixels = newPixels;
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_ndkeducation_filters_SepiaFilter_sepia(JNIEnv *env,
                                                        jobject obj,
                                                        jobject handle) {
    JniBitmap *jniBitmap = (JniBitmap *) env->GetDirectBufferAddress(handle);
    uint32_t *basePixels = jniBitmap->storedPixels;
    AndroidBitmapInfo bitmapInfo = jniBitmap->bitmapInfo;
    uint32_t *newPixels = new uint32_t[bitmapInfo.height * bitmapInfo.width];
    for (int i = 0; i < bitmapInfo.height * bitmapInfo.width; ++i) {
        uint32_t pixel = basePixels[i];
        uint32_t alpha = pixel >> 24 & 0xFF;
        uint32_t red = pixel >> 16 & 0xFF;
        uint32_t green = pixel >> 8 & 0xFF;
        uint32_t blue = pixel & 0xFF;

        int newRed = setValidColorValue((int) (0.393 * red + 0.769 * green + 0.189 * blue));
        int newGreen = setValidColorValue((int) (0.349 * red + 0.686 * green + 0.168 * blue));
        int newBlue = setValidColorValue((int) (0.272 * red + 0.534 * green + 0.131 * blue));
        newPixels[i] = ((alpha & 0xFF) << 24) + ((newRed & 0xFF) << 16) + ((newGreen & 0xFF) << 8)
                       + (newBlue & 0xFF);
    }
    delete[]basePixels;
    jniBitmap->storedPixels = newPixels;
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_ndkeducation_filters_GrayScaleFilter_grayScale(JNIEnv *env,
                                                                jobject obj,
                                                                jobject handle) {
    JniBitmap *jniBitmap = (JniBitmap *) env->GetDirectBufferAddress(handle);
    uint32_t *basePixels = jniBitmap->storedPixels;
    AndroidBitmapInfo bitmapInfo = jniBitmap->bitmapInfo;
    uint32_t *newPixels = new uint32_t[bitmapInfo.height * bitmapInfo.width];
    for (int i = 0; i < bitmapInfo.height * bitmapInfo.width; ++i) {
        uint32_t t = basePixels[i];
        uint32_t alpha = (t >> 24) & 0xFF;
        uint32_t red = (t >> 16) & 0xFF;
        uint32_t green = (t >> 8) & 0xFF;
        uint32_t blue = t & 0xFF;
        int average = setValidColorValue((red + green + blue) / 3);
        newPixels[i] = ((alpha & 0xFF) << 24) + ((average & 0xFF) << 16) + ((average & 0xFF) << 8)
                       + (average & 0xFF);
    }
    delete[]basePixels;
    jniBitmap->storedPixels = newPixels;
}

extern "C" JNIEXPORT jobject JNICALL
Java_com_example_ndkeducation_filters_BaseFilter_jniGetBitmap(JNIEnv *env,
                                                              jobject obj,
                                                              jobject handle) {
    JniBitmap *jniBitmap = (JniBitmap *) env->GetDirectBufferAddress(handle);
    if (jniBitmap->storedPixels == NULL) {
        return NULL;
    }
    jclass bitmapCls = (jclass) env->FindClass("android/graphics/Bitmap");
    jmethodID createBitmapFunction = env->GetStaticMethodID(bitmapCls, "createBitmap",
                                                            "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");

    jstring configName = env->NewStringUTF("ARGB_8888");
    jclass bitmapConfigClass = env->FindClass("android/graphics/Bitmap$Config");
    jclass enumClass = env->FindClass("android/graphics/Bitmap$Config");
    jmethodID valueOfBitmapConfigFunction = env->GetStaticMethodID(bitmapConfigClass, "valueOf",
                                                                   "(Ljava/lang/Class;Ljava/lang/String;)Ljava/lang/Enum;");
    jobject java_bitmap_config = env->CallStaticObjectMethod(bitmapConfigClass,
                                                             valueOfBitmapConfigFunction,
                                                             enumClass,
                                                             configName);
    jobject newBitmap = env->CallStaticObjectMethod(bitmapCls, createBitmapFunction,
                                                    (int) jniBitmap->bitmapInfo.width,
                                                    (int) jniBitmap->bitmapInfo.height,
                                                    java_bitmap_config);

    void *bitmapPixels;
    if ((AndroidBitmap_lockPixels(env, newBitmap, &bitmapPixels)) < 0) {
        return NULL;
    }
    uint32_t *newBitmapPixels = (uint32_t *) bitmapPixels;
    int pixelsCount = jniBitmap->bitmapInfo.height * jniBitmap->bitmapInfo.width;
    memcpy(newBitmapPixels, jniBitmap->storedPixels, sizeof(uint32_t) * pixelsCount);
    AndroidBitmap_unlockPixels(env, newBitmap);
    return newBitmap;
}
