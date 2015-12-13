/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.example.android.bluetoothlegatt;

import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Environment;
import android.os.IBinder;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.EditText;
import android.widget.ExpandableListView;
import android.widget.GridView;
import android.widget.SimpleAdapter;
import android.widget.SimpleExpandableListAdapter;
import android.widget.TextView;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;

/**
 * For a given BLE device, this Activity provides the user interface to connect, display data,
 * and display GATT services and characteristics supported by the device.  The Activity
 * communicates with {@code BluetoothLeService}, which in turn interacts with the
 * Bluetooth LE API.
 */
public class DeviceControlActivity extends Activity {
    private final static String TAG = DeviceControlActivity.class.getSimpleName();

    public static final String EXTRAS_DEVICE_NAME = "DEVICE_NAME";
    public static final String EXTRAS_DEVICE_ADDRESS = "DEVICE_ADDRESS";

    GridView gridView;
    String[] labels = new String[] {
            "Time", "Servo 1", "Servo 2", "Servo 3", "Servo 4", //Servo names
            "Start", "", "", "", "",    //Start values
            "", "", "", "", "",         //Time1 values
            "", "", "", "", "",         //Time2 values
            "", "", "", "", "",         //Etc.
            "", "", "", "", "",
            "", "", "", "", "",
            "", "", "", "", "",
            "", "", "", "", "",
            "", "", "", "", "",
            "", "", "", "", "",
            "", "", "", "", "",
            "DT", "", "", "", ""};      //DT values
    String[] sublabels = new String[] {
            "[s]", "", "", "", "",
            "", "", "", "", "",         //Start values
            "", "", "", "", "",         //Time1 values
            "", "", "", "", "",         //Time2 values
            "", "", "", "", "",         //Etc.
            "", "", "", "", "",
            "", "", "", "", "",
            "", "", "", "", "",
            "", "", "", "", "",
            "", "", "", "", "",
            "", "", "", "", "",
            "", "", "", "", "",
            "", "", "", "", ""};        //DT values

    private EditText result;
    final Context context = this;
    final ArrayList<HashMap<String,String>> list = new ArrayList<>();
    int[] dataset = new int[52];
    int[] pos2ind = new int[] {  -1,-1,-1,-1,-1,    //Servo names
                                 -1,28,-1,-1,-1,    //Start values
                                  6,30,-1,-1,-1,    //Time1
                                  8,32,-1,-1,-1,    //Time2
                                 10,34,-1,-1,-1,
                                 12,36,-1,-1,-1,
                                 14,38,-1,-1,-1,
                                 16,40,-1,-1,-1,
                                 18,42,-1,-1,-1,
                                 20,44,-1,-1,-1,
                                 22,46,-1,-1,-1,
                                 24,48,-1,-1,-1,
                                 26,50,-1,-1,-1};    //TimeDT



    private TextView mConnectionState;
    private TextView mDataField;
    private String mDeviceName;
    private String mDeviceAddress;
    private ExpandableListView mGattServicesList;
    private BluetoothLeService mBluetoothLeService;
    private ArrayList<ArrayList<BluetoothGattCharacteristic>> mGattCharacteristics =
            new ArrayList<ArrayList<BluetoothGattCharacteristic>>();
    private boolean mConnected = false;
    private BluetoothGattCharacteristic mNotifyCharacteristic;

    private final String LIST_NAME = "NAME";
    private final String LIST_UUID = "UUID";

