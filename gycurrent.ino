#include <Adafruit_INA219.h>
#include <Wire.h>
const int gyx = 0x68;
int GyX,GyY,GyZ;
float pitch=0;
float roll=0;
float yaw=0;
float v_pitch;
float v_roll;
float v_yaw;
float a_pitch;
float a_roll;
float a_yaw;
int cur = 0x40;
Adafruit_INA219 ina219(cur);
void setup() {
  Wire.begin();
  Wire.beginTransmission(gyx);
  delay(100);
  Wire.beginTransmission(cur);
  delay(100);
  Wire.write(0x68); //power management register 1
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
 // Serial.begin(115200);
  while (!Serial) {
      // will pause Zero, Leonardo, etc until serial console opens
    delay(1000);
  }

  uint32_t currentFrequency;
    
  Serial.println("Hello!");

  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }

  Serial.println("Measuring voltage and current with INA219 ...");


}

void loop() {
  Wire.beginTransmission(gyx);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(gyx,6,true);
  GyX=Wire.read()<<8|Wire.read();
  GyY=Wire.read()<<8|Wire.read();
  GyZ=Wire.read()<<8|Wire.read();
  v_pitch=(GyX/131);
  if(v_pitch==-1) {v_pitch=0;}
  v_roll=(GyY/131);
  if(v_roll==1) {v_roll=0;}
  v_yaw=GyZ/131;
  a_pitch=(v_pitch*0.046);
  a_roll=(v_roll*0.046);
  a_yaw=(v_yaw*0.045);
  pitch= pitch + a_pitch; roll= roll + a_roll;
  yaw= yaw + a_yaw;
  Serial.print(" | pitch = ");
  Serial.print(pitch);
  Serial.print(" | roll = ");
  Serial.print(roll);
  Serial.print(" | yaw = ");
  Serial.println(yaw); 
  //Serial.println("");
  delay(1000);
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println("");

  
}
