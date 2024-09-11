package com.kalasoft.digibandpiano;

import android.app.NativeActivity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.media.AudioManager;
import android.media.midi.MidiManager;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

import com.kalasoft.Midi.MidiEngine;

import java.io.IOException;
import java.io.InputStream;


public class MainActivity extends NativeActivity {

    static Bitmap bitmap;


    MidiEngine midiEngine;
 //TODO midi later

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        hideSystemUI();
        super.onCreate(savedInstanceState);
        //  setContentView(R.layout.activity_main);


        //TODO VERSION CHECK strickt
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.M)
        {
            //setup MIDi
            MidiManager midiManager = (MidiManager) getSystemService(MIDI_SERVICE);
            assert midiManager != null;//TODO notify and exit
            midiEngine = new MidiEngine(midiManager);
        }
        else
        {
            Log.e("Midi","not supported on device");
            assert false;//TODO notify user not supported
        }



    }
    float[] getDisplayParams()
    {
        float[] displayParams=new float[8];
        DisplayMetrics displayMetrics=new DisplayMetrics();
        //TODO
       // getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
        displayMetrics = getResources().getDisplayMetrics();
        displayParams[0]=displayMetrics.widthPixels;
        displayParams[1]=displayMetrics.heightPixels;
        displayParams[2]=displayMetrics.density;
        displayParams[3]=displayMetrics.densityDpi;
        displayParams[4]=displayMetrics.DENSITY_DEVICE_STABLE;
        displayParams[5]=displayMetrics.scaledDensity;
        displayParams[6]=displayMetrics.xdpi;
        displayParams[7]=displayMetrics.ydpi;
        return displayParams;

    }
    Bitmap getBitmap()
    {
        try {
            // InputStream stream = getContentResolver().openInputStream(imageUri);
            bitmap= BitmapFactory.decodeResource(getResources(),R.drawable.allkeys);
        }catch (Exception e)
        {e.printStackTrace();}


        return bitmap;
    }

    Bitmap loadImageAsset(String path) {
        InputStream is = null;
        Bitmap image = null;
        try {
            is = getAssets().open(path);
            image=BitmapFactory.decodeStream(is);
        }
        catch(IOException ioe) {

            ioe.printStackTrace();
            Log.e("LoadImage", "from assets io error");

        }
        finally {
            if( is != null)
            {
                try {
                    is.close();
                } catch (IOException e) {
                    Log.e("LoadImage", " error close the stream");
                }
            }
        }

        return image;
    }
    private void hideSystemUI() {
        // Enables regular immersive mode.
        // For "lean back" mode, remove SYSTEM_UI_FLAG_IMMERSIVE.
        // Or for "sticky immersive," replace it with SYSTEM_UI_FLAG_IMMERSIVE_STICKY

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                try {

                    View decorView = getWindow().getDecorView();
                    decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                            // Set the content to appear under the system bars so that the
                            // content doesn't resize when the system bars hide and show.
                            | View.SYSTEM_UI_FLAG_LAYOUT_STABLE | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                            | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                            // Hide the nav bar and status bar
                            | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                            | View.SYSTEM_UI_FLAG_FULLSCREEN);
                }catch (Exception e){e.printStackTrace();}

            }
        });

    }
    private void showSystemUI() {
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN);
    }

    private void changeVolume(boolean increase)
    {
        if(increase)
        {
            Log.e("Java ","increasevbolume");
            AudioManager audioManager=(AudioManager)getApplicationContext().getSystemService(getApplicationContext().AUDIO_SERVICE);
            audioManager.adjustVolume(AudioManager.ADJUST_RAISE,AudioManager.FLAG_SHOW_UI);
        }
        else
        {
            //decrease volume
            Log.e("Java ","decreasevbolume");
            AudioManager audioManager=(AudioManager)getApplicationContext().getSystemService(getApplicationContext().AUDIO_SERVICE);
            audioManager.adjustVolume(AudioManager.ADJUST_LOWER,AudioManager.FLAG_SHOW_UI);
        }

    }
}