    // Code to manage Service lifecycle.
    private final ServiceConnection mServiceConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName componentName, IBinder service) {
            mBluetoothLeService = ((BluetoothLeService.LocalBinder) service).getService();
            if (!mBluetoothLeService.initialize()) {
                Log.e(TAG, "Unable to initialize Bluetooth");
                finish();
            }
            // Automatically connects to the device upon successful start-up initialization.
            mBluetoothLeService.connect(mDeviceAddress);
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            mBluetoothLeService = null;
        }
    };

    // Handles various events fired by the Service.
    // ACTION_GATT_CONNECTED: connected to a GATT server.
    // ACTION_GATT_DISCONNECTED: disconnected from a GATT server.
    // ACTION_GATT_SERVICES_DISCOVERED: discovered GATT services.
    // ACTION_DATA_AVAILABLE: received data from the device.  This can be a result of read
    //                        or notification operations.
    private final BroadcastReceiver mGattUpdateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            if (BluetoothLeService.ACTION_GATT_CONNECTED.equals(action)) {
                mConnected = true;
                updateConnectionState(R.string.connected);
                displayData(intent.getStringExtra(BluetoothLeService.EXTRA_DATA));
                invalidateOptionsMenu();
            } else if (BluetoothLeService.ACTION_GATT_DISCONNECTED.equals(action)) {
                mConnected = false;
                updateConnectionState(R.string.disconnected);
                invalidateOptionsMenu();
                clearUI();
            } else if (BluetoothLeService.ACTION_GATT_SERVICES_DISCOVERED.equals(action)) {
                // Show all the supported services and characteristics on the user interface.
//                displayGattServices(mBluetoothLeService.getSupportedGattServices());
            } else if (BluetoothLeService.ACTION_DATA_AVAILABLE.equals(action)) {
                displayData(intent.getStringExtra(BluetoothLeService.EXTRA_DATA));
            }
        }
    };

    // If a given GATT characteristic is selected, check for supported features.  This sample
    // demonstrates 'Read' and 'Notify' features.  See
    // http://d.android.com/reference/android/bluetooth/BluetoothGatt.html for the complete
    // list of supported characteristic features.
    private final ExpandableListView.OnChildClickListener servicesListClickListner =
            new ExpandableListView.OnChildClickListener() {
                @Override
                public boolean onChildClick(ExpandableListView parent, View v, int groupPosition,
                                            int childPosition, long id) {
                    if (mGattCharacteristics != null) {
                        final BluetoothGattCharacteristic characteristic =
                                mGattCharacteristics.get(groupPosition).get(childPosition);
                        final int charaProp = characteristic.getProperties();
                        if ((charaProp | BluetoothGattCharacteristic.PROPERTY_READ) > 0) {
                            // If there is an active notification on a characteristic, clear
                            // it first so it doesn't update the data field on the user interface.
                            if (mNotifyCharacteristic != null) {
                                mBluetoothLeService.setCharacteristicNotification(
                                        mNotifyCharacteristic, false);
                                mNotifyCharacteristic = null;
                            }
                            mBluetoothLeService.readCharacteristic(characteristic);
                        }
                        if ((charaProp | BluetoothGattCharacteristic.PROPERTY_NOTIFY) > 0) {
                            mNotifyCharacteristic = characteristic;
                            mBluetoothLeService.setCharacteristicNotification(
                                    characteristic, true);
                        }
                        return true;
                    }
                    return false;
                }
    };

    private void clearUI() {
//        mGattServicesList.setAdapter((SimpleExpandableListAdapter) null);
        mDataField.setText(R.string.no_data);
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.button_control);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        final Intent intent = getIntent();
        mDeviceName = intent.getStringExtra(EXTRAS_DEVICE_NAME);
        mDeviceAddress = intent.getStringExtra(EXTRAS_DEVICE_ADDRESS);



        mDataField = (TextView) findViewById(R.id.data_value);

        gridView = (GridView) findViewById(R.id.gridView1);

        SimpleAdapter adapter = new SimpleAdapter(this, list, R.layout.customgridview, new String[] {"Text","subText"}, new int[] {R.id.Text,R.id.subText});

        populateList();

        dataset = readLogFile();
        for (int i=0; i<dataset.length; i++) {
            Log.d(TAG,"dataset[i]"+dataset[i]);
        }
        updateList(dataset);

        gridView.setAdapter(adapter);

        gridView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            public void onItemClick(AdapterView<?> parent, View v,
                                    final int position, long id) {

                // get prompts.xml view
                if (pos2ind[position] != -1) {

                    LayoutInflater li = LayoutInflater.from(context);
                    View promptsView = li.inflate(R.layout.prompt, null);

                    AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(
                            context);

                    // set prompts.xml to alert dialog builder
                    alertDialogBuilder.setView(promptsView);

                    final EditText userInput = (EditText) promptsView
                            .findViewById(R.id.editTextDialogUserInput);

                    // set dialog message
                    alertDialogBuilder
                            .setCancelable(false)
                            .setPositiveButton("OK",
                                    new DialogInterface.OnClickListener() {
                                        public void onClick(DialogInterface dialog, int id) {
                                            // get user input and set it to result
                                            // edit text

                                            dataset[pos2ind[position]] = Integer.valueOf(userInput.getText().toString(), 10) % 256;
                                            dataset[pos2ind[position] + 1] = Integer.valueOf(userInput.getText().toString(), 10) / 256;
                                            Log.v(TAG, "dataset[pos2ind[position]]: " + dataset[pos2ind[position]]);
                                            updateList(dataset);

                                        }
                                    })
                            .setNegativeButton("Cancel",
                                    new DialogInterface.OnClickListener() {
                                        public void onClick(DialogInterface dialog, int id) {
                                            dialog.cancel();
                                        }
                                    });

                    // create alert dialog
                    AlertDialog alertDialog = alertDialogBuilder.create();

                    // show it
                    alertDialog.show();

                }
            }
        });

        getActionBar().setTitle(mDeviceName);
        getActionBar().setDisplayHomeAsUpEnabled(true);
        Intent gattServiceIntent = new Intent(this, BluetoothLeService.class);
        bindService(gattServiceIntent, mServiceConnection, BIND_AUTO_CREATE);
    }

    @Override
    protected void onResume() {
        super.onResume();
        registerReceiver(mGattUpdateReceiver, makeGattUpdateIntentFilter());
        if (mBluetoothLeService != null) {
            final boolean result = mBluetoothLeService.connect(mDeviceAddress);
            Log.d(TAG, "Connect request result=" + result);
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        unregisterReceiver(mGattUpdateReceiver);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unbindService(mServiceConnection);
        mBluetoothLeService = null;
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.gatt_services, menu);
        if (mConnected) {
            menu.findItem(R.id.menu_connect).setVisible(false);
            menu.findItem(R.id.menu_disconnect).setVisible(true);
        } else {
            menu.findItem(R.id.menu_connect).setVisible(true);
            menu.findItem(R.id.menu_disconnect).setVisible(false);
        }
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch(item.getItemId()) {
            case R.id.menu_connect:
                mBluetoothLeService.connect(mDeviceAddress);
                return true;
            case R.id.menu_disconnect:
                mBluetoothLeService.disconnect();
                return true;
            case android.R.id.home:
                onBackPressed();
                return true;
        }
        return super.onOptionsItemSelected(item);
    }

    private void updateConnectionState(final int resourceId) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
