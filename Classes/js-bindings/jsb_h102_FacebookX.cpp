//
//  jsb_h102_FacebookX.cpp
//  FacebookXTest
//
//  Created by Thuy Dong Xuan on 4/12/16.
//
//

#include "jsb_h102_FacebookX.hpp"
#include "cocos2d_specifics.hpp"

#include "JSHelper.h"
#include "FacebookX.hpp"

using namespace h102;

JSOBJECT* FBGraphUserToJS(JSContext* cx, const FBGraphUser& info) {
    
    JS_INIT_CONTEXT_FOR_UPDATE(cx);
    
    JSOBJECT* jsobj= JS_NEW_OBJECT(cx);
    
    for( std::pair<std::string, std::string> _p : info.getFields() ) {
        addProperty( cx, jsobj, _p.first.c_str(), _p.second );
    }
    
    return jsobj;
}

JSOBJECT* FBInvitableFriendsInfoToJS( JSContext* cx, const FBInvitableFriendsInfo& ifap ) {
    
    JS_INIT_CONTEXT_FOR_UPDATE(cx);
    
    // collection of FBGraphUser
    JSOBJECT *jarr= JS_NEW_ARRAY(cx, ifap.getNumInvitationTokens());
    
    uint32_t index=0;
    for( auto u= ifap.begin(); u!=ifap.end(); ++u ) {
        JSOBJECT* gu= FBGraphUserToJS( cx, *(u) );
        JS_ARRAY_SET( cx, jarr, index, gu );
        index++;
    }
    
    // cursor data
    JSOBJECT* jsobj= JS_NEW_OBJECT(cx);
    addProperty( cx, jsobj, "next_url", ifap.getNextURL() );
    addProperty( cx, jsobj, "prev_url", ifap.getPrevURL() );
    addProperty( cx, jsobj, "prev_cursor", ifap.getPrevCursor() );
    addProperty( cx, jsobj, "next_cursor", ifap.getNextCursor() );
    
    // data
    JSOBJECT* ret= JS_NEW_OBJECT(cx);
    addProperty( cx, ret, "data", jarr );
    addProperty( cx, ret, "paging", jsobj );
    
    return ret;
}


FBShareInfo map_to_FBShareInfo(const std::map<std::string, std::string>& dict)
{
    FBShareInfo info;

    if (dict.find("title") != dict.end())
    {
        info.title = dict.find("title")->second;
    }
    if (dict.find("link") != dict.end())
    {
        info.link = dict.find("link")->second;
    }
    if (dict.find("text") != dict.end())
    {
        info.text = dict.find("text")->second;
    }
    if (dict.find("media") != dict.end())
    {
        info.media = dict.find("media")->second;
    }
    if (dict.find("type") != dict.end())
    {
        std::string type = dict.find("type")->second;
        if (type.compare("link") == 0)
        {
            info.type = FB_LINK;
        }
        else if (type.compare("photo") == 0)
        {
            info.type = FB_PHOTO;
        }
    }

    return info;
}

FBGraphStoryProperties map_to_FBGraphStoryProperties(const std::map<std::string,std::string> &dict) {
	FBGraphStoryProperties properties;

    if (dict.find("type") != dict.end())
    {
        properties.type = dict.find("type")->second;
    }
    if (dict.find("title") != dict.end())
    {
        properties.title = dict.find("title")->second;
    }
    if (dict.find("description") != dict.end())
    {
        properties.description = dict.find("description")->second;
    }
    if (dict.find("image") != dict.end())
    {
        properties.image = dict.find("image")->second;
    }
    if (dict.find("url") != dict.end())
    {
        properties.url = dict.find("url")->second;
    }
    
    return properties;
}


