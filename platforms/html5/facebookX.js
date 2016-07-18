(function(global){"use strict";var _Base64=global.Base64;var version="2.1.7";var buffer;if(typeof module!=="undefined"&&module.exports){buffer=require("buffer").Buffer}var b64chars="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";var b64tab=function(bin){var t={};for(var i=0,l=bin.length;i<l;i++)t[bin.charAt(i)]=i;return t}(b64chars);var fromCharCode=String.fromCharCode;var cb_utob=function(c){if(c.length<2){var cc=c.charCodeAt(0);return cc<128?c:cc<2048?fromCharCode(192|cc>>>6)+fromCharCode(128|cc&63):fromCharCode(224|cc>>>12&15)+fromCharCode(128|cc>>>6&63)+fromCharCode(128|cc&63)}else{var cc=65536+(c.charCodeAt(0)-55296)*1024+(c.charCodeAt(1)-56320);return fromCharCode(240|cc>>>18&7)+fromCharCode(128|cc>>>12&63)+fromCharCode(128|cc>>>6&63)+fromCharCode(128|cc&63)}};var re_utob=/[\uD800-\uDBFF][\uDC00-\uDFFFF]|[^\x00-\x7F]/g;var utob=function(u){return u.replace(re_utob,cb_utob)};var cb_encode=function(ccc){var padlen=[0,2,1][ccc.length%3],ord=ccc.charCodeAt(0)<<16|(ccc.length>1?ccc.charCodeAt(1):0)<<8|(ccc.length>2?ccc.charCodeAt(2):0),chars=[b64chars.charAt(ord>>>18),b64chars.charAt(ord>>>12&63),padlen>=2?"=":b64chars.charAt(ord>>>6&63),padlen>=1?"=":b64chars.charAt(ord&63)];return chars.join("")};var btoa=global.btoa?function(b){return global.btoa(b)}:function(b){return b.replace(/[\s\S]{1,3}/g,cb_encode)};var _encode=buffer?function(u){return(u.constructor===buffer.constructor?u:new buffer(u)).toString("base64")}:function(u){return btoa(utob(u))};var encode=function(u,urisafe){return!urisafe?_encode(String(u)):_encode(String(u)).replace(/[+\/]/g,function(m0){return m0=="+"?"-":"_"}).replace(/=/g,"")};var encodeURI=function(u){return encode(u,true)};var re_btou=new RegExp(["[À-ß][-¿]","[à-ï][-¿]{2}","[ð-÷][-¿]{3}"].join("|"),"g");var cb_btou=function(cccc){switch(cccc.length){case 4:var cp=(7&cccc.charCodeAt(0))<<18|(63&cccc.charCodeAt(1))<<12|(63&cccc.charCodeAt(2))<<6|63&cccc.charCodeAt(3),offset=cp-65536;return fromCharCode((offset>>>10)+55296)+fromCharCode((offset&1023)+56320);case 3:return fromCharCode((15&cccc.charCodeAt(0))<<12|(63&cccc.charCodeAt(1))<<6|63&cccc.charCodeAt(2));default:return fromCharCode((31&cccc.charCodeAt(0))<<6|63&cccc.charCodeAt(1))}};var btou=function(b){return b.replace(re_btou,cb_btou)};var cb_decode=function(cccc){var len=cccc.length,padlen=len%4,n=(len>0?b64tab[cccc.charAt(0)]<<18:0)|(len>1?b64tab[cccc.charAt(1)]<<12:0)|(len>2?b64tab[cccc.charAt(2)]<<6:0)|(len>3?b64tab[cccc.charAt(3)]:0),chars=[fromCharCode(n>>>16),fromCharCode(n>>>8&255),fromCharCode(n&255)];chars.length-=[0,0,2,1][padlen];return chars.join("")};var atob=global.atob?function(a){return global.atob(a)}:function(a){return a.replace(/[\s\S]{1,4}/g,cb_decode)};var _decode=buffer?function(a){return(a.constructor===buffer.constructor?a:new buffer(a,"base64")).toString()}:function(a){return btou(atob(a))};var decode=function(a){return _decode(String(a).replace(/[-_]/g,function(m0){return m0=="-"?"+":"/"}).replace(/[^A-Za-z0-9\+\/]/g,""))};var noConflict=function(){var Base64=global.Base64;global.Base64=_Base64;return Base64};global.Base64={VERSION:version,atob:atob,btoa:btoa,fromBase64:decode,toBase64:encode,utob:utob,encode:encode,encodeURI:encodeURI,btou:btou,decode:decode,noConflict:noConflict};if(typeof Object.defineProperty==="function"){var noEnum=function(v){return{value:v,enumerable:false,writable:true,configurable:true}};global.Base64.extendString=function(){Object.defineProperty(String.prototype,"fromBase64",noEnum(function(){return decode(this)}));Object.defineProperty(String.prototype,"toBase64",noEnum(function(urisafe){return encode(this,urisafe)}));Object.defineProperty(String.prototype,"toBase64URI",noEnum(function(){return encode(this,true)}))}}})(this);if(this["Meteor"]){Base64=global.Base64}

