# 12V Automated Battery Backup Lighting System

This README contains the complete Bill of Materials (BOM) and links for building an automated 12V LiFePO4 emergency lighting system using an Arduino Nano ESP32.

## ⚠️ Safety Warning
This project uses a high-capacity (100Ah) battery. A short circuit can cause extreme heat, fire, or component destruction. **Never skip the inline fuse.** Always double-check your logic-level (3.3V) and power-level (12V) isolation before connecting the battery.

## Core Components (The Heavy Lifters)

These are the primary components needed to power, control, and charge the system.

1. **The Battery (Power Source):**
   * [GreenOE 12V 100Ah Mini LiFePO4 Battery](https://www.walmart.com/ip/GreenOE-12V-100Ah-Mini-LiFePO4-Battery-Deep-Cycle-Battery-100A-BMS-1280Wh-Up-15000-Cycles-Lithium-RVs-Trolling-Motor-Solar-System/16777073833)
   * *Alternative Reference:* [Google Search: 12V LiFePO4](https://www.google.com/search?q=12V+LiFePO4)
2. **The Brain (Microcontroller):**
   * [Arduino Nano ESP32 (with headers)](https://store-usa.arduino.cc/products/nano-esp32-with-headers)
3. **The Lights (Illumination):**
   * [12V Flexible LED Strips (LEDSupply)](https://www.ledsupply.com/12v-flexible-led-strips)
4. **The Charger (Battery Maintainer):**
   * [Mighty Max 12V 7Amp Smart Charger for LiFePO4 (Home Depot)](https://www.homedepot.com/p/Mighty-Max-Battery-12V7Amp-Battery-Smart-Charger-Maintainer-for-Car-Truck-Boat-ATV-Lithium-LiFePO4-Lead-Acid-Deep-Cycle-Battery-Charger-MAX4094904/334273296)
5. **The Switch (MOSFET):**
   * [Logic-Level N-Channel MOSFETs (Digikey / Micro Commercial Co)](https://www.digikey.com/en/product-highlight/m/micro-commercial-co/logic-level-n-channel-mosfets) 
   * *Recommended Model:* IRLZ44N

## Supporting Components (The Glue)

To make the system safe and to connect the logic board to the grid monitor, you will need these critical smaller parts:

1. **Grid Monitor Sensor:**
   * **5V USB Wall Power Adapter:** Any standard phone charger block works perfectly. Plug it into the same wall outlet as the battery charger.
   * *Link:* [Standard 5V USB Adapters (Amazon)](https://www.amazon.com/s?k=5v+1a+usb+wall+charger)
   * **Sacrificial USB Cable:** You will cut one end off this to wire the 5V and Ground to the ESP32 (through the voltage divider).
2. **Circuit Protection (Crucial):**
   * **Inline Blade Fuse Holder (10A to 15A):** This goes directly on the positive terminal of the 12V battery.
   * *Link:* [12V Inline Automotive Fuse Holder (Amazon)](https://www.amazon.com/s?k=12v+inline+fuse+holder+14+awg)
3. **Resistor Kit (Voltage Divider & Pull-Down):**
   * You need a **10kΩ** and **20kΩ** resistor to step down the 5V USB signal to a safe 3.3V for the ESP32 input pin. You also need a **10kΩ** resistor to pull down the MOSFET gate to ground.
   * *Link:* [Standard 1/4W Resistor Assortment Kit (Amazon)](https://www.amazon.com/s?k=resistor+kit+1%2F4w)
4. **Wiring & Connectors:**
   * **16 AWG or 14 AWG Wire:** For the 12V high-current power runs (Battery -> Fuse -> LED Strips -> MOSFET).
   * **22 AWG Solid Core Wire:** For connecting the logic pins on the breadboard or directly to the ESP32.
   * *Link:* [Primary Wire Spools (Amazon)](https://www.amazon.com/s?k=16+awg+silicone+wire)
  
## Wiring Instructions

### **1. Preparing the Power Circuit (12V High-Current & Logic Power)**
* Use 16 AWG or 14 AWG wire for all high-current 12V power runs. 
* Solder the 10A to 15A inline blade fuse holder directly to the wire connecting to the positive terminal of the 12V battery.
* Split the output of the inline fuse into two paths:
  * **Path A (Lights):** Solder directly to the positive wire of the 12V flexible LED strips.
  * **Path B (ESP32 Power):** Solder a 22 AWG solid core wire to the `VIN` pin on the Arduino Nano ESP32. *(This critical step ensures the microcontroller remains powered by the battery when the grid fails).*

### **2. Assembling the Grid Monitor (Voltage Divider)**
* Cut one end off a sacrificial USB cable and strip the shielding to expose the 5V and Ground wires.
* Create a voltage divider by soldering the 10kΩ and 20kΩ resistors together to step the 5V signal down to 3.3V.
* Solder the 5V wire from the USB cable to the input of the voltage divider.
* Connect and solder a 22 AWG solid core wire to the 3.3V center of your voltage divider. 
* Solder the other end of this 22 AWG wire to Pin 2 (`GRID_MONITOR_PIN`) on the ESP32.

Here is the corrected Step 3, incorporating the 12V-to-5V Buck Converter to properly power the logic level shifter and ensure the MOSFET fully saturates.

### **3. Assembling the Control Circuit (Buck Converter, Logic Level Shifter & MOSFET)**
* *Note: A 12V-to-5V Buck Converter and a 3.3V to 5V Logic Level Shifter are added here to properly step up the ESP32's signal. This ensures the MOSFET fully saturates to handle the high current of the LED strips safely.*
* Tap into the 12V power line (after the inline fuse) and solder wires to the `IN+` and `IN-` terminals of the 12V-to-5V Buck Converter.
* Wire the **High Voltage (HV)** reference pin on the Logic Level Shifter to the 5V `OUT+` on the Buck Converter.
* Wire the **Low Voltage (LV)** reference pin on the Logic Level Shifter to the ESP32's `3.3V` pin.
* Solder a 22 AWG solid core wire from Pin 3 (`MOSFET_GATE_PIN`) on the ESP32 to the signal input channel on the LV side of the shifter.
* Position your IRLZ44N Logic-Level N-Channel MOSFET.
* Solder a 10kΩ pull-down resistor directly between the MOSFET Gate and the system ground.
* Solder a wire from the signal output channel on the HV side of the shifter to the MOSFET Gate.
* Solder the negative wire of the LED strips to the MOSFET Drain.
* Solder the MOSFET Source to the system ground.

### **4. Final Grounding & Safety Checks**
* Ensure that the ESP32 ground, Logic Level Shifter grounds, USB cable ground, and MOSFET Source are all connected together to form a common system ground.
* Verify that the 12V high-power rail and the 3.3V/5V logic rails remain completely separated to ensure safe operation.

## High-Level Wiring Reminder
* Keep the **12V High-Power Rail** separated from the **3.3V Logic Rail**.
* The 5V USB Adapter outputs 5V. Use the 10k/20k resistors to step this down to 3.3V before connecting it to an ESP32 GPIO pin.
* The ESP32 logic pin connects to the MOSFET **Gate**.
* The MOSFET **Source** connects to System Ground.
* The MOSFET **Drain** connects to the negative wire of the LED strips.
* The positive wire of the LED strips connects directly to the 12V battery (after the inline fuse).