class FacebookListenerJsHelper : public FacebookListener
{
private:
    void invokeDelegate(std::string& fName, jsval dataVal[], int argc) {
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        const char* func_name = fName.c_str();

        JS::RootedObject obj(cx, getJSDelegate());
        JSAutoCompartment ac(cx, obj);

#if MOZJS_MAJOR_VERSION >= 31
        bool hasAction;
        JS::RootedValue retval(cx);
        JS::RootedValue func_handle(cx);
#elif MOZJS_MAJOR_VERSION >= 28
        bool hasAction;
        jsval retval;
        JS::RootedValue func_handle(cx);
#else
        JSBool hasAction;
        jsval retval;
        jsval func_handle;
#endif

        if (JS_HasProperty(cx, obj, func_name, &hasAction) && hasAction) {
            if(!JS_GetProperty(cx, obj, func_name, &func_handle)) {
                return;
            }
            if(func_handle == JSVAL_VOID) {
                return;
            }

#if MOZJS_MAJOR_VERSION >= 31
            if (0 == argc) {
                JS_CallFunctionName(cx, obj, func_name, JS::HandleValueArray::empty(), &retval);
            } else {
                JS_CallFunctionName(cx, obj, func_name, JS::HandleValueArray::fromMarkedLocation(argc, dataVal), &retval);
            }
#else
            if (0 == argc) {
                JS_CallFunctionName(cx, obj, func_name, 0, nullptr, &retval);
            } else {
                JS_CallFunctionName(cx, obj, func_name, argc, dataVal, &retval);
            }
#endif
        }
    }
    
private:
    JSObject* mJsDelegate;
    JSFunctionWrapper *_jsFuncWrapper;
    
public:
    void setJSDelegate(JS::HandleValue delegate)
    {
        mJsDelegate = delegate.toObjectOrNull();
      
        CC_SAFE_DELETE(_jsFuncWrapper);
        
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        
        JS::RootedObject targetObj(cx);
        targetObj.set(mJsDelegate);
        _jsFuncWrapper = new JSFunctionWrapper(cx, targetObj, JS::NullHandleValue);
    }
    
    JSObject* getJSDelegate()
    {
        return mJsDelegate;
    }
    
    FacebookListenerJsHelper() : mJsDelegate(0)
    {
      _jsFuncWrapper = NULL;
    }
    
    virtual void onLogin(bool isLogin, const std::string& error)
    {
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        std::string name("onLogin");
        jsval dataVal[2];
        dataVal[0] = BOOLEAN_TO_JSVAL(isLogin);
        dataVal[1] = c_string_to_jsval(cx, error.c_str());
        invokeDelegate(name, dataVal, 2);
    }
    virtual void onSharedSuccess(const std::string& message)
    {
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        std::string name("onSharedSuccess");
        jsval dataVal[1];
        dataVal[0] = c_string_to_jsval(cx, message.c_str());
        invokeDelegate(name, dataVal, 1);
    }
    virtual void onSharedFailed(const std::string& message)
    {
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        std::string name("onSharedFailed");
        jsval dataVal[1];
        dataVal[0] = c_string_to_jsval(cx, message.c_str());
        invokeDelegate(name, dataVal, 1);
    }
    virtual void onSharedCancel()
    {
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        std::string name("onSharedCancel");
        jsval dataVal[0];
        invokeDelegate(name, dataVal, 0);
    }
    virtual void onAPI(const std::string& tag, const std::string& jsonData)
    {
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        std::string name("onAPI");
        jsval dataVal[2];
        dataVal[0] = c_string_to_jsval(cx, tag.c_str());
        dataVal[1] = c_string_to_jsval(cx, jsonData.c_str());
        invokeDelegate(name, dataVal, 2);
    }
    virtual void onAPIFailed(const std::string& tag, const std::string& msg)
    {
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        std::string name("onAPIFailed");
        jsval dataVal[2];
        dataVal[0] = c_string_to_jsval(cx, tag.c_str());
        dataVal[1] = c_string_to_jsval(cx, msg.c_str());
        invokeDelegate(name, dataVal, 2);
    }
    virtual void onPermission(bool isLogin, const std::string& error)
    {
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        std::string name("onPermission");
        jsval dataVal[2];
        dataVal[0] = BOOLEAN_TO_JSVAL(isLogin);
        dataVal[1] = c_string_to_jsval(cx, error.c_str());
        invokeDelegate(name, dataVal, 2);
    }
    virtual void onFetchFriends(bool ok, const std::string& msg)
    {
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        std::string name("onFetchFriends");
        jsval dataVal[2];
        dataVal[0] = BOOLEAN_TO_JSVAL(ok);
        dataVal[1] = c_string_to_jsval(cx, msg.c_str());
        invokeDelegate(name, dataVal, 2);
    }
    virtual void onInviteFriendsResult(bool ok, const std::string& msg)
    {
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        std::string name("onInviteFriendsResult");
        jsval dataVal[2];
        dataVal[0] = BOOLEAN_TO_JSVAL(ok);
        dataVal[1] = c_string_to_jsval(cx, msg.c_str());
        invokeDelegate(name, dataVal, 2);
    }
    virtual void onInviteFriendsWithInviteIdsResult(bool ok, const std::string& msg)
    {
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        std::string name("onInviteFriendsWithInviteIdsResult");
        jsval dataVal[2];
        dataVal[0] = BOOLEAN_TO_JSVAL(ok);
        dataVal[1] = c_string_to_jsval(cx, msg.c_str());
        invokeDelegate(name, dataVal, 2);
    }
    virtual void onRequestInvitableFriends(const FBInvitableFriendsInfo& invitable_friends_and_pagination )
    {
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        std::string name("onRequestInvitableFriends");
        jsval dataVal[1];
        dataVal[0] = OBJECT_TO_JSVAL(FBInvitableFriendsInfoToJS( cx, invitable_friends_and_pagination));
        invokeDelegate(name, dataVal, 1);
    }
    virtual void onGetUserInfo(const FBGraphUser& userInfo )
    {
        JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
        std::string name("onGetUserInfo");
        jsval dataVal[1];
        dataVal[0] = OBJECT_TO_JSVAL(FBGraphUserToJS( cx, userInfo ));
        invokeDelegate(name, dataVal, 1);
    }
};


