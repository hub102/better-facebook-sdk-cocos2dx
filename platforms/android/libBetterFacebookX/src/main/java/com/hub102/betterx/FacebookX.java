package com.hub102.betterx;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;

import com.facebook.AccessToken;
import com.facebook.AccessTokenTracker;
import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.FacebookSdk;
import com.facebook.GraphRequest;
import com.facebook.GraphResponse;
import com.facebook.HttpMethod;
import com.facebook.LoggingBehavior;
import com.facebook.appevents.AppEventsLogger;
import com.facebook.login.LoginManager;
import com.facebook.share.ShareApi;
import com.facebook.share.Sharer;
import com.facebook.share.model.AppInviteContent;
import com.facebook.share.model.GameRequestContent;
import com.facebook.share.model.ShareLinkContent;
import com.facebook.share.model.SharePhoto;
import com.facebook.share.model.SharePhotoContent;
import com.facebook.share.widget.AppInviteDialog;
import com.facebook.share.widget.GameRequestDialog;
import com.facebook.share.widget.ShareDialog;

import org.json.JSONObject;

import java.util.Arrays;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

/**
 * Created by nick on 4/12/16.
 */
public class FacebookX {
    private Context mContext;
    private CallbackManager callbackManager;
    private ShareDialog shareDialog;
    private AppInviteDialog inviteDialog;
    private GameRequestDialog requestDialog;
    private AccessTokenTracker accessTokenTracker = null;
    private boolean _userInfoRequested = false;

    public static final String PLUGIN_PRIVATE_DATA_KEY = "com.sdkbox.fb";
    public static final String LOGIN_ACCESS_TOKEN_SET = "access_token";
    public static final String LOGIN_REQUESTED_PERMISSIONS = "login_permissions";
    public static final String LOGIN_DEFAULT_REQUESTED_PERMISSIONS = "public_profile";

    private static boolean mDebug = true;
    static final String TAG = "FacebookX";

    static final int FBSDKShareTypeAPI = 1;
    static final int FBSDKShareTypeDialog = 2;
    static final int FB_NONE = 0;
    static final int FB_LINK = 1;
    static final int FB_PHOTO = 2;
    static final int kFBLoginCallback = 1;
    static final int kFBPermissionCallback = 2;

    public FacebookX(Context ctx) {
        this.mContext = ctx;
    }

    private Activity getActivity() {
        return (Activity)this.mContext;
    }