var h102 = h102 || {};
h102.facebookX = h102.facebookX || {};
	
h102.facebookX._userId = "";
h102.facebookX._accessToken = "";
h102.facebookX._isLoggedIn = false;
h102.facebookX._persmissions = null;
h102.facebookX.onLogin = null
h102.facebookX.onRequestInvitableFriends = null;
h102.facebookX.onInviteFriendsWithInviteIdsResult = null;
h102.facebookX.onSharedSuccess = null;
h102.facebookX.onSharedFailed = null;
h102.facebookX.onSharedCancel = null;
h102.facebookX.onGetUserInfo = null;
h102.facebookX.onAPI = null;
h102.facebookX.onAPIFailed = null;

h102.facebookX._cachedAPICalled = [];

h102.facebookX.init = h102.facebookX.init || function() {
    FB.init({
        appId      : 994902593934215,
        cookie     : true,
        status     : true,
        xfbml      : true,
        version    : 'v2.4' // or v2.0, v2.1, v2.2, v2.3
    });
	h102.facebookX._getLoginStatus();
};

h102.facebookX._getLoginStatus = h102.facebookX._getLoginStatus || function() {
	var callback = h102.facebookX.onGetUserInfo;
	FB.Event.subscribe('auth.statusChange', function(response) {
		if (response.status == 'connected') {
			h102.facebookX._userId = response.authResponse.userID;
			h102.facebookX._accessToken = response.authResponse.accessToken;
			h102.facebookX._isLoggedIn = true;
			callback && callback(response);
			cc.log("accessToken mới = " + h102.facebookX._accessToken);
			h102.facebookX._apiQueued();
		} else if (response.status == 'not_authorized') {
			callback && callback(false);
		} else {
			callback && callback(false);
		}
	});
};

h102.facebookX._apiQueued = h102.facebookX._apiQueued || function() {
	for (var i = 0; i < h102.facebookX._cachedAPICalled.length; i++) {
		var api = h102.facebookX._cachedAPICalled[i];
		var path = api['_path'];
		var method = api['_method'];
		var params = api['_params'];
		var callback = api['_callback'];
		FB.api(path, method, params, callback);
	}

	h102.facebookX._cachedAPICalled = [];
}

