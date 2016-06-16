#include "tvControl.h"

tvControl::tvControl() {
	// Setup lirc
	//fd = lirc_get_local_socket(NULL, 0);
	//lirc_send_one(fd, "RCA", "KEY_POWER");

	// Setup CEC if available.
	cec = true;
	//bcm_host_init();
	cecConfig.Clear();

	const std::string devicename("TV");
	devicename.copy(cecConfig.strDeviceName, std::min(devicename.size(),13u) );

	cecConfig.clientVersion       = CEC::LIBCEC_VERSION_CURRENT;
	cecConfig.bActivateSource     = 0;
	cecConfig.deviceTypes.Add(CEC::CEC_DEVICE_TYPE_RECORDING_DEVICE);

	//ec_callbacks.CBCecKeyPress    = &tvControl::onKeypress;

	// Get a cec adapter
	cecAdapter = CECInitialise(&cecConfig);
	if (cecAdapter != NULL) {
		cec = true;

		cecAdapter->GetDeviceInformation(port, &cecConfig, 10000);
		device = cecAdapter->GetActiveSource();
		if (device != CEC::CECDEVICE_UNKNOWN) {
			cecAdapter->Open(port,10000);
		}
	}
}

tvControl::~tvControl() {
	if (cec){
		cecAdapter->Close();
		CECDestroy(cecAdapter);
	}
}

void tvControl::tvOn() {
	if (cec) {
		cecAdapter->PowerOnDevices();
	}
}

void tvControl::tvOff() {
	if (cec) {
		cecAdapter->StandbyDevices();
	}
}

// CEC listen * Not sure if ill use this.
int tvControl::onKeypress(void* not_used, const CEC::cec_keypress msg) {
	 switch( msg.keycode ) {
	}
}
