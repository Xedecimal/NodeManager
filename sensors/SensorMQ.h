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
#ifndef SensorMQ_h
#define SensorMQ_h

/*
	SensorMQ
*/

#include <MQ2.h>

class SensorMQ: public Sensor {
protected:
	MQ2* _mq2;
public:
	SensorMQ(int8_t pin, uint8_t child_id = 0): Sensor(pin) {
		_name = "MQ";
		children.allocateBlocks(3);
		new Child(this,FLOAT,nodeManager.getAvailableChildId(child_id),S_AIR_QUALITY,V_LEVEL,_name);
		new Child(this,FLOAT,nodeManager.getAvailableChildId(child_id+1),S_AIR_QUALITY,V_LEVEL,_name);
		new Child(this,FLOAT,nodeManager.getAvailableChildId(child_id+2),S_AIR_QUALITY,V_LEVEL,_name);

		children.get(1)->setDescription(_name);
		children.get(2)->setDescription(_name);
		children.get(3)->setDescription(_name);
	};

	// define what to do during setup
	void onSetup() {
		_mq2 = new MQ2(_pin);
		_mq2->begin();
	};

	// define what to do during loop
	void onLoop(Child* child) {
		if (child->getChildId() == 0) {
			child->setValue((double)_mq2->readCO());
		}

		if (child->getChildId() == 1) {
			child->setValue((double)_mq2->readLPG());
		}

		if (child->getChildId() == 2) {
			child->setValue((double)_mq2->readSmoke());
		}
	};

protected:

};
#endif
