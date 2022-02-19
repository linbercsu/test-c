#include <jni.h>
#include <string>
#include "test.h"
extern "C" {

JNIEXPORT jstring JNICALL
Java_com_sample_x_testc_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    HttpRequest request;
    httpInit(&request, "not used.");
    request.fetchResponse(&request);
    jstring result = env->NewStringUTF(request.response);
    httpRelease(&request);

    return result;
//    std::string hello = "Hello from C++";
//    return env->NewStringUTF(hello.c_str());
}
}