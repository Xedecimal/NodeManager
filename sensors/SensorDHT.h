/*
* The MySensors Arduino library handles the wireless radio link and protocol
* between your home built sensors/actuators and HA controller of choice.
* The sensors forms a self healing radio network with optional repeaters. Each
* repeater and gateway builds a routing tables in EEPROM which keeps track of the
* network topology allowing messages to be routed to nodes.
*
* Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
* Copyright (C) 2013-2017 Sensnology AB
* Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
*
* Documentation: http://www.mysensors.org
* Support Forum: http://forum.mysensors.org
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* version 2 as published by the Free Software Foundation.
*/
#ifndef SensorDHT_h
#define SensorDHT_h

/*
SensorDHT
*/

#include <SimpleDHT.h>

class SensorDHT: public Sensor {
	
protected:
	SimpleDHT11* _dht;
	// int _dht_type;
	float _offset = 0;
	
public:
	SensorDHT(int8_t pin, uint8_t child_id = 0): Sensor(pin) {
		_name = "DHT";
		// _dht_type = DHT11;
		children.allocateBlocks(2);
		new Child(this,FLOAT,nodeManager.getAvailableChildId(child_id),S_TEMP,V_TEMP,_name);
		new Child(this,FLOAT,child_id > 0
			? nodeManager.getAvailableChildId(child_id+1)
			: nodeManager.getAvailableChildId(child_id),S_HUM,V_HUM,_name);
	};

	// define what to do during setup
	void onSetup() {
		// store the dht object
		_dht = new SimpleDHT11(_pin);
	};
	
	// define what to do during setup
	void onLoop(Child* child) {
		Serial.println("SensorDHT: Loop hit.");

		byte temperature = 0;
  		byte humidity = 0;

		Serial.println("SensorDHT: Calling dht->read.");
		int err = SimpleDHTErrSuccess;
		if ((err = _dht->read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
			Serial.print("Read DHT11 failed, err="); Serial.println(err); delay(1000);
			return;
		}

		nodeManager.sleepOrWait(_dht->getMinimumSamplingPeriod()+500);
		_dht->readSensor(true);
		// temperature sensor
		if (child->getType() == V_TEMP) {
			Serial.println("SensorDHT: Read V_TEMP");
			child->setValue(temperature);
		}

		// humidity sensor
		else if (child->getType() == V_HUM) {
			Serial.println("SensorDHT: Read V_HUM");
			child->setValue(humidity);
		}

		// Serial.println("SensorDHT: Sleeping...");
		// nodeManager.sleepOrWait(1000);
	};
};
#endif