static bool js_is_native_obj(JSContext *cx, uint32_t argc, jsval* vp)
{
	JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
	args.rval().setBoolean(true);
	return true;
} 

JSClass  *jsb_h102_facebookX_class;
JSObject *jsb_h102_facebookX_prototype;

bool js_h102_facebookX_constructor(JSContext *cx, uint32_t argc, jsval* vp)
{
	JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
	bool ok = true;
	FacebookX* cobj = new (std::nothrow) FacebookX();

	TypeTest<FacebookX> t;
	js_type_class_t *typeClass = nullptr;
	std::string typeName = t.s_name();
	auto typeMapIter = _js_global_type_map.find(typeName);
	CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
	typeClass = typeMapIter->second;
	CCASSERT(typeClass, "The value is null.");

//    js_type_class_t *typeClass = js_get_type_from_native<h102::FacebookX>(cobj);
    
	JS::RootedObject proto(cx, typeClass->proto.ref());
	JS::RootedObject parent(cx, typeClass->parentProto.ref());
	JS::RootedObject obj(cx, JS_NewObject(cx, typeClass->jsclass, proto, parent));
//    JS::RootedObject obj(cx, jsb_ref_create_jsobject(cx, cobj, typeClass, "h102::FacebookX"));

	js_proxy_t* p = jsb_new_proxy(cobj, obj);
	AddNamedObjectRoot(cx, &p->obj, "FacebookX");
	if (JS_HasProperty(cx, obj, "_ctor", &ok) && ok)
		ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(obj), "_ctor", args);
	return true;
//    JS::RootedObject jsobj(cx, jsb_ref_create_jsobject(cx, cobj, typeClass, "h102::FacebookX"));
//    args.rval().set(OBJECT_TO_JSVAL(jsobj));
//    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
//        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
//    return true;
}

bool js_h102_facebookX_init(JSContext *cx, uint32_t argc, jsval *vp) {
	return true;
}

bool js_h102_facebookX_setListener(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
  
    if (argc == 1) {
        
      if (!args.get(0).isObject())
      {
        ok = false;
      }
      
      JSB_PRECONDITION2(ok, cx, false, "js_PluginFacebookJS_PluginFacebook_setListener : Error processing arguments");
      FacebookListenerJsHelper* lis = new FacebookListenerJsHelper();
      lis->setJSDelegate(args.get(0));
      FacebookX::setListener(lis);
      
      args.rval().setUndefined();
      return true;
    }

    JS_ReportError(cx, "js_h102_facebookX_setListener : wrong number of arguments");
    return false;
}

bool js_h102_facebookX_login(JSContext *cx, uint32_t argc, jsval *vp) {
	JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
	bool ok = true;

	do {
		if (argc == 1) {
			std::vector<std::string> arg0;
			ok &= jsval_to_std_vector_string(cx, args.get(0), &arg0);
			if (!ok) {ok = true; break;}
			FacebookX::login(arg0);
			return true;
		}
	} while (0);

	do {
		if (argc == 0) {
			FacebookX::login();
			return true;
		}
	} while (0);

    JS_ReportError(cx, "js_h102_facebookX_login : wrong number of arguments");
    return false;
}

