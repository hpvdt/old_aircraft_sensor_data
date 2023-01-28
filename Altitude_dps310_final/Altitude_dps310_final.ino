// This example shows how to read temperature/pressure from adafruit library
//walkthrough and code: https://learn.adafruit.com/adafruit-dps310-precision-barometric-pressure-sensor/arduino

#include <Adafruit_DPS310.h>

Adafruit_DPS310 dps;

// Can also use SPI!
#define DPS310_CS 10 //will use I^2C (since its simpler) with arduino nano
  // Note on pin hardware: SCK = I2C SCL = A5 on arduino nano, SDI=SDA=A4, follow tutorial

float pressure= -1;
float temperature=-1;//global vars, -1 placeholder

//wrapper function
void measurements(){//updates global vars with new measurements
  sensors_event_t temp_event, pressure_event;
  dps.getEvents(&temp_event, &pressure_event);//setup code

  pressure = pressure_event.pressure;//updated measurements
  temperature = temp_event.pressure;
}


void setup() {
  Serial.begin(115200);//edits baud frequency
  while (!Serial) delay(10);

  Serial.println("DPS310");//placeholder to show ti works

  if (! dps.begin_I2C()) {             // Can pass in I2C address here (picked SPI)
  //if (! dps.begin_SPI(DPS310_CS)) {  // If you want to use SPI
  
  //as expected it fails for I2C when using SPI configuration -_-, also 115200 baud necessary
  //rewired with both SPI (complex) and I2C (few wires)
    Serial.println("Failed to find DPS");
    while (1) yield();
  }
  Serial.println("DPS OK!");

  dps.configurePressure(DPS310_64HZ, DPS310_64SAMPLES);
  dps.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);

  measurements();//wrapper testing 
  Serial.print("Current Temperature = ");
  Serial.println(temperature);
  Serial.print("Current Pressure = ");
  Serial.println(pressure);


//now will convert to altitude 



}//Checks is its connected

void loop() {//does basic temp and pressure info
  sensors_event_t temp_event, pressure_event;
  
  while (!dps.temperatureAvailable() || !dps.pressureAvailable()) {
    return; // wait until there's something to read
  }

  dps.getEvents(&temp_event, &pressure_event);
  Serial.print(F("Temperature = "));
  Serial.print(temp_event.temperature);
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  Serial.print(pressure_event.pressure);
  Serial.println(" hPa"); 

  Serial.println();
}