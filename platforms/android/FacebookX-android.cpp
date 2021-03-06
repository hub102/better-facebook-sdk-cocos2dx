#include "FacebookX.hpp"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
#include <iostream>
#include <android/log.h>

using namespace std;
using namespace cocos2d;

using Json = nlohmann::json;

extern "C"
{
JNIEXPORT void JNICALL Java_com_hub102_facebookx_FacebookX_onLogin(JNIEnv* env, jobject thiz, 
    jboolean isLoggedIn, jstring msg);
JNIEXPORT void JNICALL Java_com_hub102_facebookx_FacebookX_onAPI(JNIEnv* env, jobject thiz, 
    jstring tag, jstring jsonData);
JNIEXPORT void JNICALL Java_com_hub102_facebookx_FacebookX_onAPIFailed(JNIEnv* env, jobject thiz, 
    jstring tag, jstring msg);
JNIEXPORT void JNICALL Java_com_hub102_facebookx_FacebookX_onSharedSuccess(JNIEnv* env, jobject thiz, 
    jstring msg);
JNIEXPORT void JNICALL Java_com_hub102_facebookx_FacebookX_onSharedFailed(JNIEnv* env, jobject thiz, 
    jstring msg);
JNIEXPORT void JNICALL Java_com_hub102_facebookx_FacebookX_onSharedCancel(JNIEnv* env, jobject thiz);
JNIEXPORT void JNICALL Java_com_hub102_facebookx_FacebookX_onRequestInvitableFriends(JNIEnv* env, jobject thiz, 
    jstring friends);
JNIEXPORT void JNICALL Java_com_hub102_facebookx_FacebookX_onInviteFriendsWithInviteIdsResult(JNIEnv* env, 
    jobject thiz, jboolean ok, jstring msg);
JNIEXPORT void JNICALL Java_com_hub102_facebookx_FacebookX_onGetUserInfo(JNIEnv* env, jobject thiz, 
    jstring userInfo);
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
        const char* cTag = env->GetStringUTFChars(tag, NULL);
        string cppJsonData = string(cJsonData);
        string cppTag = string(cTag);
        listener->onAPI(cppTag, cppJsonData);
        env->ReleaseStringUTFChars(tag, cTag);
        env->ReleaseStringUTFChars(jsonData, cJsonData);
    }
}

