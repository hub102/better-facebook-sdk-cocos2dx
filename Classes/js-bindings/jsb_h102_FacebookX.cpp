//
//  jsb_h102_FacebookX.cpp
//  FacebookXTest
//
//  Created by Thuy Dong Xuan on 4/12/16.
//
//

#include "jsb_h102_FacebookX.hpp"
#include "cocos2d_specifics.hpp"

#include "FacebookX.hpp"

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
    if (dict.find("image") != dict.end())
    {
        info.image = dict.find("image")->second;
    }
    if (dict.find("type") != dict.end())
    {
        std::string type = dict.find("type")->second;
        if (type.compare("link") == 0)
        {
            info.type = sdkbox::FB_LINK;
        }
        else if (type.compare("photo") == 0)
        {
            info.type = sdkbox::FB_PHOTO;
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
}

bool jsval_to_std_map_string_string(JSContext *cx, JS::HandleValue v, std::map<std::string,std::string> *ret)
{
    cocos2d::ValueMap value;
    bool ok = jsval_to_ccvaluemap(cx, v, &value);
    if (!ok)
    {
        return ok;
    }
    else
    {
        for (cocos2d::ValueMap::iterator it = value.begin(); it != value.end(); it++)
        {
            ret->insert(std::make_pair(it->first, it->second.asString()));
        }
    }

    return ok;
}

// bool jsval_to_std_string(JSContext *cx, JS::HandleValue v, std::string* ret) {
//     if(v.isString() || v.isNumber())
//     {
//         JSString *tmp = JS::ToString(cx, v);
//         JSB_PRECONDITION3(tmp, cx, false, "Error processing arguments");

//         JSStringWrapper str(tmp);
//         *ret = str.get();
//         return true;
//     }

//     return false;
// }

// bool jsval_to_std_vector_string(JSContext *cx, JS::HandleValue vp, std::vector<std::string>* ret)
// {
//     JS::RootedObject jsobj(cx);
//     bool ok = vp.isObject() && JS_ValueToObject( cx, vp, &jsobj );
//     JSB_PRECONDITION3( ok, cx, false, "Error converting value to object");
//     JSB_PRECONDITION3( jsobj && JS_IsArrayObject( cx, jsobj),  cx, false, "Object must be an array");
    
//     uint32_t len = 0;
//     JS_GetArrayLength(cx, jsobj, &len);
//     ret->reserve(len);
//     for (uint32_t i=0; i < len; i++)
//     {
//         JS::RootedValue value(cx);
//         if (JS_GetElement(cx, jsobj, i, &value))
//         {
//             if (value.isString())
//             {
//                 JSStringWrapper valueWapper(value.toString(), cx);
//                 ret->push_back(valueWapper.get());
//             }
//             else
//             {
//                 JS_ReportError(cx, "not supported type in array");
//                 return false;
//             }
//         }
//     }
    
//     return true;
// }

static bool js_is_native_obj(JSContext *cx, uint32_t argc, jsval* vp)
{
	JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
	args.rval().setBoolean(true);
	return true;
} 

extern JSClass  *jsb_h102_facebookX_class;
extern JSObject *jsb_h102_facebookX_prototype;

bool *jsb_h102_facebookX_constructor(JSContext *cx, uint32_t argc, jsval* vp)
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

	JS::RootedObject proto(cx, typeClass->proto.get());
	JS::RootedObject parent(cx, typeClass->parentProto.get());
	JS::RootedObject obj(cx, JS_NewObject(cx, typeClass->jsclass, proto, parent));

	js_proxy_t* p = jsb_new_proxy(cobj, obj);
	AddNamedObjectRoot(cx, &p->obj, "FacebookX");
	if (JS_HasPropery(cx, obj, "_ctor", &ok) && ok)
		ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(obj), "_ctor", args);
	return true;
}

bool js_h102_facebookX_init(JSContext *cx, uint32_t argc, jsval *vp) {
	return true;
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
			jsval jsret = JSVAL_NULL;
			jsret = std_string_to_jsval(cx, ret);
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
			std::string ret = FacebookX::getAccessToken();
			jsval jsret = JSVAL_NULL;
			jsret = std_string_to_jsval(cx, ret);
			args.rval().set(jsret);
			return true;
		}
	} while (0);

    JS_ReportError(cx, "js_h102_facebookX_getUserID : wrong number of arguments");
    return false;	
}

bool js_h102_facebookX_isLoggedIn(JSContext *cx, uint32_t argc, jsval *vp) {
	JS::CallArgs args = JS:CallArgsFromVp(argc, vp);
	do {
		if (argc == 0) {
			bool ret = FacebookX::isLoggedIn();
			jsval jsret = JSVAL_NULL;
			jsret = BOOLEAN_TO_JSVAL(ret);
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
			jsval ret = JSVAL_NULL;
			jsret = std_vector_string_to_jsval(cx, ret);
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
			ok &= jsval_to_std_map_string_string(cx, args.get(0), &arg0);
			JSB_PRECONDITION2(ok, cx, false, "js_h102_facebookX_share : Error processing arguments");
			FacebookX::share(map_to_FBShareInfo(arg0));
			args.rval().setUndefinde();
			return true;
		}
	} while (0);

    JS_ReportError(cx, "js_h102_facebookX_share : wrong number of arguments");
    return false;
}

bool js_h102_facebookX_api(JSContext* cx, uint32_t argc, jsval *vp) {
	JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
	bool ok = true

	do {
		if (argc == 4) {
	        std::string path;
	        ok &= jsval_to_std_string(cx, args.get(0), &path);
	        std::string method;
	        ok &= jsval_to_std_string(cx, args.get(1), &method);
            std::map<std::string, std::string> param;
	        ok &= jsval_to_std_map_string_string(cx, args.get(2), &param);
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
	        ok &= jsval_to_std_map_string_string(cx, args.get(1), &param);
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

bool js_h102_facebookX_shareOpenGraphStory(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;

    do {
    	if (argc == 3) {
			std::map<std::string, std::string> arg0;
			ok &= jsval_to_std_map_string_string(cx, args.get(0), &arg0);
			JSB_PRECONDITION2(ok, cx, false, "js_h102_facebookX_shareOpenGraphStory : Error processing arguments");
    		std::string actionType;
    		ok &= jsval_to_std_string(cx, args.get(1), &actionType);
	        std::string tag;
	        ok &= jsval_to_std_string(cx, args.get(2), &tag);
			FacebookX::share(map_to_FBGraphStoryProperties(arg0), actionType, tag);
			args.rval().setUndefinde();
			return true;
    	}
	} while(0);

    JS_ReportError(cx, "js_h102_facebookX_inviteFriendsWithInviteIds : wrong number of arguments");
    return false;
}

bool js_h102_facebookX_canPresentWithFBApp(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;

    do {
	    if (argc == 1) {
	        std::map<std::string, std::string> arg0;
	        ok &= jsval_to_std_map_string_string(cx, args.get(0), &arg0);
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

bool js_h102_facebookX_inviteFriendsWithInviteIds(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
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

        FacebookX::js_h102_facebookX_inviteFriendsWithInviteIds(ids, title, text);

        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_h102_facebookX_inviteFriendsWithInviteIds : wrong number of arguments");
    return false;
}