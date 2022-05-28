package com.reactnativeraw;

import android.content.SharedPreferences;
import android.os.Build;
import android.os.Environment;
import android.preference.PreferenceManager;
import android.util.Log;
import android.content.Context;

import androidx.annotation.NonNull;

import com.facebook.react.bridge.Callback;
import com.facebook.react.bridge.JavaScriptContextHolder;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.WritableNativeMap;
import com.facebook.react.module.annotations.ReactModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.WritableMap;
import java.util.Map;
import java.util.HashMap;

@ReactModule(name = SimpleJsiModule.NAME)
public class SimpleJsiModule extends ReactContextBaseJavaModule {
  public static final String NAME = "raw";
  private native void nativeInstall(long jsiPtr, String docDir);

  public SimpleJsiModule(ReactApplicationContext reactContext) {
    super(reactContext);
  }

  @Override
  @NonNull
  public String getName() {
    return NAME;
  }

  @ReactMethod(isBlockingSynchronousMethod = true)
  public boolean install() {
    try {
      System.loadLibrary("cpp");

      ReactApplicationContext context = getReactApplicationContext();
      nativeInstall(
        context.getJavaScriptContextHolder().get(),
        context.getFilesDir().getAbsolutePath()
      );
      return true;
    } catch (Exception exception) {
      return false;
    }
  }

  @ReactMethod(isBlockingSynchronousMethod = true)
  public WritableMap  Dir() {
    Map<String, Object> res = new HashMap<>();
    ReactApplicationContext context = getReactApplicationContext();
    res.put("DocumentDir", context.getFilesDir().getAbsolutePath());
    res.put("CacheDir", context.getCacheDir().getAbsolutePath());
    res.put("DCIMDir", context.getExternalFilesDir(Environment.DIRECTORY_DCIM).getAbsolutePath());
    res.put("PictureDir", context.getExternalFilesDir(Environment.DIRECTORY_PICTURES).getAbsolutePath());
    res.put("MusicDir", context.getExternalFilesDir(Environment.DIRECTORY_MUSIC).getAbsolutePath());
    res.put("DownloadDir", context.getExternalFilesDir(Environment.DIRECTORY_DOWNLOADS).getAbsolutePath());
    res.put("MovieDir", context.getExternalFilesDir(Environment.DIRECTORY_MOVIES).getAbsolutePath());
    res.put("RingtoneDir", context.getExternalFilesDir(Environment.DIRECTORY_RINGTONES).getAbsolutePath());
    WritableMap map = new WritableNativeMap();

    for (Map.Entry<String, Object> entry : res.entrySet()) {
      map.putString(entry.getKey(), (String) entry.getValue());
    }
    return(map);

  }

}
