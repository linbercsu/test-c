#include <jni.h>
#include "test.h"

JNIEXPORT jstring JNICALL
Java_com_sample_x_testc_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject obj) {
    HttpRequest request;
    httpInit(&request, "not used.");
    request.fetchResponse(&request);
    jstring result = (*env)->NewStringUTF(env, request.response);
    httpRelease(&request);

    return result;
}