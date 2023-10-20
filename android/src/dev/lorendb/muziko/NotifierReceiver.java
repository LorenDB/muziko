package dev.lorendb.muziko;

import android.content.Context;
import android.content.Intent;
import android.content.BroadcastReceiver;

public class NotifierReceiver extends BroadcastReceiver
{
    @Override
    public void onReceive(Context context, Intent intent)
    {
        Intent startServiceIntent = new Intent(context, NotifierService.class);
        context.startService(startServiceIntent);
    }
}
