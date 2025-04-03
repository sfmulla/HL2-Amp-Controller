
// This is firmware for the Hermes Lite 2 IO board designed by Saleh Almulla, 7Z1CY. 
//    Saleh Almulla <sfmulla@hotmail.com>.


// This firmware outputs the band Voltage to control any amplifier with icom ACC
// the voltage output using pwm for fan control to J3 pin
//  Band		Frq. MHz		Band Voltage
//	160			1.8				7.0 ~ 8.0V
//	80			3.5				6.0 ~ 7.0V
//	40			7				5.0 ~ 6.0V
//	20			14				4.0 ~ 5.0V
//	17&12		18 & 21			3.0 ~ 4.0V
//	12 & 10		24 & 28			2.0 ~ 3.0V
//
//
// also it output 5.0v to GPIO10_Out5 (J4 pin 5) when PTT on (transsmition on) 
// as PTT output to key the amplifier
//
// if you coneect your I/O board PICO to your PC using USB you can see the working Band



#include "../hl2ioboard.h"
#include "../i2c_registers.h"

// These are the major and minor version numbers for firmware. You must set these.
uint8_t firmware_version_major=1;
uint8_t firmware_version_minor=3;

// PTT function to set J4 pin 5 when PTT on
void PTT(bool tx)
{
	// Operate PTT
	if (tx) {
		gpio_put(GPIO10_Out5, 0);   // In RX, release PTT
		
	} else {
		gpio_put(GPIO10_Out5, 1);   // Apply PTT
		printf("PTT ON\n");
		}
}

int main()
{
	static uint8_t current_tx_fcode = 0;
	static bool current_is_rx = true;
	static uint8_t tx_band = 0;
	static uint8_t rx_band = 0;
	// uint8_t band, fcode;
	bool is_rx;
	// bool change_band;
	uint8_t i;

	stdio_init_all();
	configure_pins(false, true);
	configure_led_flasher();

	while (1) {	// Wait for something to happen
		sleep_ms(1);	// This sets the polling frequency.


		// Poll for a changed Tx band, Rx band and T/R change
		is_rx = gpio_get(GPIO13_EXTTR);		// true for receive, false for transmit
		if (current_is_rx != is_rx) {
			current_is_rx = is_rx;

			// Update TX state on GPIO10_Out5
			if (current_is_rx) {
				gpio_put(GPIO10_Out5, 0);
			} else {
				gpio_put(GPIO10_Out5, 1);
				}
		}
		PTT(current_is_rx);
		// Poll for a changed Tx frequency. The new_tx_fcode is set in the I2C handler.
		if (current_tx_fcode != new_tx_fcode) {
			current_tx_fcode = new_tx_fcode;
			tx_band = fcode2band(current_tx_fcode);		// Convert the frequency code to a band code.
			
			printf("TX Freq: %011d\n", tx_band);
			switch (tx_band) {
			case BAND_160:
				pwm_set_chan_level(FAN_SLICE, FAN_CHAN, FAN_WRAP * 165 / 255);				
				break;
			case BAND_80:
				pwm_set_chan_level(FAN_SLICE, FAN_CHAN, FAN_WRAP * 145 / 255);				
				break;
			case BAND_40:
				pwm_set_chan_level(FAN_SLICE, FAN_CHAN, FAN_WRAP * 125 / 255);				
				break;
			case BAND_20:
				pwm_set_chan_level(FAN_SLICE, FAN_CHAN, FAN_WRAP * 102 / 255);				
				break;
			case BAND_17:
				pwm_set_chan_level(FAN_SLICE, FAN_CHAN, FAN_WRAP * 80 / 255);
				break;
			case BAND_15:
				pwm_set_chan_level(FAN_SLICE, FAN_CHAN, FAN_WRAP * 80 / 255);
				break;
			case BAND_12:
				pwm_set_chan_level(FAN_SLICE, FAN_CHAN, FAN_WRAP * 60 / 255);
				break;
			case BAND_10:
				pwm_set_chan_level(FAN_SLICE, FAN_CHAN, FAN_WRAP * 60 / 255);
				break;
			default:	// This includes band zero (reset)
				pwm_set_chan_level(FAN_SLICE, FAN_CHAN, 0);

			}
		}

		// Poll for a change in one of the twelve Rx frequencies. The rx_freq_changed is set in the I2C handler.
		if (rx_freq_changed) {
			rx_freq_changed = false;
			if (rx_freq_high == 0)
				rx_band = tx_band;
			else
				rx_band = fcode2band(rx_freq_high);	// Convert the frequency code to a band code.

			
			printf("RX Freq: %011d\n", rx_band);
		}

	}
}
