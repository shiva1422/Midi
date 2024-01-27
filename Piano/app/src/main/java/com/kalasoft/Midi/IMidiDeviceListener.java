package com.kalasoft.Midi;

public interface IMidiDeviceListener {
    public  void onDeviceAdded(KSMidiDeviceInfo midiDeviceInfo);
    public  void onDeviceRemoved(KSMidiDeviceInfo midiDeviceInfo);
}
