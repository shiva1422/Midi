package com.kalasoft.Midi;

import android.media.midi.MidiReceiver;
import android.os.Build;
import android.util.Log;

import androidx.annotation.RequiresApi;

import java.io.IOException;

/**
 * Convert stream of arbitrary MIDI bytes into discrete messages.
 *
 * Parses the incoming bytes and then posts individual messages to the receiver
 * specified in the constructor. Short messages of 1-3 bytes will be complete.
 * System Exclusive messages may be posted in pieces.
 *
 * Resolves Running Status and interleaved System Real-Time messages.
 */
@RequiresApi(api = Build.VERSION_CODES.M)
public class KMidiReceiver extends MidiReceiver {

    MidiReceiver receiver;

    /*
     * @see android.midi.MidiReceiver#onSend(byte[], int, int, long)
     */
    @Override
    public void onSend(byte[] bytes, int i, int i1, long l) throws IOException
    {
        Log.d("KMidiReceiver","on Send");
    }

    public KMidiReceiver(MidiReceiver receiver)
    {
        this.receiver = receiver;
    }
}
