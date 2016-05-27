package com.hub102.betterx;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.hardware.camera2.params.Face;
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

import java.lang.reflect.Method;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

/**
 * Created by nick on 4/12/16.
 */
public class FacebookX {
    private static Context mContext;
    private static ShareDialog shareDialog;
    private static AppInviteDialog inviteDialog;
    private static GameRequestDialog requestDialog;
    private static AccessTokenTracker accessTokenTracker = null;
    private boolean _userInfoRequested = false;

    protected static Method _reflectRunOnGLThread = null;

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

    public static void init(Context ctx) {
        mContext = ctx;
        FacebookX.configure();
    }

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

    public static void onStart() {
    }

    public static void onStop() {
    }

    public static void onResume() {
        AppEventsLogger.activateApp(mContext);
    }

    public static void onPause() {
        AppEventsLogger.deactivateApp(mContext);
    }

    public static void onDestroy() {
        if(accessTokenTracker != null) {
            accessTokenTracker.stopTracking();
        }

    }

    public static boolean onBackPressed() {
        return false;
    }

    public static boolean onActivityResult(int requestCode, int resultCode, Intent data) {
//        this.callbackManager.onActivityResult(requestCode, resultCode, data);
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
