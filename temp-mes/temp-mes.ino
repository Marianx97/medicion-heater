#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 4 on the Arduino
#define ONE_WIRE_BUS 4

// button connected to pin 2
#define BUTTON_PIN 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// Number of temperature devices found
int numberOfDevices;

// We'll use this variable to store a found device address
DeviceAddress tempDeviceAddress;

// Counter
int count = 0;

// flag to track if button was pressed
bool samplingStarted = false;

void setup(void) {
  // start serial port
  Serial.begin(9600);

  // use internal pull-up resistor
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  byte addr[8];
  while(oneWire.search(addr)) {
    Serial.print("Address = ");
    for(int i = 0; i < 8; i++) {
      Serial.print(" 0x");
      Serial.print(addr[i], HEX);
    }
    Serial.println();
  }
  oneWire.reset_search();

  // Start up the library
  sensors.begin();

  // Grab a count of devices on the wire
  numberOfDevices = sensors.getDeviceCount();

  // locate devices on the bus
  Serial.print("Locating devices... Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");

  // Loop through each device, print out address
  for(int i = 0; i < numberOfDevices; i++) {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i)) {
      Serial.print("Found device ");
      Serial.print(i, DEC);
      Serial.print(" with address: ");
      printAddress(tempDeviceAddress);
      Serial.println();
    } else {
      Serial.print("Found ghost device at ");
      Serial.print(i, DEC);
      Serial.println(" (check wiring)");
    }
  }

  Serial.println("Press the button to start temperature sampling...");
}

void loop(void) {
  // Wait for button press to start sampling
  if (!samplingStarted) {
    if (digitalRead(BUTTON_PIN) == LOW) {
      samplingStarted = true;
      Serial.println("Button pressed! Starting temperature sampling...");
      delay(500); // debounce delay
    } else {
      return; // do nothing until button pressed
    }
  }

  // Send the command to get temperatures
  sensors.requestTemperatures();

  Serial.print("med ");
  Serial.print(count);

  // Loop through each device, print out temperature data
  for(int i = 0; i < numberOfDevices; i++) {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i)) {
      // Output the device ID
      Serial.print("  T");
      Serial.print(i, DEC);
      Serial.print("=");

      // Print the data
      float tempC = sensors.getTempC(tempDeviceAddress);
      Serial.print(tempC);
    }
  }

  Serial.println();
  count += 2;
  delay(2000);
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