//                mConnectionState.setText(resourceId);
            }
        });
    }

    private void displayData(String data) {
        if (data != null) {
            mDataField.setText(data);
            Log.d(TAG, "data: " + data);

            //String[] items = data.replaceAll("[^A-Z0-9\\s]", "").split(" ");
            String[] items = data.split(" ");
            for (int i = 0; i < items.length; i++) {
                    Log.d(TAG, "items[i]: "+items[i]);
            }

            for (int i = 0; i < items.length; i++) {
                try {
                    dataset[i] = Integer.valueOf(items[i], 16);
                    Log.d(TAG, "dataset[i]: "+dataset[i]);
                } catch (NumberFormatException nfe) {}
            }
            updateList(dataset);


        }
    }

    private static IntentFilter makeGattUpdateIntentFilter() {
        final IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_CONNECTED);
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_DISCONNECTED);
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_SERVICES_DISCOVERED);
        intentFilter.addAction(BluetoothLeService.ACTION_DATA_AVAILABLE);
        return intentFilter;
    }

    public void onClickWrite(View v){
        if(mBluetoothLeService != null) {
            // First write data set to file for backup
            //String externalStorage = Environment.getExternalStorageDirectory().getAbsolutePath();
            //File mydir = context.getDir("mydir", Context.MODE_PRIVATE); //Creating an internal dir;
            //File mydir = context.getDir("mydir", Context.MODE_WORLD_READABLE);
            //File folder = new File( Environment.getDataDirectory().getAbsolutePath() + "/storage/");
            //folder.mkdir();
            //File fileWithinMyDir = new File(mydir, "myfile"); //Getting a file within the dir.
            //String filename = "dataSet.txt";
            //File outputFile = new File(File.separator + myDirectory + File.separator + fileName);

            //FileOutputStream outputStream;
            createLogFile(dataset);
            /*try {
                //outputStream = openFileOutput(filename, Context.MODE_APPEND);
                FileOutputStream out = new FileOutputStream(fileWithinMyDir); //Use the stream as usual to write into the file.
                for (int i=0; i<dataset.length; i++) {
                    out.write(dataset[i]);
                }
                out.close();
            } catch (Exception e) {
                e.printStackTrace();
            }*/

            // Convert int array to signed byte array
            // byte[] dataSetBytes = new byte[dataset.length];
            byte[] dataSetBytes = new byte[52];
            for (int i = 0; i < 52; i++)
            {
                dataSetBytes[i] = (byte)(dataset[i] & 0xFF);
            }




            // Write byte array to characteristic
            mBluetoothLeService.writeCustomCharacteristic(dataSetBytes);
        }
    }

    public void onClickRead(View v){
        if(mBluetoothLeService != null) {
            mBluetoothLeService.readCustomCharacteristic();
            //displayData(intent.getStringExtra(BluetoothLeService.EXTRA_DATA));
        }

    }

    private void populateList() {
        for (int i = 0; i < labels.length; i++) {
            HashMap<String, String> temp = new HashMap<>();
            temp.put("Text", labels[i]);
            temp.put("subText", sublabels[i]);
            list.add(temp);
        }
    }

    private void updateList(int[] data) {
        // Update times
        int tempval;
        int tempcum = 0;
        for (int i = 0; i < 11; i++) {
            int pos = 10 + i*5;
            HashMap<String, String> temp = new HashMap<>();
            tempval = data[pos2ind[pos]]|(data[pos2ind[pos]+1]<<8);
            tempcum = tempcum + tempval;
            temp.put("Text", String.valueOf(tempval/100.0f));
            temp.put("subText", String.valueOf(tempcum/100.0f));
            if (i==10) {
                temp.put("Text", "DT");
                temp.put("subText", String.valueOf(tempval/100.0f));
            }
            list.set(pos, temp);
        }

        // Update servo positions
        for (int i = 0; i < 12; i++) {
            int pos = 6 + i*5;
            HashMap<String, String> temp = new HashMap<>();
            tempval = data[pos2ind[pos]]|(data[pos2ind[pos]+1]<<8);
            temp.put("Text", String.valueOf(tempval));
            temp.put("subText", "");
            list.set(pos, temp);
        }
        gridView.invalidateViews();
    }

    private void createLogFile(int[] data) {
            /** Directory */
            String root = Environment.getExternalStorageDirectory().toString();
            File directory = new File(root + "/EZtimer");
            if (!directory.exists()) {
                Log.d(TAG, "Making directory");
                directory.mkdirs();
            }

            /** File  name */
            File file = new File(directory + "/dataset.txt");
            try {
                FileOutputStream out = new FileOutputStream(file); //Use the stream as usual to write into the file.
                for (int i=0; i<data.length; i++) {
                    String string = Integer.toString(data[i]);
                    out.write(string.getBytes());
                    string = "\n";
                    out.write(string.getBytes());
                }
                out.close();
            }
            catch (Exception e) {
                e.printStackTrace();
            }
    }

    public int[] readLogFile() {
        ArrayList<String> list = new ArrayList<String>();
        int[] data = new int[52];
        String root = Environment.getExternalStorageDirectory().toString();
        File directory = new File(root + "/EZtimer");
        File file = new File(directory + "/dataset.txt");
        try {
            FileInputStream in = new FileInputStream(file);
            // in = openFileInput(file);
            if (file.exists ()) {
                BufferedReader br = new BufferedReader(new InputStreamReader(in));
                String line;
                for (int i=0; i<data.length; i++) {
                    //line = br.readLine();
                    data[i] = Integer.parseInt(br.readLine());
                    Log.d(TAG, "data[i]"+data[i]);
                }
            }
            in.close();
        }
        catch (Exception e) {
        }
        return data;
    }
}
