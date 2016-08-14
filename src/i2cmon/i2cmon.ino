#include <Wire.h>

void writeI2CRegister8bit(int addr, int reg, int value) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

void writeI2CRegister8bit(int addr, int value) {
  Wire.beginTransmission(addr);
  Wire.write(value);
  Wire.endTransmission();
}

unsigned int readI2CRegister16bit(int addr, int reg) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  delay(50);
  Wire.requestFrom(addr, 2);
  unsigned int t = Wire.read() << 8;
  t = t | Wire.read();
  return t;
}

unsigned char readI2CRegister8bit(int addr, int reg) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  delay(50);
  Wire.requestFrom(addr, 1);
  unsigned char t = Wire.read();
  return t;
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Setup");
                                       //talking to the default address 0x20
  //writeI2CRegister8bit(0x20, 1, 0x21); //change address to 0x21
  //writeI2CRegister8bit(0x20, 6);       //reset
  delay(1000);                         //give it some time to boot
}

/*loop scans I2C bus and displays foud addresses*/ 
void loop()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      Serial.print("Ver: ");
      Serial.println(readI2CRegister8bit(address, 7), HEX);
      delay(500);

      Serial.print("Capacitance: ");
      Serial.println(readI2CRegister16bit(address, 0));
      delay(500);

      Serial.print("Temperature: ");
      Serial.println(readI2CRegister16bit(address, 5)/(float)10);
      delay(500);

      Serial.print("Light: ");
      writeI2CRegister8bit(address, 3); //request light measurement 
      delay(500);                   //this can take a while
      Serial.println(readI2CRegister16bit(address, 4)); //read light register
      writeI2CRegister8bit(address, 3);

      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknow error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(5000);           // wait 5 seconds for next scan
}
