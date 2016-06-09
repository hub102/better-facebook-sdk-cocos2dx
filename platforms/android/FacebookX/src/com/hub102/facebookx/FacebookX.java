package com.hub102.facebookx;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.hardware.camera2.params.Face;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.os.Handler;

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
import com.facebook.Profile;
import com.facebook.appevents.AppEventsLogger;
import com.facebook.login.LoginManager;
import com.facebook.login.LoginResult;
import com.facebook.share.ShareApi;
import com.facebook.share.Sharer;
import com.facebook.share.model.AppInviteContent;
import com.facebook.share.model.GameRequestContent;
import com.facebook.share.model.ShareContent;
import com.facebook.share.model.ShareLinkContent;
import com.facebook.share.model.ShareOpenGraphObject;
import com.facebook.share.model.ShareOpenGraphAction;
import com.facebook.share.model.ShareOpenGraphContent;
import com.facebook.share.model.SharePhoto;
import com.facebook.share.model.SharePhotoContent;
import com.facebook.share.widget.AppInviteDialog;
import com.facebook.share.widget.GameRequestDialog;
import com.facebook.share.widget.ShareDialog;

import org.json.JSONObject;

import java.lang.reflect.Method;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import java.util.List;

/**
 * Created by nick on 4/12/16.
 */
public class FacebookX {
    private static Context mContext;
    protected static Handler mMainThreadHandler = null;
    private static CallbackManager mCallbackManager;
    private static ShareDialog shareDialog;
    private static AppInviteDialog inviteDialog;
    private static GameRequestDialog requestDialog;
    private static AccessTokenTracker mAccessTokenTracker = null;
    private boolean _userInfoRequested = false;
    private static boolean mLoggedIn = false;

    protected static Method _reflectRunOnGLThread = null;

    public static final String PLUGIN_PRIVATE_DATA_KEY = "com.sdkbox.fb";
    public static final String LOGIN_ACCESS_TOKEN_SET = "access_token";
    public static final String LOGIN_REQUESTED_PERMISSIONS = "login_permissions";
    public static final String LOGIN_DEFAULT_REQUESTED_PERMISSIONS = "public_profile";

    private static boolean mDebug = true;
    static final String TAG = "FacebookX";

    static final int FBSDKShareTypeAPI = 1;
    static final int FBSDKShareTypeDialog = 2;
    static final String FB_NONE = "0";
    static final String FB_LINK = "1";
    static final String FB_PHOTO = "2";
    static final int kFBLoginCallback = 1;
    static final int kFBPermissionCallback = 2;

    public static void init(Context ctx) {
        mContext = ctx;
        
        if (null == mMainThreadHandler) {
            mMainThreadHandler = new Handler();
        }

        try {
            Class ownerClass = mContext.getClass();
            _reflectRunOnGLThread = ownerClass.getMethod("runOnGLThread", Runnable.class);
        } catch (Exception e) {}

        FacebookX.configure();
    }

    public static CallbackManager getCallbackManager() {
        return mCallbackManager;
    }
    public static AccessTokenTracker getAccessTokenTracker() { return mAccessTokenTracker; };

    private static Activity getActivity() {
        return (Activity)mContext;
    }

    public static void setLoggedIn(boolean loggedIn) { mLoggedIn = loggedIn; }

    public static boolean isLoggedIn() { return mLoggedIn; }

    public static void runOnGLThread(Runnable r) {
        if (_reflectRunOnGLThread != null) {
            try {
                _reflectRunOnGLThread.invoke(mContext, r);
            } catch (Exception var2) {
                runOnMainThread(r);
            }
        } else {
            runOnMainThread(r);
        }
    }

    public static void runOnMainThread(Runnable r) {
        if (null != mContext) {
            Activity act = (Activity) mContext;
            act.runOnUiThread(r);
        } else {
            Log.i(TAG, "Runnable executed in current thread.");
            r.run();
        }
    }