h102.facebookX.login = h102.facebookX.login || function(permissions) {
    var callbackLogin = h102.facebookX.onLogin;
    var callbackInfo = h102.facebookX.onGetUserInfo;

    if (h102.facebookX._isLoggedIn) {
        cc.log("facebookX: User is already logged in.");
        FB.api("/me", function(_response) {
            cc.log("facebookX: Good to see you, " + _response.name);
            callbackLogin && callbackLogin(true, "User is already logged in");
            callbackInfo && callbackInfo(_response);
        });        
        return;
    }

    FB.login(function(response) {
        if (response.authResponse) {
            cc.log("facebookX: Welcome! Fetching your infomation.... ");
            h102.facebookX._userId = response.authResponse.userID;
            h102.facebookX._accessToken = response.authResponse.accessToken;
            h102.facebookX._isLoggedIn = true;
            FB.api("/me", function(_response) {
                cc.log("facebookX: Good to see you, " + _response.name);
                callbackLogin && callbackLogin(true, response);
                callbackInfo && callbackInfo(_response);
            });
            h102.facebookX._persmissions = permissions;
        } else {
            cc.log("facebookX: User cancelled login or did not fully authorized.");
            callbackLogin && callbackLogin(false);
            callbackInfo && callbackInfo(false);
        }
    }, {
        scope: permissions,
        return_scopes: true
    });
};

h102.facebookX.logout = h102.facebookX.logout || function() {
    h102.facebookX._isLoggedIn = false;
	FB.logout(function(response) {
		cc.log("facebookX: Logged out.");
	});
};

h102.facebookX.isLoggedIn = h102.facebookX.isLoggedIn || function() {
	return h102.facebookX._isLoggedIn;
};

h102.facebookX.getUserID = h102.facebookX.getUserID || function() {
	return h102.facebookX._userId;
};

h102.facebookX.getAccessToken = h102.facebookX.getAccessToken || function() {
	return h102.facebookX._accessToken;
};

h102.facebookX.getPermissionList = h102.facebookX.getPermissionList || function() {
	return h102.facebookX._persmissions;
};

h102.facebookX.share =  function(info) {
	if (cc.sys.isNative) {
        if (info['text']) info['text'] = Base64.encode(info['text']);
        if (info['title']) info['title'] = Base64.encode(info['title']);
        if (info['link']) info['link'] = Base64.encode(info['link']);
        if (info['media']) info['media'] = Base64.encode(info['media']);
        h102.facebookX.shareEncodedContent(info);
    } else {
        //TODO: implement Web version sharing
    }
};

h102.facebookX.canPresentDialog = h102.facebookX.canPresentDialog || function(info) {
    if (info && info['dialog'] && (
        info['dialog'] === 'shareLink' ||
            info['dialog'] === 'feedDialog' ||
            info['dialog'] === 'shareOpenGraph' ||
            info['dialog'] === 'messageLink')
    ) {
    	return true;
   	} else {
        return false;
   	}
};

h102.facebookX.dialog = h102.facebookX.dialog || function (info) {
	var callbackOK = h102.facebookX.onSharedSuccess;
	var callbackFailed = h102.facebookX.onSharedFailed;

    if (!info) {
        typeof callbackFailed === 'function' && callbackFailed({
            error_message: "No info parameter provided"
        });
        return;
    }

    if (!h102.facebookX.canPresentDialog(info)) {
        typeof callbackFailed === 'function' && callbackFailed({
            error_message: "The requested dialog: " + info['dialog'] + " can not be presented on Web"
        });
        return;
    }

    // Preprocess properties
    info['name'] = info['name'] || info['site'];
    delete info['site'];

    info['href'] = info['href'] || info['link'] || info['siteUrl'];
    delete info['siteUrl'];
    delete info['link'];

    info['picture'] = info['picture'] || info['image'] || info['photo'] || info['imageUrl'] || info['imagePath'];
    delete info['imageUrl'];
    delete info['imagePath'];
    delete info['photo'];
    delete info['image'];

    info['caption'] = info['title'] || info['caption'];
    delete info['title'];

    info['description'] = info['text'] || info['description'];
    delete info['text'];

    var method = info['dialog'];
    delete info['dialog'];

    if (method === 'shareLink' || method == 'feedDialog') {
        info['method'] = 'share';
    } else if (method == 'messageLink') {
        info['method'] = 'send';
        info['link'] = info['href'];
    } else if (method == 'shareOpenGraph') {
        info['method'] = 'share_open_graph';

        if (info['url']) {
            var obj = {};
            if (info["preview_property_name"])
                obj[info["preview_property_name"]] = info["url"];
            else
                obj["object"] = info["url"];

            for (var p in info) {
                if (p != "method" && p != "action_type" && p != "action_properties") {
                    info[p] && (obj[p] = info[p]);
                    delete info[p];
                }
            }

            info['action_properties'] = JSON.stringify(obj);
        }
    }

    FB.ui(info,
        function (response) {
            if (response && typeof callbackOK === 'function') {
                if (response['post_id'] || response['success']) {
                    callbackOK({
                        didComplete: true,
                        post_id: response['post_id'] || ""
                    });
                }
                else {
                    if (response['error_code']) {
                         callbackFailed({
                             error_message : response['error_message'] || 'Unknown error'
                         });
                    }
                    else callback(response);
                }
            } else if (response == undefined && typeof callbackFailed === 'function') {
                callbackFailed({
                    error_message: "Unknown error"
                });
            }
        }
	);
};

