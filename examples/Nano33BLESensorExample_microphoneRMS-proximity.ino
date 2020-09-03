/*
  Nano33BLESensorExample_microphoneRMS-proximity.ino
  Copyright (c) 2020 Dale Giancono. All rights reserved..

  This program is an example program showing some of the cababilities of the 
  Nano33BLESensor Library. In this case it outputs RMS microphone data and 
  proximity data from two of the Arduino Nano 33 BLE Sense's on board 
  sensors via serial in a format that can be displayed on the Arduino IDE 
  serial plotter. It also outputs the data via BLE in a string format that 
  can be viewed using a variety of BLE scanning software.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
/*****************************************************************************/
/*INCLUDES                                                                   */
/*****************************************************************************/
#include "Arduino.h"
/* For the bluetooth funcionality */
#include <ArduinoBLE.h>
#include "Nano33BLEProximity.h"
#include "Nano33BLEMicrophoneRMS.h"

/*****************************************************************************/
/*MACROS                                                                     */
/*****************************************************************************/
/* 
 * We use strings to transmit the data via BLE, and this defines the buffer
 * size used to transmit these strings. Only 20 bytes of data can be 
 * transmitted in one packet with BLE, so a size of 20 is chosen the the data 
 * can be displayed nicely in whatever application we are using to monitor the
 * data.
 */
#define BLE_BUFFER_SIZES 			20
/* Device name which can be scene in BLE scanning software. */
#define BLE_DEVICE_NAME				"Arduino Nano 33 BLE Sense"
/* Local name which should pop up when scanning for BLE devices. */
#define BLE_LOCAL_NAME				"MicrophoneRMS/Proximity BLE"

/*****************************************************************************/
/*GLOBAL Data                                                                */
/*****************************************************************************/
/* Pointer to Nano33BLEMicrophoneRMS that we will instanciate in the setup */
Nano33BLEMicrophoneRMS *Microphone;
/* Pointer to Nano33BLEProximity that we will instanciate in the setup */
Nano33BLEProximity *Proximity;
/* 
 * Nano33BLEMicrophoneRMSData and Nano33BLEProximityData object which we will 
 * store data in each time we read the microphone and proximity data. 
 */ 
Nano33BLEMicrophoneRMSData microphoneData;
Nano33BLEProximityData proximityData;

/* 
 * Declares the BLEService and characteristics we will need for the BLE 
 * transfer. The UUID was randomly generated using one of the many online 
 * tools that exist. It was chosen to use BLECharacteristic instead of 
 * BLEIntCharacteristic was it is hard to view int data in most BLE 
 * scanning software. Strings can be viewed easiler enough. In an actual 
 * application you might want to transfer ints directly.
 */
BLEService BLESensors("590d65c7-3a0a-4023-a05a-6aaf2f22441c");
BLECharacteristic microphoneRMSBLE("0007", BLERead | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES);
BLECharacteristic proximityBLE("0008", BLERead | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES);

/* Common global buffer will be used to write to the BLE characteristics. */
char bleBuffer[BLE_BUFFER_SIZES];
/*****************************************************************************/
/*SETUP (Initialisation)                                                     */
/*****************************************************************************/
void setup()
{
	/* 
	 * Serial setup. This will be used to transmit data for viewing on serial 
	 * plotter 
	 */
	Serial.begin(115200);
	while(!Serial);


	/* BLE Setup. For information, search for the many ArduinoBLE examples.*/
	if (!BLE.begin()) 
	{
		while (1);	
	}
	else
	{
		BLE.setDeviceName(BLE_DEVICE_NAME);
		BLE.setLocalName(BLE_LOCAL_NAME);
		BLE.setAdvertisedService(BLESensors);
        /* A seperate characteristic is used for each kind of data. */
		BLESensors.addCharacteristic(microphoneRMSBLE);
		BLESensors.addCharacteristic(proximityBLE);

		BLE.addService(BLESensors);
		BLE.advertise();
		/* 
		 * Initialises the microphone and proximity sensor, and starts the 
         * periodic reading of the sensor using a Mbed OS thread. 
         * The data is placed in a circular buffer and can be read whenever.
		 */
		Microphone = Nano33BLESensor<Nano33BLEMicrophoneRMS>::begin();
	    Proximity = Nano33BLESensor<Nano33BLEProximity>::begin();

		/* Plots the legend on Serial Plotter */
		Serial.println("MicrophoneRMS, Proximity");
	}
}

/*****************************************************************************/
/*LOOP (runtime super loop)                                                  */
/*****************************************************************************/
void loop()
{
	BLEDevice central = BLE.central();
	if(central)
	{
		int writeLength;
		bool dataGotFlag = false;
		/* 
		 * If a BLE device is connected, the data will start being read, 
		 * and the data will be written to each BLE characteristic. The same 
		 * data will also be output through serial so it can be plotted using 
		 * Serial Plotter. 
		 */
		while(central.connected())
		{	
			/* 
			 * sprintf is used to convert the read int value to a string 
			 * which is stored in bleBuffer. This string is then written to 
			 * the BLE characteristic. In regard to the dataGotFlag, each
             * time one of the two sensors is read we want to print the 
             * data to serial plotter. If only one sensor has new data, old
             * data from the other sensor will be printed. This is okay and
             * ensures that the output in Serial plotter will look okay.
			 */		
			if(Microphone->pop(microphoneData))
			{
				writeLength = sprintf(bleBuffer, "%d", microphoneData.RMSValue);
				microphoneRMSBLE.writeValue(bleBuffer, writeLength); 
				dataGotFlag = true;
			}

			if(Proximity->pop(proximityData))
			{
				writeLength = sprintf(bleBuffer, "%d", proximityData.proximity);
				proximityBLE.writeValue(bleBuffer, writeLength); 
				dataGotFlag = true;
			}

            if(dataGotFlag)
			{
			    Serial.printf("%d,%d\r\n", microphoneData.RMSValue, proximityData.proximity);
				dataGotFlag = false;
			}
		}
	}
}