package com.kalasoft.Midi;

import android.media.midi.MidiDeviceInfo;
import android.media.midi.MidiDeviceStatus;
import android.media.midi.MidiManager;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;

import androidx.annotation.RequiresApi;

import java.util.HashSet;

@RequiresApi(api = Build.VERSION_CODES.M)
public class MidiEngine extends MidiManager.DeviceCallback {


    //TODO Comparator
   private HashSet<KSMidiDeviceInfo> connectedDevices = new HashSet<KSMidiDeviceInfo>();


   private MidiManager midiManager;

   public MidiEngine(MidiManager midiManager)
   {
       this.midiManager = midiManager;
       midiManager.registerDeviceCallback(this, new Handler(Looper.getMainLooper()));
   }


    //Midi Device Listeners
    @Override
    public void onDeviceAdded(MidiDeviceInfo device)
    {
        super.onDeviceAdded(device);
        Log.d("MidiEngine","device added" + device.toString());
    }

    @Override
    public void onDeviceRemoved(MidiDeviceInfo device)
    {
        super.onDeviceRemoved(device);
        Log.d("MidiEngine","device removed " + device.toString());
        int inputPortCount  = device.getInputPortCount();
        int outputPortCount = device.getOutputPortCount();
    }

    @Override
    public void onDeviceStatusChanged(MidiDeviceStatus status)
    {
        super.onDeviceStatusChanged(status);
        Log.d("MidiEngine","devices status changed  status -" + status.toString() + " device -" + status.getDeviceInfo().toString() );

    }

}
