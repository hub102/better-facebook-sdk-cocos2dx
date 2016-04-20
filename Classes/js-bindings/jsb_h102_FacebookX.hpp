//
//  jsb_h102_FacebookX.hpp
//  FacebookXTest
//
//  Created by Thuy Dong Xuan on 4/12/16.
//
//

#include "base/ccConfig.h"
#ifndef jsb_h102_FacebookX_hpp
#define jsb_h102_FacebookX_hpp

#include "jsapi.h"
#include "jsfriendapi.h"

extern JSClass  *jsb_h102_facebookX_class;
extern JSObject *jsb_h102_facebookX_prototype;

bool js_h102_facebookX_constructor(JSContext *cx, uint32_t argc, jsval *vp);
bool js_h102_facebookX_init(JSContext *cx, uint32_t argc, jsval *vp);
bool js_h102_facebookX_setListener(JSContext *cx, uint32_t argc, jsval *vp);
bool js_h102_facebookX_login(JSContext *cx, uint32_t argc, jsval *vp);
bool js_h102_facebookX_getAccessToken(JSContext *cx, uint32_t argc, jsval *vp);
bool js_h102_facebookX_getUserID(JSContext *cx, uint32_t argc, jsval *vp);
bool js_h102_facebookX_getName(JSContext *cx, uint32_t argc, jsval *vp);
bool js_h102_facebookX_isLoggedIn(JSContext *cx, uint32_t argc, jsval *vp);
bool js_h102_facebookX_logout(JSContext *cx, uint32_t argc, jsval *vp);
bool js_h102_facebookX_getPermissionList(JSContext *cx, uint32_t argc, jsval *vp);
bool js_h102_facebookX_share(JSContext *cx, uint32_t argc, jsval *vp);
bool js_h102_facebookX_api(JSContext *cx, uint32_t argc, jsval *vp);
bool js_h102_facebookX_shareEncodedOpenGraphStory(JSContext *cx, uint32_t argc, jsval *vp);
bool js_h102_facebookX_canPresentWithFBApp(JSContext *cx, uint32_t argc, jsval *vp);
bool js_h102_facebookX_requestInvitableFriends(JSContext *cx, uint32_t argc, jsval *vp);
bool js_h102_facebookX_inviteFriendsWithInviteIds(JSContext *cx, uint32_t argc, jsval *vp);

void register_all_h102_facebookX(JSContext* cx, JS::HandleObject obj);

#endif /* jsb_h102_FacebookX_hpp */
