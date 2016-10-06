There are two applications contained under this folder:
	- prngtest
	This application enumerates the CRC16 based PRNG on CC2538.
	- cc2538seed
	This applications demonstrates the radio jamming attack to bias the TRNG on CC2538.

How to use cc2538seed(*1):
To (best) demonstrate the attack, upload cc2538seed.bin to the device. 

Make sure the 'cheating' code are:

------------------------------------------------------------
  ...
  //This is something evil...
  {
    //Turn on PD_OVERRIDE
    //REG(RFCORE_XREG_PTEST1) = 0x08;
    //Turn off LNA and mixer.
    //REG(RFCORE_XREG_PTEST0) = 0x02 << 2;
    
    //AGC override.
    //To achieve the best bias result, tune AGCs to max and disable AAF attenuation.
    REG(RFCORE_XREG_AGCCTRL0) = 0x00; 	//Disable AAF attenuation.
    //REG (RFCORE_XREG_AGCCTRL1) = 0x00;	//Manipulate refernece voltage.
    REG (RFCORE_XREG_AGCCTRL2) = 0xFF;	//Tune all AGCs to maximum.
  }
  ...
------------------------------------------------------------

i.e. only the configuration for AGCCTRL0 and AGCCTRL2 are enabled.

Once uploaded, connect HackRF One(*2) to the device and execute:
  ./grc/attack.py

The python script pops a GRC generated executable, where the signal source is configured to Sinewave(*3).
Default parameters (frequency, offset, amplitude) are pre-configured for 802.15.4 Channel 25 which is the default for CC2538DK in Contiki.

Then slowly tune up the IF Gain until the readings of TRNG are biased. Normally the bias would appear to be visible for IF Gain around 16~20 (dB).


(*1) Test environment:
	Ubuntu 16.04 LTS with 4.4.0.38-generic #57-Ubuntu SMP
	GNU Radio Companion 3.7.9
	Hack RF Firmware 2015.07.2
	OpenMote CC2538 Rev.B
	Contiki release-3.0

(*2) Other SDRs should also work, but we only tested on HackRF One.

(*3) Although amplitude 0 indicates that the waveform doesn't matter, but the expeirments showed there are subtle differents to how HackRF One interprets the signal.