JNIEXPORT void JNICALL Java_com_hub102_facebookx_FacebookX_onAPIFailed(JNIEnv* env, jobject thiz, 
    jstring tag, jstring msg) {
    h102::FacebookListener* listener = h102::FacebookX::getListener();
    if (listener) {
        const char* cMsg= env->GetStringUTFChars(msg, NULL);
        const char* cTag = env->GetStringUTFChars(tag, NULL);
        string cppMsg = string(cMsg);
        string cppTag = string(cTag);
        listener->onAPI(cppTag, cppMsg);
        env->ReleaseStringUTFChars(tag, cTag);
        env->ReleaseStringUTFChars(msg, cMsg);
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

JNIEXPORT void JNICALL Java_com_hub102_facebookx_FacebookX_onGetUserInfo(JNIEnv* env, jobject thiz, 
    jstring userInfo) {
    h102::FacebookListener* listener = h102::FacebookX::getListener();
    if (listener) {
        const char* cUserInfo = env->GetStringUTFChars(userInfo, NULL);
        string cppUserInfo = string(cUserInfo);
        h102::FBGraphUser user;
        auto json = Json::parse(cppUserInfo);
        if (json.find("id") != json.end() && json["id"].is_string()) {
            user.setField("id", json["id"].get<string>());
        }
        if (json.find("name") != json.end() && json["name"].is_string()) {
            user.setField("name", json["name"].get<string>());   
        }
        if (json.find("first_name") != json.end() && json["first_name"].is_string()) {
            user.setField("first_name", json["first_name"].get<string>());  
        }
        if (json.find("middle_name") != json.end() && json["middle_name"].is_string()) {
            user.setField("first_name", json["first_name"].get<string>());  
        }
        if (json.find("last_name") != json.end() && json["last_name"].is_string()) {
            user.setField("last_name", json["last_name"].get<string>());  
        }
        if (json.find("link_uri") != json.end() && json["link_uri"].is_string()) {
            user.setField("link_uri", json["link_uri"].get<string>());  
        }
        listener->onGetUserInfo(user);
        env->ReleaseStringUTFChars(userInfo, cUserInfo);
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
        for (auto i = permissions.begin(); i != permissions.end(); ++i) {
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
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t, "com/hub102/facebookx/FacebookX", 
                "getUserID", "()Ljava/lang/String;")) {
            jstring id = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);

            t.env->DeleteLocalRef(t.classID);
            
            return JniHelper::jstring2string(id);
        }
    }

    std::string FacebookX::getName() {
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t, "com/hub102/facebookx/FacebookX", 
                "getName", "()Ljava/lang/String;")) {
            jstring name = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);

            t.env->DeleteLocalRef(t.classID);
            
            return JniHelper::jstring2string(name);
        }
    }

    bool FacebookX::isLoggedIn() {
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t, "com/hub102/facebookx/FacebookX",
            "isLoggedIn", "()Z")) {
            jboolean login = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
            t.env->DeleteLocalRef(t.classID);
            return login;
        }
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
        //TODO: implement logic for the C++ api. Remember to invoke the base64 encoding!!!
    }

    void FacebookX::shareEncodedContent(const FBShareInfo& info) {
        string stringifiedInfo;
        stringifiedInfo += (string("type:") + to_string(info.type) + string(";"));
        stringifiedInfo += (string("link:") + string(info.link) + string(";"));
        stringifiedInfo += (string("title:") + string(info.title) + string(";"));
        stringifiedInfo += (string("text:") + string(info.text) + string(";"));
        stringifiedInfo += (string("media:") + string(info.media) + string(";"));
        const char* c_info = stringifiedInfo.c_str();
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t, "com/hub102/facebookx/FacebookX", "share", "(Ljava/lang/String;)V")) {
            jstring stringArg = t.env->NewStringUTF(c_info);
            t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg);
            t.env->DeleteLocalRef(t.classID);
            t.env->DeleteLocalRef(stringArg);
        }
    }

    void FacebookX::shareOpenGraphStory(const FBGraphStoryProperties& properties, const std::string& actionType, const std::string& previewPropertyName) {
        //TODO: implement logic for the C++ api. Remember to invoke the base64 encoding!!!
    }

    void FacebookX::shareEncodedOpenGraphStory(const FBGraphStoryProperties& properties, const std::string& actionType, const std::string& previewPropertyName) {
        string stringifiedProps;
        stringifiedProps += (string("type:") + string(properties.type) + string(";"));
        stringifiedProps += (string("title:") + string(properties.title) + string(";"));
        stringifiedProps += (string("description:") + string(properties.description) + string(";"));
        stringifiedProps += (string("image:") + string(properties.image) + string(";"));
        stringifiedProps += (string("url:") + string(properties.url) + string(";"));
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t, "com/hub102/facebookx/FacebookX", "shareOpenGraphStory", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
            jstring stringProps = t.env->NewStringUTF(stringifiedProps.c_str());
            jstring stringActionType = t.env->NewStringUTF(actionType.c_str());
            jstring stringPreviewPropertyName = t.env->NewStringUTF(previewPropertyName.c_str());
            t.env->CallStaticVoidMethod(t.classID, t.methodID, stringProps, stringActionType, stringPreviewPropertyName);
            t.env->DeleteLocalRef(t.classID);
            t.env->DeleteLocalRef(stringProps);
            t.env->DeleteLocalRef(stringActionType);
            t.env->DeleteLocalRef(stringPreviewPropertyName);
        }    
    }

    bool FacebookX::canPresentWithFBApp(const h102::FBShareInfo &info) {

    }

    void FacebookX::api(const std::string& path, const std::string& tag) {
        FacebookX::api(path, string("GET"), FBAPIParam(), tag);
    }

    void FacebookX::api(const std::string& path, const FBAPIParam& params, const std::string& tag) {
        FacebookX::api(path, string("GET"), params, tag);
    }

    void FacebookX::api(const std::string& path, const std::string& method, const FBAPIParam& params, const std::string& tag) {
        string stringifiedParams;
        for (auto i = params.begin(); i != params.end(); ++i) {
            stringifiedParams += (i->first + string(":") + i->second + string(";"));
        }
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t, "com/hub102/facebookx/FacebookX", "api", 
                "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
            jstring stringPath = t.env->NewStringUTF(path.c_str());
            jstring stringMethod = t.env->NewStringUTF(method.c_str());
            jstring stringParams = t.env->NewStringUTF(stringifiedParams.c_str());
            jstring stringTag = t.env->NewStringUTF(tag.c_str());
            t.env->CallStaticVoidMethod(t.classID, t.methodID, stringPath, stringMethod, stringParams, stringTag);
            t.env->DeleteLocalRef(t.classID);
            t.env->DeleteLocalRef(stringPath);
            t.env->DeleteLocalRef(stringMethod);
            t.env->DeleteLocalRef(stringParams);
            t.env->DeleteLocalRef(stringTag);
        }
    }

    void FacebookX::requestInvitableFriends(const FBAPIParam &params) {
        string stringifiedParams;
        for (auto i = params.begin(); i != params.end(); ++i) {
            stringifiedParams += (i->first + string(":") + i->second + string(";"));
        }
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t, "com/hub102/facebookx/FacebookX", "requestInvitableFriends", "(Ljava/lang/String;)V")) {
            jstring stringParams = t.env->NewStringUTF(stringifiedParams.c_str());
            t.env->CallStaticVoidMethod(t.classID, t.methodID, stringParams);
            t.env->DeleteLocalRef(t.classID);
            t.env->DeleteLocalRef(stringParams);
        }           
    }

    void FacebookX::inviteFriendsWithInviteIds(const std::vector<std::string> &invite_ids, const std::string &title, const std::string &invite_text) {
        string stringifiedIds;
        for (auto i = invite_ids.begin(); i != invite_ids.end(); ++i) {
            stringifiedIds += (*i + string(";"));
        }    
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t, "com/hub102/facebookx/FacebookX", "inviteFriendsWithInviteIds", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
            jstring stringIds = t.env->NewStringUTF(stringifiedIds.c_str());
            jstring stringTitle = t.env->NewStringUTF(title.c_str());
            jstring stringText = t.env->NewStringUTF(invite_text.c_str());
            t.env->CallStaticVoidMethod(t.classID, t.methodID, stringIds, stringTitle, stringText);
            t.env->DeleteLocalRef(t.classID);
            t.env->DeleteLocalRef(stringIds);
            t.env->DeleteLocalRef(stringTitle);
            t.env->DeleteLocalRef(stringText);
        }       
    }
}

#endif