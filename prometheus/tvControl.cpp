#include "tvControl.h"

tvControl::tvControl() {
	// Setup lirc
	//fd = lirc_get_local_socket(NULL, 0);
	//lirc_send_one(fd, "RCA", "KEY_POWER");

	// Setup CEC if available.
	cec = true;
	//bcm_host_init();
	cec_config.Clear();
	cec_callbacks.Clear();

	const std::string devicename("TV");
	devicename.copy(cec_config.strDeviceName, std::min(devicename.size(),13u) );

	cec_config.clientVersion       = CEC::LIBCEC_VERSION_CURRENT;
	cec_config.bActivateSource     = 0;
	cec_config.callbacks           = &cec_callbacks;
	cec_config.deviceTypes.Add(CEC::CEC_DEVICE_TYPE_RECORDING_DEVICE);

	//ec_callbacks.CBCecKeyPress    = &tvControl::onKeypress;

	// Get a cec adapter
	/*cec_adapter = LibCecInitialise(&cec_config);
	if( !cec_adapter ) {
        	cec = false;
 	} else {
		// Determine the CEC devices
		CEC::cec_adapter devices[10];
		int8_t devices_found = cec_adapter->FindAdapters(devices, 10, NULL);
		if( devices_found <= 0) {
        		cec = false;
			UnloadLibCec(cec_adapter);
		} else {
			if( !cec_adapter->Open(devices[0].comm) ) {
				cec = false;
				UnloadLibCec(cec_adapter);
			}
		}
	}*/
}

tvControl::~tvControl() {
	//cec_adapter->Close();
	//UnloadLibCec(cec_adapter);
}

void tvControl::tvOn() {
	
}

void tvOff() {
	
}

// CEC listen * Not sure if ill use this.
int tvControl::onKeypress(void* not_used, const CEC::cec_keypress msg) {
	 switch( msg.keycode ) {
	}
}
