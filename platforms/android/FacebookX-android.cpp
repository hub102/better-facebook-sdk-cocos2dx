#include "FacebookX.hpp"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
#include <iostream>

using namespace std;
using namespace cocos2d;

extern "C"
{
JNIEXPORT void JNICALL Java_com_hub102_facebookx_FacebookX_onLogin(JNIEnv* env, jobject thiz, 
    jboolean isLoggedIn, jstring msg);
JNIEXPORT void JNICALL Java_com_hub102_facebookx_FacebookX_onAPI(JNIEnv* env, jobject thiz, 
    jstring tag, jstring jsonData);
JNIEXPORT void JNICALL Java_com_hub102_facebookx_FacebookX_onSharedSuccess(JNIEnv* env, jobject thiz, 
    jstring msg);
JNIEXPORT void JNICALL Java_com_hub102_facebookx_FacebookX_onSharedFailed(JNIEnv* env, jobject thiz, 
    jstring msg);
JNIEXPORT void JNICALL Java_com_hub102_facebookx_FacebookX_onSharedCancel(JNIEnv* env, jobject thiz);
JNIEXPORT void JNICALL Java_com_hub102_facebookx_FacebookX_onRequestInvitableFriends(JNIEnv* env, jobject thiz, 
    jstring friends);
JNIEXPORT void JNICALL Java_com_hub102_facebookx_FacebookX_onInviteFriendsWithInviteIdsResult(JNIEnv* env, 
    jobject thiz, jboolean ok, jstring msg);
};

JNIEXPORT void JNICALL Java_com_hub102_facebookx_FacebookX_onLogin(JNIEnv* env, jobject thiz, 
    jboolean isLoggedIn, jstring msg) {
    h102::FacebookListener* listener = h102::FacebookX::getListener();
    if (listener) {
        const char* cMsg = env->GetStringUTFChars(msg, NULL);
        bool cppIsLoggedIn = isLoggedIn;
        string cppMsg = string(cMsg);
        listener->onLogin(cppIsLoggedIn, cppMsg);
        env->ReleaseStringUTFChars(msg, cMsg);
    }
}

JNIEXPORT void JNICALL Java_com_hub102_facebookx_FacebookX_onAPI(JNIEnv* env, jobject thiz, 
    jstring tag, jstring jsonData) {
    h102::FacebookListener* listener = h102::FacebookX::getListener();
    if (listener) {
        const char* cJsonData = env->GetStringUTFChars(jsonData, NULL);
        const char* cTag = env->GetStringUTFChars(jsonData, NULL);
        string cppJsonData = string(cJsonData);
        string cppTag = string(cTag);
        listener->onAPI(cppTag, cppJsonData);
        env->ReleaseStringUTFChars(tag, cTag);
        env->ReleaseStringUTFChars(jsonData, cJsonData);
    }
}

JNIEXPORT void JNICALL Java_com_hub102_facebookx_FacebookX_onSharedSuccess(JNIEnv* env, jobject thiz, 
    jstring msg) {
    h102::FacebookListener* listener = h102::FacebookX::getListener();
    if (listener) {
        const char* cMsg = env->GetStringUTFChars(msg, NULL);
        string cppMsg = string(cMsg);
        listener->onSharedSuccess(cppMsg);
        env->ReleaseStringUTFChars(msg, cMsg);
    }
}

JNIEXPORT void JNICALL Java_com_hub102_facebookx_FacebookX_onSharedFailed(JNIEnv* env, jobject thiz, 
    jstring msg) {
    h102::FacebookListener* listener = h102::FacebookX::getListener();
    if (listener) {
        const char* cMsg = env->GetStringUTFChars(msg, NULL);
        string cppMsg = string(cMsg);
        listener->onSharedFailed(cppMsg);
        env->ReleaseStringUTFChars(msg, cMsg);
    }
}

JNIEXPORT void JNICALL Java_com_hub102_facebookx_FacebookX_onSharedCancel(JNIEnv* env, jobject thiz) {
    h102::FacebookListener* listener = h102::FacebookX::getListener();
    if (listener) {
        listener->onSharedCancel();
    }
}