bool js_h102_facebookX_getAccessToken(JSContext *cx, uint32_t argc, jsval *vp) {
	JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
	do {
		if (argc == 0) {
			std::string ret = FacebookX::getAccessToken();
			jsval jsret = std_string_to_jsval(cx, ret);
			args.rval().set(jsret);
			return true;
		}
	} while (0);

    JS_ReportError(cx, "js_h102_facebookX_getAccessToken : wrong number of arguments");
    return false;
}

bool js_h102_facebookX_getUserID(JSContext *cx, uint32_t argc, jsval *vp) {
	JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
	do {
		if (argc == 0) {
			std::string ret = FacebookX::getUserID();
			jsval jsret = std_string_to_jsval(cx, ret);
			args.rval().set(jsret);
			return true;
		}
	} while (0);

    JS_ReportError(cx, "js_h102_facebookX_getUserID : wrong number of arguments");
    return false;	
}

bool js_h102_facebookX_getName(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    do {
        if (argc == 0) {
            std::string ret = FacebookX::getName();
            jsval jsret = std_string_to_jsval(cx, ret);
            args.rval().set(jsret);
            return true;
        }
    } while (0);

    JS_ReportError(cx, "js_h102_facebookX_getName : wrong number of arguments");
    return false;   
}

bool js_h102_facebookX_isLoggedIn(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
	do {
		if (argc == 0) {
			bool ret = FacebookX::isLoggedIn();
			jsval jsret = BOOLEAN_TO_JSVAL(ret);
			args.rval().set(jsret);
			return true;
		}
	} while (0);

    JS_ReportError(cx, "js_h102_facebookX_isLoggedIn : wrong number of arguments");
    return false;	
}

bool js_h102_facebookX_logout(JSContext *cx, uint32_t argc, jsval *vp) {
	JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
	do {
		if (argc == 0) {
			FacebookX::logout();
      args.rval().setUndefined();
			return true;
		}
	} while (0);

    JS_ReportError(cx, "js_h102_facebookX_logout : wrong number of arguments");
    return false;	
}

bool js_h102_facebookX_getPermissionList(JSContext *cx, uint32_t argc, jsval *vp) {
	JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
	do {
		if (argc == 0) {
			std::vector <std::string> ret = FacebookX::getPermissionList();
			jsval jsret = std_vector_string_to_jsval(cx, ret);
      args.rval().set(jsret);
			return true;
		}
	} while (0);

    JS_ReportError(cx, "js_h102_facebookX_getPermissionList : wrong number of arguments");
    return false;	
}

bool js_h102_facebookX_share(JSContext *cx, uint32_t argc, jsval *vp) {
	JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
	bool ok = true;
	do {
		if (argc == 1) {
			std::map<std::string, std::string> arg0;
            ok &= h102::jsval_to_std_map_string_string(cx, args.get(0), &arg0);
			JSB_PRECONDITION2(ok, cx, false, "js_h102_facebookX_share : Error processing arguments");
			FacebookX::share(map_to_FBShareInfo(arg0));
			args.rval().setUndefined();
			return true;
		}
	} while (0);

    JS_ReportError(cx, "js_h102_facebookX_share : wrong number of arguments");
    return false;
}

bool js_h102_facebookX_shareEncodedContent(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    do {
        if (argc == 1) {
            std::map<std::string, std::string> arg0;
            ok &= h102::jsval_to_std_map_string_string(cx, args.get(0), &arg0);
            JSB_PRECONDITION2(ok, cx, false, "js_h102_facebookX_shareEncodedContent : Error processing arguments");
            FacebookX::shareEncodedContent(map_to_FBShareInfo(arg0));
            args.rval().setUndefined();
            return true;
        }
    } while (0);

    JS_ReportError(cx, "js_h102_facebookX_shareEncodedContent : wrong number of arguments");
    return false;
}