    public void configure() {
        FacebookSdk.sdkInitialize(this.mContext);
//        SDKBox.addListener(this);
        this.callbackManager = CallbackManager.Factory.create();
        LoginManager.getInstance().registerCallback(this.callbackManager, new FacebookCallback() {
            public void onSuccess(Object o) {
            }

            public void onCancel() {
                if(FacebookX.this.isLoggedIn()) {
                    FacebookX.this.onPermissionWrapper(false, "cancel");
                } else {
                    FacebookX.this.onLoginWrapper(false, "cancel");
                }

            }

            public void onError(FacebookException exception) {
                if(FacebookX.this.isLoggedIn()) {
                    FacebookX.this.onPermissionWrapper(false, exception.toString());
                } else {
                    FacebookX.this.onLoginWrapper(false, exception.toString());
                }

            }
        });

        try {
            BetterX.runOnMainThread(new Runnable() {
                public void run() {
                    FacebookX.this.shareDialog = new ShareDialog(FacebookX.this.getActivity());
                    FacebookX.this.shareDialog.registerCallback(FacebookX.this.callbackManager, new FacebookCallback() {
                        public void onSuccess(Object o) {
                            Sharer.Result result = (Sharer.Result)o;
                            String ret = "{\"postId\":\"" + result.getPostId() + "\"}";
                            FacebookX.this.onSharedSuccessWrapper(ret);
                        }

                        public void onCancel() {
                            FacebookX.this.onSharedCancelWrapper();
                        }

                        public void onError(FacebookException exception) {
                            FacebookX.this.onSharedFailedWrapper(exception.toString());
                        }
                    });
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        this.requestDialog = new GameRequestDialog(this.getActivity());
        this.requestDialog.registerCallback(this.callbackManager, new FacebookCallback() {
            public void onSuccess(Object o) {
                GameRequestDialog.Result result = (GameRequestDialog.Result)o;
                String id = result.getRequestId();
                FacebookX.onInviteFriendsWithInviteIdsResult(true, id);
            }

            public void onCancel() {
                FacebookX.onInviteFriendsWithInviteIdsResult(false, "Cancel");
            }

            public void onError(FacebookException error) {
                FacebookX.onInviteFriendsWithInviteIdsResult(false, error.getMessage());
            }
        });
        this.inviteDialog = new AppInviteDialog(this.getActivity());
        this.inviteDialog.registerCallback(this.callbackManager, new FacebookCallback() {
            public void onSuccess(Object o) {
                FacebookX.onInviteFriendsResult(true, "ok");
            }

            public void onCancel() {
                FacebookX.onInviteFriendsResult(false, "Cancel");
            }

            public void onError(FacebookException error) {
                FacebookX.onInviteFriendsResult(false, error.getMessage());
            }
        });
        this.accessTokenTracker = new AccessTokenTracker() {
            protected void onCurrentAccessTokenChanged(AccessToken oldAccessToken, AccessToken currentAccessToken) {
//                FacebookX.LogD("FB onCurrentAccessTokenChanged: oldAccessToken " + oldAccessToken + " currentAccessToken " + currentAccessToken);
                if(oldAccessToken != null && currentAccessToken != null) {
                    FacebookX.this.onPermissionWrapper(true, "success");
                } else if(oldAccessToken == null && currentAccessToken != null && !currentAccessToken.isExpired()) {
                    FacebookX.this.onLoginWrapper(true, "success");
                }

            }
        };
    }

    public void logInWithPublishPermissions(String permissions, int callbackType) {
        if(permissions.length() > 0) {
            LoginManager.getInstance().logInWithPublishPermissions(this.getActivity(), Arrays.asList(permissions.split(",")));
        }

    }

    public void logInWithReadPermissions(String permissions, int callbackType) {
        if(permissions.length() > 0) {
            if(!this.isLoggedIn()) {
                Log.d("FacebookX", "Login with permissions: \'" + permissions + "\'");
            } else {
                Log.d("FacebookX", "Requesting permissions: \'" + permissions + "\'");
            }

            LoginManager.getInstance().logInWithReadPermissions(this.getActivity(), Arrays.asList(permissions.split(",")));
        }

    }

    public void share(final Map<String, String> info, final int type) {
        try {
            final int e = Integer.parseInt((String)getOrDefault(info, "type", "0"));
            if(e == 0) {
                return;
            }

            BetterX.runOnMainThread(new Runnable() {
                public void run() {
                    if(e == 1) {
                        ShareLinkContent image = ((ShareLinkContent.Builder)(new ShareLinkContent.Builder()).setContentUrl(Uri.parse((String)FacebookX.getOrDefault(info, "link", "")))).setContentTitle((String)FacebookX.getOrDefault(info, "title", "")).setContentDescription((String)FacebookX.getOrDefault(info, "text", "")).setImageUrl(Uri.parse((String)FacebookX.getOrDefault(info, "image", ""))).build();
                        if(type == 1) {
                            ShareApi.share(image, new FacebookCallback() {
                                public void onSuccess(Object o) {
                                    Sharer.Result result = (Sharer.Result)o;
                                    String ret = "{\"postId\":\"" + result.getPostId() + "\"}";
                                    FacebookX.this.onSharedSuccessWrapper(ret);
                                }

                                public void onCancel() {
                                    FacebookX.this.onSharedCancelWrapper();
                                }

                                public void onError(FacebookException exception) {
                                    FacebookX.this.onSharedFailedWrapper(exception.toString());
                                }
                            });
                        } else if(type == 2) {
                            FacebookX.this.shareDialog.show(image);
                        }
                    } else if(e == 2) {
                        Bitmap image1 = BitmapFactory.decodeFile((String)FacebookX.getOrDefault(info, "image", ""));
                        SharePhoto photo = (new com.facebook.share.model.SharePhoto.Builder()).setBitmap(image1).build();
                        SharePhotoContent content = (new com.facebook.share.model.SharePhotoContent.Builder()).addPhoto(photo).build();
                        if(type == 1) {
                            ShareApi.share(content, new FacebookCallback() {
                                public void onSuccess(Object o) {
                                    Sharer.Result result = (Sharer.Result)o;
                                    String ret = "{\"postId\":\"" + result.getPostId() + "\"}";
                                    FacebookX.this.onSharedSuccessWrapper(ret);
                                }

                                public void onCancel() {
                                    FacebookX.this.onSharedCancelWrapper();
                                }

                                public void onError(FacebookException exception) {
                                    FacebookX.this.onSharedFailedWrapper(exception.toString());
                                }
                            });
                        } else if(type == 2) {
                            FacebookX.this.shareDialog.show(content);
                        }
                    }

                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    public void getUserInfo() {
        if(!this._userInfoRequested) {
            this._userInfoRequested = true;
            Bundle parameters = new Bundle();
            parameters.putString("fields", "id,name,first_name,last_name,installed,email");
            GraphRequest.Callback callback = new GraphRequest.Callback() {
                public void onCompleted(GraphResponse response) {
                    FacebookX.onUserInfo(response.getRawResponse());
                }
            };
            GraphRequest request = new GraphRequest(AccessToken.getCurrentAccessToken(), "me", parameters, HttpMethod.GET, callback);
            request.executeAsync();
        }
    }

    public void api(String path, String method, Map<String, String> params, final String tag) {
        Bundle parameters = new Bundle();
        Iterator httpMethod = params.keySet().iterator();

        while(httpMethod.hasNext()) {
            String callback = (String)httpMethod.next();
            parameters.putString(callback, (String)params.get(callback));
        }

        HttpMethod httpMethod1 = HttpMethod.GET;
        if(method.equals("POST")) {
            httpMethod1 = HttpMethod.POST;
        } else if(method.equals("DELETE")) {
            httpMethod1 = HttpMethod.DELETE;
        }

        GraphRequest.Callback callback1 = new GraphRequest.Callback() {
            public void onCompleted(GraphResponse response) {
                try {
                    int errorCode = response.getError() == null?0:response.getError().getErrorCode();
                    String errorMessage = response.getError() == null?"":response.getError().getErrorMessage().replace("java.net.SocketTimeoutException: ", "");
                    String responseText = response.getJSONObject() == null?"":response.getJSONObject().toString();
                    if(tag.equals("__fb_me_friends__")) {
                        try {
                            String jsone = response.getJSONObject().getString("data");
                            FacebookX.this.onFetchFriendsWrapper(true, jsone);
                        } catch (Exception e) {
                            e.printStackTrace();

                            try {
                                FacebookX.this.onFetchFriendsWrapper(false, (new JSONObject()).put("error", errorMessage).put("code", String.valueOf(errorCode)).toString());
                            } catch (Exception e1) {
                                e1.printStackTrace();
                            }
                        }
                    } else if(errorCode == 0) {
                        FacebookX.this.onAPIWrapper(tag, responseText);
                    } else {
                        try {
                            FacebookX.this.onAPIWrapper(tag, (new JSONObject()).put("error", errorMessage).put("code", String.valueOf(errorCode)).toString());
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                } catch (Exception e) {
                    ;
                }

            }
        };
        final GraphRequest request = new GraphRequest(AccessToken.getCurrentAccessToken(), path, parameters, httpMethod1, callback1);

        try {
            BetterX.runOnMainThread(new Runnable() {
                public void run() {
                    request.executeAsync();
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    public boolean isLoggedIn() {
        return this.getCurrentAccessToken().length() > 0;
    }

    public String getCurrentAccessToken() {
        String tokenString = "";
        AccessToken token = AccessToken.getCurrentAccessToken();
        if(token != null) {
            tokenString = token.getToken();
        }

        return tokenString;
    }

    public String getPermissionList() {
        String permissions = "";
        AccessToken token = AccessToken.getCurrentAccessToken();
        if(token != null) {
            permissions = this.convertSetString2String(token.getPermissions());
        }

        return permissions;
    }

    public String getUserID() {
        String userId = "";
        AccessToken token = AccessToken.getCurrentAccessToken();
        if(token != null) {
            userId = token.getUserId();
        }

        return userId;
    }

    public void enableDebug(boolean enable) {
        mDebug = enable;
        if(mDebug) {
            FacebookSdk.addLoggingBehavior(LoggingBehavior.REQUESTS);
        }

    }

    public void logout() {
        LoginManager.getInstance().logOut();
    }

    public String getSDKVersion() {
        return FacebookSdk.getSdkVersion();
    }

    public boolean canPresent(Map<String, String> info) {
        int shareType = Integer.parseInt((String)getOrDefault(info, "type", "0"));
        if(shareType == 0) {
            return false;
        } else if(shareType == 1) {
            ShareLinkContent image1 = ((ShareLinkContent.Builder)(new ShareLinkContent.Builder()).setContentUrl(Uri.parse((String)getOrDefault(info, "link", "")))).setContentTitle((String)getOrDefault(info, "title", "")).setContentDescription((String)getOrDefault(info, "text", "")).setImageUrl(Uri.parse((String)getOrDefault(info, "image", ""))).build();
            return this.shareDialog.canShow(image1, ShareDialog.Mode.NATIVE);
        } else if(shareType == 2) {
            Bitmap image = BitmapFactory.decodeFile((String)getOrDefault(info, "image", ""));
            SharePhoto photo = (new com.facebook.share.model.SharePhoto.Builder()).setBitmap(image).build();
            SharePhotoContent content = (new com.facebook.share.model.SharePhotoContent.Builder()).addPhoto(photo).build();
            return this.shareDialog.canShow(content, ShareDialog.Mode.NATIVE);
        } else {
            return false;
        }
    }

    public void requestInvitableFriends() {
        this.requestInvitableFriends((Map)null);
    }

    public void requestInvitableFriends(Map<String, String> bundle) {
        HashMap params = new HashMap();
        StringBuilder req = new StringBuilder();
        req.append("me/invitable_friends");
        if(bundle != null && bundle.size() > 0) {
            String _fields = (String)bundle.get("ResponseFields");
            if(_fields != null) {
                params.put("fields", _fields);
            } else {
                params.put("fields", "name,id,picture");
            }

            String e = (String)bundle.get("PictureSize");
            if(e != null) {
                _fields = (String)params.get("fields");
                if(null == _fields) {
                    _fields = "";
                } else {
                    _fields = _fields + ",";
                }

                _fields = _fields + "picture.width(" + e + ")s";
                params.put("fields", _fields);
            }

            String _friendsLimit = (String)bundle.get("PaginationLimit");
            if(_friendsLimit != null) {
                params.put("limit", _friendsLimit);
            }

            String _excludeIds = (String)bundle.get("ExcludeFromListIds");
            if(_excludeIds != null) {
                params.put("excluded_ids", "[" + _excludeIds + "]");
            }
        } else {
            params.put("fields", "name,picture.width(300)");
        }

        if(params.size() > 0) {
            req.append("?");
            Iterator _fields1 = params.entrySet().iterator();

            while(_fields1.hasNext()) {
                Map.Entry e1 = (Map.Entry)_fields1.next();
                req.append((String)e1.getKey());
                req.append("=");
                req.append((String)e1.getValue());
                req.append("&");
            }
        }

        (new GraphRequest(AccessToken.getCurrentAccessToken(), req.toString(), (Bundle)null, HttpMethod.GET, new GraphRequest.Callback() {
            public void onCompleted(GraphResponse response) {
                FacebookX.this.onRequestInvitableFriendsWrapper(response.getRawResponse());
            }
        })).executeAsync();
    }

    public void inviteFriends(String app_link_url, String preview_image_url) {
        if(AppInviteDialog.canShow()) {
            AppInviteContent content = (new com.facebook.share.model.AppInviteContent.Builder()).setApplinkUrl(app_link_url).setPreviewImageUrl(preview_image_url).build();
            this.inviteDialog.show(content);
        }

    }

    public void inviteFriendsWithInviteIds(String[] invite_ids, String title, String text) {
        if(invite_ids != null && invite_ids.length != 0) {
            StringBuilder sb = new StringBuilder();

            for(int id = 0; id < invite_ids.length; ++id) {
                sb.append(invite_ids[id]);
                if(id < invite_ids.length - 1) {
                    sb.append(",");
                }
            }

            GameRequestContent content = (new GameRequestContent.Builder())
                    .setMessage(text)
                    .setTitle(title)
                    .setTo(sb.toString())
                    .build();
            this.requestDialog.show(content);
        }
    }

    private void onRequestInvitableFriendsWrapper(final String invitableFriendsData) {
        BetterX.runOnGLThread(new Runnable() {
            public void run() {
                FacebookX.onRequestInvitableFriends(invitableFriendsData);
            }
        });
    }

    private void onLoginWrapper(final boolean hasLogin, final String data) {
        BetterX.runOnGLThread(new Runnable() {
            public void run() {
                FacebookX.onLogin(hasLogin, data);
            }
        });
    }

    private void onAPIWrapper(final String apiPath, final String data) {
        BetterX.runOnGLThread(new Runnable() {
            public void run() {
                FacebookX.onAPI(apiPath, data);
            }
        });
    }

    private void onFetchFriendsWrapper(final boolean ok, final String data) {
        BetterX.runOnGLThread(new Runnable() {
            public void run() {
                FacebookX.onFetchFriends(ok, data);
            }
        });
    }

    private void onSharedSuccessWrapper(final String data) {
        BetterX.runOnGLThread(new Runnable() {
            public void run() {
                FacebookX.onSharedSuccess(data);
            }
        });
    }

    private void onSharedFailedWrapper(final String data) {
        BetterX.runOnGLThread(new Runnable() {
            public void run() {
                FacebookX.onSharedFailed(data);
            }
        });
    }

    private void onSharedCancelWrapper() {
        BetterX.runOnGLThread(new Runnable() {
            public void run() {
                FacebookX.onSharedCancel();
            }
        });
    }

    private void onPermissionWrapper(final boolean hasLogin, final String data) {
        BetterX.runOnGLThread(new Runnable() {
            public void run() {
                FacebookX.onPermission(hasLogin, data);
            }
        });
    }

    private static <K, V> V getOrDefault(Map<K, V> map, K key, V defaultValue) {
        return map.containsKey(key)?map.get(key):defaultValue;
    }

    private String convertSetString2String(Set<String> permissionSet) {
        StringBuilder strBuilder = new StringBuilder();
        int index = 0;
        Iterator iter = permissionSet.iterator();

        while(iter.hasNext()) {
            String permission = (String)iter.next();
            if(index > 0) {
                strBuilder.append(",");
            }

            ++index;
            strBuilder.append(permission);
        }

        return strBuilder.toString();
    }

    public void onStart() {
    }

    public void onStop() {
    }

    public void onResume() {
        AppEventsLogger.activateApp(this.getActivity());
    }

    public void onPause() {
        AppEventsLogger.deactivateApp(this.getActivity());
    }

    public void onDestroy() {
        if(this.accessTokenTracker != null) {
            this.accessTokenTracker.stopTracking();
        }

    }

    public boolean onBackPressed() {
        return false;
    }

    public boolean onActivityResult(int requestCode, int resultCode, Intent data) {
        this.callbackManager.onActivityResult(requestCode, resultCode, data);
        return false;
    }

    private static native void onLogin(boolean isLoggedIn, String msg);

    private static native void onAPI(String tag, String jsonData);

    private static native void onFetchFriends(boolean ok, String msg);

    private static native void onSharedSuccess(String msg);

    private static native void onSharedFailed(String msg);

    private static native void onSharedCancel();

    private static native void onPermission(boolean ok, String msg);

    private static native void onRequestInvitableFriends(String friends);

    private static native void onInviteFriendsWithInviteIdsResult(boolean var0, String var1);

    private static native void onInviteFriendsResult(boolean ok, String msg);

    private static native void onUserInfo(String userInfo);
}
