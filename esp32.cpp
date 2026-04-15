// --- PIN DEFINITIONS ---
// Adjust these numbers based on which pins you use on your Nano ESP32
const int GRID_MONITOR_PIN = 2; // Connected to the 3.3V center of your voltage divider
const int MOSFET_GATE_PIN = 3;  // Connected to the Gate of the IRLZ44N MOSFET

// --- STATE TRACKING ---
// We track the state to prevent sending the ON/OFF command 100 times a second
int currentGridState = HIGH;      // Assume grid is up on boot
int previousGridState = HIGH;
unsigned long lastDebounceTime = 0;  
const unsigned long DEBOUNCE_DELAY = 100; // Milliseconds to wait before confirming an outage

void setup() {
  // 1. Start the Serial Monitor for debugging via USB
  Serial.begin(115200);
  
  // 2. Configure the Pins
  // INPUT_PULLDOWN ensures the pin reads LOW if the wire accidentally disconnects
  pinMode(GRID_MONITOR_PIN, INPUT_PULLDOWN); 
  pinMode(MOSFET_GATE_PIN, OUTPUT);
  
  // 3. Failsafe: Explicitly force the lights OFF on boot
  digitalWrite(MOSFET_GATE_PIN, LOW);
  
  Serial.println("Backup Lighting System Initialized.");
}

void loop() {
  // Read the raw voltage from the wall adapter (HIGH = Grid Up, LOW = Outage)
  int reading = digitalRead(GRID_MONITOR_PIN);

  // If the reading flickers or changes, reset the debounce timer
  if (reading != previousGridState) {
    lastDebounceTime = millis();
  }

  // If the reading has stayed consistent longer than our debounce delay...
  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    
    // ...and if it's a completely new state...
    if (reading != currentGridState) {
      currentGridState = reading; // Update the official system state

      // Trigger the Hardware
      if (currentGridState == HIGH) {
        // Grid is UP. 
        digitalWrite(MOSFET_GATE_PIN, LOW); // Drop the Gate voltage to 0V. Lights OFF.
        Serial.println("Grid Status: ACTIVE. Lights OFF.");
      } else {
        // Grid is DOWN. 
        digitalWrite(MOSFET_GATE_PIN, HIGH); // Push the Gate voltage to 3.3V. Lights ON.
        Serial.println("Grid Status: FAILED! Emergency Lights ON.");
      }
    }
  }

  // Save the current reading for the next loop comparison
  previousGridState = reading;
}