bool js_h102_facebookX_api(JSContext* cx, uint32_t argc, jsval *vp) {
	JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;

	do {
		if (argc == 4) {
	        std::string path;
	        ok &= jsval_to_std_string(cx, args.get(0), &path);
	        std::string method;
	        ok &= jsval_to_std_string(cx, args.get(1), &method);
            std::map<std::string, std::string> param;
	        ok &= h102::jsval_to_std_map_string_string(cx, args.get(2), &param);
	        std::string tag;
	        ok &= jsval_to_std_string(cx, args.get(3), &tag);
	        JSB_PRECONDITION2(ok, cx, false, "js_h102_facebookX_api : Error processing arguments");
	        if (!ok) {ok = true; break;}
	        FacebookX::api(path, method, param, tag);
	        args.rval().setUndefined();
			return true;
		}
	} while (0);

	do {
		if (argc == 3) {
	        std::string path;
	        ok &= jsval_to_std_string(cx, args.get(0), &path);
            std::map<std::string, std::string> param;
	        ok &= h102::jsval_to_std_map_string_string(cx, args.get(1), &param);
	        std::string tag;
	        ok &= jsval_to_std_string(cx, args.get(2), &tag);
	        JSB_PRECONDITION2(ok, cx, false, "js_h102_facebookX_api : Error processing arguments");
	        if (!ok) {ok = true; break;}
	        FacebookX::api(path, param, tag);
	        args.rval().setUndefined();
			return true;
		}
	} while (0);

	do {
		if (argc == 2) {
	        std::string path;
	        ok &= jsval_to_std_string(cx, args.get(0), &path);
	        std::string tag;
	        ok &= jsval_to_std_string(cx, args.get(1), &tag);
	        JSB_PRECONDITION2(ok, cx, false, "js_h102_facebookX_api : Error processing arguments");
	        if (!ok) {ok = true; break;}
	        FacebookX::api(path, tag);
	        args.rval().setUndefined();
			return true;
		}
	} while (0);

    JS_ReportError(cx, "js_h102_facebookX_api : wrong number of arguments");
    return false;
}

bool js_h102_facebookX_shareEncodedOpenGraphStory(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;

    do {
    	if (argc == 3) {
			std::map<std::string, std::string> arg0;
			ok &= h102::jsval_to_std_map_string_string(cx, args.get(0), &arg0);
			JSB_PRECONDITION2(ok, cx, false, "js_h102_facebookX_shareEncodedOpenGraphStory : Error processing arguments");
    		std::string actionType;
    		ok &= jsval_to_std_string(cx, args.get(1), &actionType);
	        std::string tag;
	        ok &= jsval_to_std_string(cx, args.get(2), &tag);
			FacebookX::shareEncodedOpenGraphStory(map_to_FBGraphStoryProperties(arg0), actionType, tag);
			args.rval().setUndefined();
			return true;
    	}
	} while(0);

    JS_ReportError(cx, "js_h102_facebookX_shareEncodedOpenGraphStory : wrong number of arguments");
    return false;
}

bool js_h102_facebookX_canPresentWithFBApp(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;

    do {
	    if (argc == 1) {
	        std::map<std::string, std::string> arg0;
	        ok &= h102::jsval_to_std_map_string_string(cx, args.get(0), &arg0);
	        JSB_PRECONDITION2(ok, cx, false, "js_h102_facebookX_canPresentWithFBApp : Error processing arguments");
	        bool canPresent = FacebookX::canPresentWithFBApp(map_to_FBShareInfo(arg0));

	        jsval jsret = BOOLEAN_TO_JSVAL(canPresent);
	        args.rval().set(jsret);

	        return true;
	    }
	} while (0);

    JS_ReportError(cx, "js_h102_facebookX_canPresentWithFBApp : wrong number of arguments");
    return false;
}

bool js_h102_facebookX_requestInvitableFriends(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    do {
    	if (argc == 1) {
			std::map<std::string, std::string> arg0;
			ok &= h102::jsval_to_std_map_string_string(cx, args.get(0), &arg0);
			JSB_PRECONDITION2(ok, cx, false, "js_h102_facebookX_requestInvitableFriends : Error processing arguments");
			FacebookX::requestInvitableFriends(arg0);
			args.rval().setUndefined();
			return true;
    	}
    } while (0);

	JS_ReportError(cx, "js_PluginFacebookJS_PluginFacebook_requestInvitableFriends : wrong number of arguments");
    return false;
}