h102.facebookX.requestInvitableFriends = h102.facebookX.requestInvitableFriends || function(flags) {
	var callback = h102.facebookX.onRequestInvitableFriends;
	FB.api(
		"/me/invitable_friends",
		"GET",
		flags,
		function(response) {
			if (response && !response.error) {
				callback && callback(response);
			} else {
				callback && callback(false);
			}
		}
	);
};

h102.facebookX.inviteFriendsWithInviteIds = h102.facebookX.inviteFriendsWithInviteIds || function(ids, inviteTitle, invitationText) {
	var callback = h102.facebookX.onInviteFriendsWithInviteIdsResult;
    var tokensString = '';
    for (var i = 0; i < ids.length; i++) {
        tokensString += (ids[i] + ", ");
    }

    tokensString = tokensString.substring(0, tokensString.length - 2); 

    FB.ui({
		method: 'apprequests',
		message: invitationText,
		title: inviteTitle,
		to: tokensString
    },
    function(response) {
    	if (response && !response.error_code) {
    		callback && callback(true, response);
    	} else {
    		callback && callback(false);
    	}
    });
};

h102.facebookX.setListener = h102.facebookX.setListener || function(listeners) {
	h102.facebookX.onLogin = listeners['onLogin'] || h102.facebookX.onLogin;
	h102.facebookX.onRequestInvitableFriends = listeners['onRequestInvitableFriends'] || h102.facebookX.onRequestInvitableFriends;
	h102.facebookX.onInviteFriendsWithInviteIdsResult = listeners['onInviteFriendsWithInviteIdsResult'] || h102.facebookX.onInviteFriendsWithInviteIdsResult;
	h102.facebookX.onSharedSuccess = listeners['onSharedSuccess'] || h102.facebookX.onSharedSuccess;
	h102.facebookX.onSharedFailed = listeners['onSharedFailed'] || h102.facebookX.onSharedFailed;
	h102.facebookX.onSharedCancel = listeners['onSharedCancel'] || h102.facebookX.onSharedCancel;
	h102.facebookX.onGetUserInfo = listeners['onGetUserInfo'] || h102.facebookX.onGetUserInfo;
	h102.facebookX.onAPI = listeners['onAPI'] || h102.facebookX.onAPI;
    h102.facebookX.onAPI = listeners['onAPIFailed'] || h102.facebookX.onAPIFailed; 
};

