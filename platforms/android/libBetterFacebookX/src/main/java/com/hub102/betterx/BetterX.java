package com.hub102.betterx;

import android.app.Activity;
import android.content.Context;
import android.os.Handler;
import android.util.Log;

import com.sdkbox.plugin.PluginListener;

import java.lang.reflect.Method;
import java.util.concurrent.CopyOnWriteArraySet;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Created by nick on 4/12/16.
 */
public class BetterX {
    protected static final String TAG = "BetterX";
    private static ExecutorService _pool = Executors.newFixedThreadPool(8);
    protected static Context _sContext = null;
    protected static Handler _sMainThreadHandler = null;
    protected static CopyOnWriteArraySet<PluginListener> _sListeners = new CopyOnWriteArraySet();
    protected static Method _reflectRunOnGLThread = null;
    protected static String _applicationToken = null;
    protected static String _cdid = null;

    public static void init(Context context) {
        _sContext = context;
        if(null == _sMainThreadHandler) {
            _sMainThreadHandler = new Handler();
        }

        try {
            Class x = _sContext.getClass();
            _reflectRunOnGLThread = x.getMethod("runOnGLThread", new Class[]{Runnable.class});
        } catch (Exception var2) {
            Log.i(TAG, "Context class has no method: \'runOnGLThread\'. All methods will run in main thread.");
        }

        nativeInit();
    }

    private static native void nativeInit();

    public static void runOnGLThread(Runnable r) {
        if(_reflectRunOnGLThread != null) {
            try {
                _reflectRunOnGLThread.invoke(_sContext, new Object[]{r});
            } catch (Exception var2) {
                runOnMainThread(r);
            }
        } else {
            runOnMainThread(r);
        }
    }

    public static void runOnMainThread(Runnable r) {
        if(null != _sContext) {
            Activity act = (Activity)_sContext;
            act.runOnUiThread(r);
        } else {
            Log.i(TAG, "Runnable executed in current thread.");
            r.run();
        }

    }
}
