 #include "Wire.h"
 #include <MPU6050_light.h>
 MPU6050 mpu(Wire);
 unsigned long timer = 0;


 #include <Adafruit_BMP280.h>

#define BMP_SCK (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS (10)

Adafruit_BMP280 bmp; // I2C
// Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
// Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

/////////////////////////////////////////////////
float currentPitch,currentRoll,currentYaw,currentAlt,currentAltGround,currentAltLocal,currentVertSpeed;
float setAlt, setPitch , setRoll, setYaw;
float prevAlt;
unsigned int prevMillisVertSpeed = 0;
unsigned int delayVertSpeed = 2

00; //vertspeed is calculated every 500 ms
////////////////////////////////////////////////


void TaskMPU(){
     mpu.update();
 if ((millis() - timer) > 10) { // print data every 10ms
     currentPitch = mpu.getAngleX();
     currentRoll = mpu.getAngleY();
     currentYaw = mpu.getAngleZ();
     timer = millis();
}
}

void TaskBMP(){
    /*
    Serial.print(F("Temperature = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure());
    Serial.println(" Pa"); */

    currentAlt = bmp.readAltitude(1011);
    currentAltLocal = currentAlt-currentAltGround;
    
    currentAltLocal = currentAltLocal * 10;//decreasing decimel precision for reducing noise
    currentAltLocal = (int)currentAltLocal;
    currentAltLocal = (float)currentAltLocal;
    currentAltLocal = currentAltLocal/10;
}

void TaskMeasureVertSpeed(){
  if(delayVertSpeed < millis() - prevMillisVertSpeed){
    
  currentVertSpeed = (currentAltLocal - prevAlt)/(millis() - prevMillisVertSpeed); // meter per millisecond
  prevMillisVertSpeed = millis();
  prevAlt = currentAltLocal;
  currentVertSpeed = currentVertSpeed * 1000; // meter per second
  }
}

void TaskTelemetry(){
  Serial.print("X :");
  Serial.print(currentPitch);
  Serial.print("  Y :");
  Serial.print(currentRoll);
  Serial.print("  Z :");
  Serial.print(currentYaw);
  Serial.print("  Alt :"); 
  Serial.print(currentAlt);
  Serial.print("m");
  Serial.print("  LocAlt :");
  Serial.print(currentAltLocal);
  Serial.print("m");
  Serial.print("  VertSpeed :");
  Serial.println(currentVertSpeed);
  }



 
 void setup() {
   Serial.begin(9600);
   Wire.begin();

  Serial.println("-------FlyNation Firmware version : 0.0.1beta--------------");
   
 byte status = mpu.begin();
   Serial.print(F("MPU6050 status: "));
   Serial.println(status);
   while (status != 0) { } // stop everything if could not connect to MPU6050
 Serial.println(F("Calculating offsets, do not move MPU6050"));
   delay(1000);
   mpu.calcOffsets(); // gyro and accelero
   Serial.println("Done!\n");


Serial.println(F("BMP280 test"));
  //unsigned status;
  // status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
  status = bmp.begin();
  if (!status)
  {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                     "try a different address!"));
    Serial.print("SensorID was: 0x");
    Serial.println(bmp.sensorID(), 16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1)
      delay(10);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X8,      /* Filtering. */ /*Default:FILTER_X16*/
                  Adafruit_BMP280::STANDBY_MS_63); /* Standby time. */

   currentAltGround = bmp.readAltitude(1011);
 }


 void loop(){
  TaskMPU();
  TaskBMP();
  TaskMeasureVertSpeed();
  TaskTelemetry();
  }