h102.facebookX.api = h102.facebookX.api || function(path, method, params, tag) {
	cc.log("called api: path = " + path + "; method = " + method + "; params = " + params + "; tag = " + tag);
	var callback = h102.facebookX.onAPI;
    var callbackFailed = h102.facebookX.onAPIFailed;
	if (!h102.facebookX._isLoggedIn) {
		h102.facebookX._cachedAPICalled.push({
			_path: path,
			_method: method,
			_params: params,
			_tag: tag,
			_callback: function(response) {
				cc.log("test response = " + JSON.stringify(response));
				if (response && !response.error) {
					callback && callback(tag, response);
				} else {
					callbackFailed && callbackFailed(tag, response);
				}
			}
		})
	} else {
		FB.api(
			path,
			method,
			params,
			function(response) {
				cc.log("facebookX_api: response = " + JSON.stringify(response));
				if (response && !response.error) {
					callback && callback(tag, response);
				} else {
					callbackFailed && callbackFailed(tag, response);
				}
			}
		);
	}
};

h102.facebookX.shareOpenGraphStory = h102.facebookX.shareOpenGraphStory || function(info, actionType, previewPropertyName) {
	if (cc.sys.isNative) {
		info["url"] = Base64.encode(info["url"]);
		info["type"] = Base64.encode(info["type"]);
		info["title"] = Base64.encode(info["title"]);
		info["image"] = Base64.encode(info["image"]);
		info["description"] = Base64.encode(info["description"]);
		actionType = Base64.encode(actionType);
		previewPropertyName = Base64.encode(previewPropertyName);
		h102.facebookX.shareEncodedOpenGraphStory(info, actionType, previewPropertyName);
	} else {
        var ogType = info['type'];
        var propertyName = ogType.substring(ogType.indexOf(':') + 1);
        var callbackOK = h102.facebookX.onSharedSuccess;
        var callbackFailed = h102.facebookX.onSharedFailed;
        h102.facebookX.setListener({
            onAPI: function(tag, msg) {
                // if (tag == 'me/joselitopuzzle_test:beat') {
            	if (tag == 'me/' + actionType) {
                    if (msg) {
                        var tmpId = msg['id'];
                        h102.facebookX.setListener({
                            onAPI: function(tag, msg) {
                                if (tag == '/' + tmpId) {
                                    if (msg) {
                                        var lastId = msg['data']['level']['id'];
                                        var obj1 = {};
                                        obj1[propertyName] = lastId;
                                        cc.log("test obj1 = " + JSON.stringify(obj1));
                                        var infoToShare = {
                                            "dialog": "shareOpenGraph",
                                            "object_type": info['type'], 
                                            "title": 'title',
                                            "url": info['url'],
                                            "description": 'description',
                                            "action_type": actionType,
                                        };
                                        infoToShare[previewPropertyName] = lastId;
                                        h102.facebookX.dialog(infoToShare);
                                        
                                    } else {
                                        cc.log("FAILED to get lastID");
                                        callback && callback(false, true);
                                    }
                                }
                            }
                        });

                        h102.facebookX.api('/' + tmpId, 'GET', {}, '/' + tmpId);

                    } else {
                        cc.log("FAILED to get tmpId");
                        cc.log("MSG = " + JSON.stringify(msg));
                        callback && callback(false, true);
                    }
                } 
            }
        });

        var obj = {};
        obj[propertyName] = {
            'og:title': info['title'],
            'og:image': info['image'],
            'og:url': info['url'],
            'og:type': info['type'],
            'og:description': info['description']
        };

        h102.facebookX.api(
            'me/' + actionType,
            'POST', 
            obj,
            'me/' + actionType
        );
	}
};

// getSDKVersion: function() {

// },

// fetchFriends: function() {

// },

// canPresentWithFBApp: function() {

// },

// requestReadPermission: function(permissions) {

// },

// requestPublishPermissions: function(permissions) {

// },

// inviteFriends: function(appLinkURL, previewImageURL) {

// },

// setAppId: function(appId) {

// },

// onPermission: function(isLogin, msg) {

// },

// onFetchFriends: function(ok, msg) {

// },

// onInviteFriendsResult: function(result, msg){

// }
