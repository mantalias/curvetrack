#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"

static constexpr int I2C_SDA = 32;
static constexpr int I2C_SCL = 33;
static constexpr int SD_SCK  = 27;
static constexpr int SD_MISO = 25;
static constexpr int SD_MOSI = 26;
static constexpr int SD_CS   = 14;
static constexpr int BTN_PIN = 12;

MPU6050 accelgyro(0x68);
SPIClass customSPI(HSPI);
File myFile;

int16_t ax, ay, az;
float offsetX = 0, offsetY = 0, offsetZ = 0; 
volatile bool buttonPressed = false;

void calibrate();
void write_logs(const float &gX, const float &gY, const float &gZ);
void IRAM_ATTR handleButtonInterrupt();

void setup() {
    Serial.begin(115200);
    Wire.begin(I2C_SDA, I2C_SCL); 
    customSPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS); 
    pinMode(BTN_PIN, INPUT_PULLUP);

    accelgyro.initialize();
    if (!accelgyro.testConnection()) {
        Serial.println("MPU6050 connection failed!");
    }
    
    accelgyro.setFullScaleAccelRange(0); 
    
    if (!SD.begin(SD_CS, customSPI, 1000000)) {
        Serial.println("SD Card Failed!");
        return;
    }

    myFile = SD.open("/sensorlog.txt", FILE_APPEND);
    attachInterrupt(digitalPinToInterrupt(BTN_PIN), handleButtonInterrupt, FALLING);
}

void loop() {
    accelgyro.getAcceleration(&ax, &ay, &az);
    float gX = ((float)ax - offsetX) / 16384.0;
    float gY = ((float)ay - offsetY) / 16384.0;
    float gZ = ((float)az - offsetZ) / 16384.0;
    
    write_logs(gX, gY, gZ);

    if(buttonPressed){
        buttonPressed = false; // Fixed assignment
        calibrate();
    }
}

void calibrate(){
    Serial.println("Calibrating...");
    long sumX = 0, sumY = 0, sumZ = 0;
    int samples = 300;

    for (int i = 0; i < samples; i++) {
        accelgyro.getAcceleration(&ax, &ay, &az);
        sumX += ax;
        sumY += ay;
        sumZ += az;
        delay(10); 
    }

    offsetX = (float)sumX / samples;
    offsetY = (float)sumY / samples;
    offsetZ = ((float)sumZ / samples) - 16384; 

    File calLog = SD.open("/calibration.txt", FILE_APPEND);

    if (calLog) {
        calLog.println("calibrated 1 time");
        calLog.close();
        Serial.println("Calibration logged.");
    }
}

void write_logs(const float &gX, const float &gY, const float &gZ){
    static int counter = 0;
    if (myFile) {
        myFile.print(millis()/1000.0, 3);
        myFile.print(", ");
        myFile.print(gX, 3);
        myFile.print(", ");
        myFile.print(gY, 3);
        myFile.print(", ");
        myFile.println(gZ, 3);
        counter = counter + 1;

        if(counter == 100){
            myFile.flush();
            counter = 0;
        }
        
    }
}

void IRAM_ATTR handleButtonInterrupt() {
    static unsigned long lastInterruptTime = 0;
    unsigned long interruptTime = millis();
    
    if (interruptTime - lastInterruptTime > 200) { //an einai makrina ta press tote einai real press
        buttonPressed = true;
    }
    lastInterruptTime = interruptTime;
}