bool js_h102_facebookX_inviteFriendsWithInviteIds(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    do {
	    if (argc == 3) {
	        std::vector<std::string> ids;
	        ok &= jsval_to_std_vector_string(cx, args.get(0), &ids);
	        JSB_PRECONDITION2(ok, cx, false, "js_h102_facebookX_inviteFriendsWithInviteIds : Error processing arguments");

	        std::string title;
	        ok &= jsval_to_std_string(cx, args.get(1), &title);
	        JSB_PRECONDITION2(ok, cx, false, "Error processing arguments");

	        std::string text;
	        ok &= jsval_to_std_string(cx, args.get(2), &text);
	        JSB_PRECONDITION2(ok, cx, false, "Error processing arguments");

	        FacebookX::inviteFriendsWithInviteIds(ids, title, text);

	        args.rval().setUndefined();
	        return true;
	    }
	} while (0);

    JS_ReportError(cx, "js_h102_facebookX_inviteFriendsWithInviteIds : wrong number of arguments");
    return false;
}

void js_h102_facebookX_finalize(JSFreeOp *fop, JSObject *obj) {
	CCLOGINFO("jsbinding: finalizing JS object %p (H102::FacebookX)", obj);
}

void js_register_h102_facebookX(JSContext *cx, JS::HandleObject global) {
	jsb_h102_facebookX_class = (JSClass *)calloc(1, sizeof(JSClass));
	jsb_h102_facebookX_class->name = "facebookX";
	jsb_h102_facebookX_class->addProperty = JS_PropertyStub;
	jsb_h102_facebookX_class->delProperty = JS_DeletePropertyStub;
	jsb_h102_facebookX_class->getProperty = JS_PropertyStub;
	jsb_h102_facebookX_class->setProperty = JS_StrictPropertyStub;
	jsb_h102_facebookX_class->enumerate = JS_EnumerateStub;
	jsb_h102_facebookX_class->resolve = JS_ResolveStub;
	jsb_h102_facebookX_class->convert = JS_ConvertStub;
	jsb_h102_facebookX_class->finalize = js_h102_facebookX_finalize;
	jsb_h102_facebookX_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

	static JSPropertySpec properties[] = {
		JS_PSG("__nativeObj", js_is_native_obj, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_PS_END
	};

	static JSFunctionSpec funcs[] = {
		JS_FS_END
	};

	static JSFunctionSpec st_funcs[] = {
        JS_FN("init", js_h102_facebookX_init, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("login", js_h102_facebookX_login, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("getAccessToken", js_h102_facebookX_getAccessToken, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("getUserID", js_h102_facebookX_getUserID, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getName", js_h102_facebookX_getName, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("isLoggedIn", js_h102_facebookX_isLoggedIn, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("logout", js_h102_facebookX_logout, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("getPermissionList", js_h102_facebookX_getPermissionList, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("share", js_h102_facebookX_share, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("shareEncodedContent", js_h102_facebookX_shareEncodedContent, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("api", js_h102_facebookX_api, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("shareEncodedOpenGraphStory", js_h102_facebookX_shareEncodedOpenGraphStory, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("canPresentWithFBApp", js_h102_facebookX_canPresentWithFBApp, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("requestInvitableFriends", js_h102_facebookX_requestInvitableFriends, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("inviteFriendsWithInviteIds", js_h102_facebookX_inviteFriendsWithInviteIds, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("setListener", js_h102_facebookX_setListener, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	};

	jsb_h102_facebookX_prototype = JS_InitClass(
										cx, global,
										JS::NullPtr(),
										jsb_h102_facebookX_class,
										js_h102_facebookX_constructor, 0,
										properties,
										funcs,
										NULL, //no static properties
										st_funcs);

//	TypeTest<FacebookX> t;
//	js_type_class_t *p;
//	std::string typeName = t.s_name();
//	if (_js_global_type_map.find(typeName) == _js_global_type_map.end()) 
//	{
//		p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
//		p->jsclass = jsb_h102_facebookX_class;
//		p->proto = jsb_h102_facebookX_prototype;
//		p->parentProto = NULL;
//		_js_global_type_map.insert(std::make_pair(typeName, p));
//	}
    JS::RootedObject proto(cx, jsb_h102_facebookX_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "FacebookX"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    jsb_register_class<h102::FacebookX>(cx, jsb_h102_facebookX_class, proto, JS::NullPtr());
//    anonEvaluate(cx, global, "(function () { cc.Sprite.extend = cc.Class.extend; })()");
}

void register_all_h102_facebookX(JSContext* cx, JS::HandleObject obj) {
	JS::RootedObject ns(cx);
	get_or_create_js_obj(cx, obj, "h102", &ns);

	js_register_h102_facebookX(cx, ns);
}