JNIEXPORT void JNICALL Java_com_hub102_facebookx_FacebookX_onRequestInvitableFriends(JNIEnv* env, jobject thiz, 
    jstring friends) {
    h102::FacebookListener* listener = h102::FacebookX::getListener();
    if (listener) {
        const char* cFriends = env->GetStringUTFChars(friends, NULL);
        string cppFriends = string(cFriends);
        listener->onRequestInvitableFriends(cppFriends);
        env->ReleaseStringUTFChars(friends, cFriends);
    }
}

JNIEXPORT void JNICALL Java_com_hub102_facebookx_FacebookX_onInviteFriendsWithInviteIdsResult(JNIEnv* env, 
    jobject thiz, jboolean ok, jstring msg) {
    h102::FacebookListener* listener = h102::FacebookX::getListener();
    if (listener) {
        const char* cMsg = env->GetStringUTFChars(msg, NULL);
        string cppMsg = string(cMsg);
        bool cppOk = ok;
        listener->onInviteFriendsWithInviteIdsResult(cppOk, cppMsg);
        env->ReleaseStringUTFChars(msg, cMsg);
    }
}

namespace h102 {

    void FacebookX::login() {
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t, "com/hub102/facebookx/FacebookX", "login", "()V")) {
            t.env->CallStaticVoidMethod(t.classID, t.methodID);
            t.env->DeleteLocalRef(t.classID);
        } 
    }

    void FacebookX::login(std::vector<std::string>& permissions) {
        if (permissions.size() == 0) {
            FacebookX::login();
            return;
        }
        string params;
        for (auto i = permissions.begin(); i != permissions.end(); i++) {
            params += *i;
            params += string(";");
        }

        const char* c_params = params.c_str();

        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t, "com/hub102/facebookx/FacebookX", "login", "(Ljava/lang/String;)V")) {
            jstring stringArg = t.env->NewStringUTF(c_params);
            t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg);
            t.env->DeleteLocalRef(t.classID);
            t.env->DeleteLocalRef(stringArg);
        }
    }

    std::string FacebookX::getAccessToken() {
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t, "com/hub102/facebookx/FacebookX", 
                "getAccessToken", "()Ljava/lang/String;")) {
            jstring token = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);

            t.env->DeleteLocalRef(t.classID);
            
            return JniHelper::jstring2string(token);
        }
    }

    std::string FacebookX::getUserID() {

    }

    std::string FacebookX::getName() {

    }

    bool FacebookX::isLoggedIn() {

    }

    void FacebookX::logout() {
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t, "com/hub102/facebookx/FacebookX", "logout", "()V")) {
            t.env->CallStaticVoidMethod(t.classID, t.methodID);
            t.env->DeleteLocalRef(t.classID);
        } 
    }

    vector<string> FacebookX::getPermissionList() {

    }

    void FacebookX::share(const FBShareInfo& info) {

    }

    bool FacebookX::canPresentWithFBApp(const h102::FBShareInfo &info) {

    }

    void FacebookX::api(const std::string& path, const std::string& tag) {

    }

    void FacebookX::api(const std::string& path, const FBAPIParam& params, const std::string& tag) {

    }

    void FacebookX::api(const std::string& path, const std::string& method, const FBAPIParam& params, const std::string& tag) {

    }

    void FacebookX::shareOpenGraphStory(const FBGraphStoryProperties& properties, const std::string& actionType, const std::string& previewPropertyName) {

    }

    void FacebookX::shareEncodedOpenGraphStory(const FBGraphStoryProperties& properties, const std::string& actionType, const std::string& previewPropertyName) {

    }

    void FacebookX::requestInvitableFriends(const FBAPIParam &params) {

    }

    void FacebookX::inviteFriendsWithInviteIds(const std::vector<std::string> &invite_ids, const std::string &title, const std::string &invite_text) {

    }
}

#endif