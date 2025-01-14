// AUTHOR: Nisheli P - MakerBotics
// VERSION: 1 
//    DATE: 02/07/2024
// PURPOSE: Display BME680 sensor data on a 1.3-inch OLED Display and RP2040 Core Board.
//     License details: 
//     Attribution-ShareAlike 4.0 International
//     CC BY-SA 4.0 Deed.

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_BME680.h>

// Define OLED display width and height
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Define I2C addresses
#define OLED_I2C_ADDR 0x3C  // Common I2C address for SH110X OLED
#define BME680_I2C_ADDR 0x77  // Default I2C address for BME680

// Initialize OLED and BME680
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_BME680 bme;

void setup() {
  // Initialize Serial for debugging
  Serial.begin(115200);
  unsigned long startTime = millis();
  while (!Serial && (millis() - startTime < 3000)) {
    // Wait up to 3 seconds for Serial connection
  }

  // Initialize OLED display
  if (!display.begin(OLED_I2C_ADDR, true)) {
    Serial.println(F("Failed to initialize OLED display!"));
    for (;;);
  }
  display.clearDisplay();
  display.setRotation(0);  // Optional: Adjust rotation
  display.display();
  Serial.println(F("OLED initialized."));

  // Initialize BME680 sensor
  if (!bme.begin(BME680_I2C_ADDR)) {
    Serial.println(F("Failed to initialize BME680 sensor!"));
    for (;;);
  }
  Serial.println(F("BME680 initialized."));

  // Set up BME680 settings
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150);  // 320°C for 150 ms
}

void loop() {
  // Ensure sensor data is available
  if (!bme.performReading()) {
    Serial.println(F("Failed to perform reading!"));
    return;
  }

  // Prepare sensor data
  float temperature = bme.temperature;
  float humidity = bme.humidity;
  float pressure = bme.pressure / 100.0;  // Convert to hPa
  float gas = bme.gas_resistance / 1000.0;  // Convert to kOhms

  // Print data to Serial
  Serial.print(F("Temperature: "));
  Serial.print(temperature);
  Serial.println(F(" °C"));

  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.println(F(" %"));

  Serial.print(F("Pressure: "));
  Serial.print(pressure);
  Serial.println(F(" hPa"));

  Serial.print(F("Gas: "));
  Serial.print(gas);
  Serial.println(F(" kOhms"));

  // Display data on OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  display.setCursor(10, 0);
  display.print(F("MakerBotics.com.au "));
 
  display.setCursor(0, 10);
  display.print(F("Temp: "));
  display.print(temperature);
  display.println(F(" C"));

  display.setCursor(0, 20);
  display.print(F("Humidity: "));
  display.print(humidity);
  display.println(F(" %"));

  display.setCursor(0, 30);
  display.print(F("Pressure: "));
  display.print(pressure);
  display.println(F(" hPa"));

  display.setCursor(0, 40);
  display.print(F("Gas: "));
  display.print(gas);
  display.println(F(" kOhms"));

  display.display();

  // Wait before the next loop
  delay(2000);
}
