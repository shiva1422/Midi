package com.kalasoft.Midi;

import android.media.midi.MidiDeviceInfo;

import androidx.annotation.Nullable;

public class KSMidiPort {

    private MidiDeviceInfo info;
    private int portIndex;
    private int type;
    private String string;

    @Override
    public boolean equals(@Nullable Object obj)
    {
        return super.equals(obj);
    }
}