    public static void configure() {
        FacebookSdk.sdkInitialize(mContext);
        mCallbackManager = CallbackManager.Factory.create();

        try {
            FacebookX.runOnMainThread((Runnable)new Runnable(){

                public void run() {
                    FacebookX.shareDialog = new ShareDialog(FacebookX.getActivity());
                    FacebookX.shareDialog.registerCallback(FacebookX.getCallbackManager(), (FacebookCallback)new FacebookCallback<Sharer.Result>(){

                        public void onSuccess(Sharer.Result result) {
                            String ret = "{\"postId\":\"" + result.getPostId() + "\"}";
                            FacebookX.onSharedSuccessWrapper(ret);
                        }

                        public void onCancel() {
                            FacebookX.onSharedCancelWrapper();
                        }

                        public void onError(FacebookException exception) {
                            FacebookX.onSharedFailedWrapper(exception.toString());
                        }
                    });
                    FacebookX.requestDialog = new GameRequestDialog(FacebookX.getActivity());
                    FacebookX.requestDialog.registerCallback(FacebookX.getCallbackManager(),
                            new FacebookCallback<GameRequestDialog.Result>() {
                                @Override
                                public void onSuccess(GameRequestDialog.Result result) {
                                    Log.v("game request success", result.toString());
                                    onInviteFriendsWithInviteIdsResult(true, result.toString());
                                }
                                @Override
                                public void onCancel() {
                                    onInviteFriendsWithInviteIdsResult(false, "Request cancelled");
                                }
                                @Override
                                public void onError(FacebookException e) {
                                    onInviteFriendsWithInviteIdsResult(false, e.toString());
                                }
                            }
                    );
                }

            });
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void onRequestInvitableFriendsWrapper(final String invitableFriendsData) {
        FacebookX.runOnGLThread(new Runnable() {
            public void run() {
                FacebookX.onRequestInvitableFriends(invitableFriendsData);
            }
        });
    }

    private static void onLoginWrapper(final boolean hasLogin, final String data) {
        FacebookX.runOnGLThread(new Runnable() {
            public void run() {
                FacebookX.onLogin(hasLogin, data);
            }
        });
    }

    private static void onAPIWrapper(final String apiPath, final String data) {
        FacebookX.runOnGLThread(new Runnable() {
            public void run() {
                FacebookX.onAPI(apiPath, data);
            }
        });
    }

    private static void onFetchFriendsWrapper(final boolean ok, final String data) {
        FacebookX.runOnGLThread(new Runnable() {
            public void run() {
                FacebookX.onFetchFriends(ok, data);
            }
        });
    }

    private static void onSharedSuccessWrapper(final String data) {
        FacebookX.runOnGLThread(new Runnable() {
            public void run() {
                FacebookX.onSharedSuccess(data);
            }
        });
    }

    private static void onSharedFailedWrapper(final String data) {
        FacebookX.runOnGLThread(new Runnable() {
            public void run() {
                FacebookX.onSharedFailed(data);
            }
        });
    }

    private static void onSharedCancelWrapper() {
        FacebookX.runOnGLThread(new Runnable() {
            public void run() {
                FacebookX.onSharedCancel();
            }
        });
    }

    private static void onPermissionWrapper(final boolean hasLogin, final String data) {
        FacebookX.runOnGLThread(new Runnable() {
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
//
//    public static void onStart() {
//    }
//
//    public static void onStop() {
//    }
//
//    public static void onResume() {
//        AppEventsLogger.activateApp(mContext);
//    }
//
//    public static void onPause() {
//        AppEventsLogger.deactivateApp(mContext);
//    }
//
//    public static void onDestroy() {
//        if(accessTokenTracker != null) {
//            accessTokenTracker.stopTracking();
//        }
//    }
//
//    public static boolean onBackPressed() {
//        return false;
//    }

    public static void login() {
        FacebookX.login(new String("public_profile;email;user_friends"));
    }

    public static void login(String permissions) {
        if (FacebookX.isLoggedIn()) {
            onLoginWrapper(true, "Player is already logged in");
        } else {
            String[] permissionsArray = permissions.split(";");
            List<String> permissionNeeds = Arrays.asList(permissionsArray);
            LoginManager.getInstance().logInWithReadPermissions((Activity)mContext, permissionNeeds);
            LoginManager.getInstance().registerCallback(mCallbackManager,
                new FacebookCallback<LoginResult>() {
                    @Override
                    public void onSuccess(LoginResult loginResults) {
                        FacebookX.onLoginWrapper(true, loginResults.toString());
                    }
                    @Override
                    public void onCancel() {
                        FacebookX.onLoginWrapper(false, null);
                    }


                    @Override
                    public void onError(FacebookException e) {
                        FacebookX.onLoginWrapper(false, e.toString());
                    }
                }
            );
        }
    }

    public static void logout() {
        LoginManager.getInstance().logOut();
    }

    public static String getAccessToken() {
        if (AccessToken.getCurrentAccessToken() != null) {
            return AccessToken.getCurrentAccessToken().getToken();
        } else {
            return null;
        }
    }

    public static String getUserID() {
        return Profile.getCurrentProfile().getId();
    }

    public static String getName() {
        return Profile.getCurrentProfile().getName();
    }

    public static Arrays getPermissionList() {
        return null;
    }

    public static void share(final String info) {
//        Map<String, String> mappedInfo = new HashMap<String, String>();
//        String[] pairs = info.split(";");
//        for (String pair: pairs) {
//            String[] keyAndValue = pair.split("Hub102MarkRulesTheWorld");
//            if (keyAndValue.length == 2) {
//                mappedInfo.put(keyAndValue[0], keyAndValue[1]);
//            } else {
//                mappedInfo.put(keyAndValue[0], "");
//            }
//        }
        FacebookX.share(FacebookX.convertStringToPairs(info, ";", "Hub102MarkRulesTheWorld"));
    }

    public static void share(final Map<String, String> info) {
        Log.v("Java share:", info.toString());
        try {
            final String shareType = FacebookX.getOrDefault(info, "type", FB_NONE);
            final int type = 2;
            if (isConnectionAvailable() == false) {
                FacebookX.onSharedFailedWrapper("Connection not available");
                return;
            }
            if (shareType == FB_NONE) {
                FacebookX.onSharedFailedWrapper("Share type not available");
                return;
            }
            FacebookX.runOnMainThread((Runnable)new Runnable() {
                public void run() {
                    if (shareType.equals(FB_LINK)) {
                        String value = null;
                        ShareLinkContent.Builder contentBuilder = new ShareLinkContent.Builder();
                        value = (String)FacebookX.getOrDefault(info, "link", "");
                        if (value.length() > 0) {
                            contentBuilder = (ShareLinkContent.Builder)contentBuilder.setContentUrl(
                                Uri.parse((String)value));
                        }
                        if ((value = (String)FacebookX.getOrDefault(info, "title", "")).length() > 0) {
                            contentBuilder = contentBuilder.setContentTitle(value);
                        }
                        if ((value = (String)FacebookX.getOrDefault(info, "text", "")).length() > 0) {
                            contentBuilder = contentBuilder.setContentDescription(value);
                        }
                        if ((value = (String)FacebookX.getOrDefault(info, "image", "")).length() > 0) {
                            contentBuilder = contentBuilder.setImageUrl(Uri.parse((String)value));
                        }
                        ShareLinkContent content = contentBuilder.build();
                        if (type == 1) {
                            ShareApi.share((ShareContent)content, (FacebookCallback)new FacebookCallback<Sharer.Result>(){
                                public void onSuccess(Sharer.Result result) {
                                    String ret = "{\"postId\":\"" + result.getPostId() + "\"}";
                                    FacebookX.onSharedSuccessWrapper(ret);
                                }

                                public void onCancel() {
                                    FacebookX.onSharedCancelWrapper();
                                }

                                public void onError(FacebookException e) {
                                    FacebookX.onSharedFailedWrapper(e.toString());
                                }
                            });
                        }
                        else if (type == 2) {
                            FacebookX.shareDialog.show(content);
                        }
                    } else if (shareType.equals(FB_PHOTO)) {
                        Bitmap image = BitmapFactory.decodeFile((String)FacebookX.getOrDefault(info, "image", ""));
                        SharePhoto photo = new SharePhoto.Builder().setBitmap(image).build();
                        SharePhotoContent content = new SharePhotoContent.Builder().addPhoto(photo).build();
                        if (type == 1) {
                            ShareApi.share((ShareContent)content, (FacebookCallback)new FacebookCallback<Sharer.Result>() {
                                public void onSuccess(Sharer.Result result) {
                                    String ret = "{\"postId\":\"" + result.getPostId() + "\"}";
                                    FacebookX.onSharedSuccessWrapper(ret);
                                }

                                public void onCancel() {
                                    FacebookX.onSharedCancelWrapper();
                                }

                                public void onError(FacebookException e) {
                                    FacebookX.onSharedFailedWrapper(e.toString());
                                }
                            });
                        } else if (type == 2) {
                            FacebookX.shareDialog.show(content);
                        }
                    }
                } 
            });
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void shareOpenGraphStory(final String properties, final String actionType, final String previewPropertyName) {
        Log.v("Java shareOG Wrapper", properties);
        shareOpenGraphStory(FacebookX.convertStringToPairs(properties, ";", "Hub102MarkRulesTheWorld"), actionType, previewPropertyName);
    }

    public static void shareOpenGraphStory(final Map<String, String> properties, final String actionType, final String previewPropertyName) {
        Log.v("Java shareOG", properties.toString());
        Log.v("Java shareOG", actionType);
        Log.v("Java shareOG", previewPropertyName);
        String type = null;
        String title = null;
        String description = null;
        String image = null;
        String url = null;
        type = (String)FacebookX.getOrDefault(properties, "type", "");
        title = (String)FacebookX.getOrDefault(properties, "title", "");
        description = (String)FacebookX.getOrDefault(properties, "description", "");
        image = (String)FacebookX.getOrDefault(properties, "image", "");
        url = (String)FacebookX.getOrDefault(properties, "url", "");

        if (isConnectionAvailable() == false) {
            FacebookX.onSharedFailedWrapper("Connection not available");
            return;
        }

        ShareOpenGraphObject object = new ShareOpenGraphObject.Builder()
                .putString("og:type", type)
                .putString("og:title", title)
                .putString("og:description", description)
                .putString("og:image", image)
                .putString("og:url", url)
                .build();

        ShareOpenGraphAction action = new ShareOpenGraphAction.Builder()
                .setActionType(actionType)
                .putObject(previewPropertyName, object)
                .build();

        ShareOpenGraphContent content = new ShareOpenGraphContent.Builder()
                .setPreviewPropertyName(previewPropertyName)
                .setAction(action)
                .build();

        FacebookX.shareDialog.show(content);
    }

    public static void requestInvitableFriends(final String params) {
        requestInvitableFriends(FacebookX.convertStringToPairs(params, ";", "Hub102MarkRulesTheWorld"));
    }

    public static void requestInvitableFriends(final Map<String, String> params) {
        Bundle args = new Bundle();
        for (Map.Entry<String, String>entry:params.entrySet()) {
            args.putString(entry.getKey(), entry.getValue());
        }
        GraphRequest request = new GraphRequest(AccessToken.getCurrentAccessToken(), "me/invitable_friends", args, HttpMethod.GET, new GraphRequest.Callback() {
            @Override
            public void onCompleted(GraphResponse graphResponse) {
                if (graphResponse.getJSONObject() != null) {
                    FacebookX.onRequestInvitableFriendsWrapper(graphResponse.getJSONObject().toString());
                } else {
                    FacebookX.onRequestInvitableFriendsWrapper(null);
                }
            }
        });
        request.executeAsync();
    }

    public static void inviteFriendsWithInviteIds(final String ids, String title, String text) {
        FacebookX.inviteFriendsWithInviteIds(ids.split(";"), title, text);
    }

    public static void inviteFriendsWithInviteIds(final String[] invite_ids, String title, String text) {
        if (invite_ids == null || invite_ids.length == 0) {
            return;
        }
        Log.v("Java reqFriends", invite_ids.toString());
//        StringBuilder sb = new StringBuilder();
//        for (int i = 0; i < invite_ids.length; ++i) {
//            sb.append(invite_ids[i]);
//            if (i >= invite_ids.length - 1) continue;
//            sb.append(",");
//        }
//        GameRequestContent content = new GameRequestContent.Builder().setMessage(text).setTitle(title).setTo(sb.toString()).build();
//        FacebookX.requestDialog.show(content);
        List<String> recipients = Arrays.asList(invite_ids);
        GameRequestContent content = new GameRequestContent.Builder()
                .setTitle(title)
                .setMessage(text)
                .setRecipients(recipients)
                .build();
        FacebookX.requestDialog.show(content);
    }

    private static Map<String, String> convertStringToPairs(String string, String pairSplRegexp, String keyValSplRegexp) {
        Map<String, String> ret = new HashMap<String, String>();
        String[] pairs = string.split(pairSplRegexp);
        for (String pair: pairs) {
            String[] keyAndValue = pair.split(keyValSplRegexp);
            if (keyAndValue.length == 2) {
                ret.put(keyAndValue[0], keyAndValue[1]);
            } else {
                ret.put(keyAndValue[0], "");
            }
        }
        return ret;
    }

    private static boolean isConnectionAvailable() {
        ConnectivityManager cm =
                (ConnectivityManager)mContext.getSystemService(Context.CONNECTIVITY_SERVICE);

        NetworkInfo activeNetwork = cm.getActiveNetworkInfo();
        boolean isConnected = activeNetwork != null &&
                activeNetwork.isConnectedOrConnecting();
        return isConnected;
    }

    private static native void onLogin(boolean isLoggedIn, String msg);

    private static native void onAPI(String tag, String jsonData);

    private static native void onFetchFriends(boolean ok, String msg);

    private static native void onSharedSuccess(String msg);

    private static native void onSharedFailed(String msg);

    private static native void onSharedCancel();

    private static native void onPermission(boolean ok, String msg);

    private static native void onRequestInvitableFriends(String friends);

    private static native void onInviteFriendsWithInviteIdsResult(boolean ok, String msg);

    private static native void onInviteFriendsResult(boolean ok, String msg);

    private static native void onUserInfo(String userInfo);
}