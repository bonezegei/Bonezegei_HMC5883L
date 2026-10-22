/*
  Bonezegei_HMC5883L.h
  Arduino library for HMC5883L 3-Axis Digital Compass/Magnetometer
*/

#ifndef BONEZEGEI_HMC5883L_H
#define BONEZEGEI_HMC5883L_H

#include <Arduino.h>
#include <Wire.h>

#define HMC5883L_I2C_ADDR       0x1E

// Registers
#define HMC5883L_REG_CONFIG_A   0x00
#define HMC5883L_REG_CONFIG_B   0x01
#define HMC5883L_REG_MODE       0x02
#define HMC5883L_REG_OUT_X_M    0x03
#define HMC5883L_REG_OUT_X_L    0x04
#define HMC5883L_REG_OUT_Z_M    0x05
#define HMC5883L_REG_OUT_Z_L    0x06
#define HMC5883L_REG_OUT_Y_M    0x07
#define HMC5883L_REG_OUT_Y_L    0x08
#define HMC5883L_REG_STATUS     0x09
#define HMC5883L_REG_IDA        0x0A

// Measurement Modes
#define HMC5883L_MODE_CONTINUOUS 0x00
#define HMC5883L_MODE_SINGLE     0x01
#define HMC5883L_MODE_IDLE       0x03

// Gain Ranges
typedef enum {
  HMC5883L_RANGE_0_88GA = 0x00, // ±0.88 Ga
  HMC5883L_RANGE_1_3GA  = 0x01, // ±1.3 Ga (Default)
  HMC5883L_RANGE_1_9GA  = 0x02, // ±1.9 Ga
  HMC5883L_RANGE_2_5GA  = 0x03, // ±2.5 Ga
  HMC5883L_RANGE_4_0GA  = 0x04, // ±4.0 Ga
  HMC5883L_RANGE_4_7GA  = 0x05, // ±4.7 Ga
  HMC5883L_RANGE_5_6GA  = 0x06, // ±5.6 Ga
  HMC5883L_RANGE_8_1GA  = 0x07  // ±8.1 Ga
} hmc5883l_range_t;

class Bonezegei_HMC5883L {
public:
  Bonezegei_HMC5883L(uint8_t addr = HMC5883L_I2C_ADDR);

  bool begin(TwoWire &wirePort = Wire);
  bool read();
  
  int16_t getRawX();
  int16_t getRawY();
  int16_t getRawZ();

  float getX();
  float getY();
  float getZ();

  float getHeading(float declinationAngleRad = 0.0);
  
  void setRange(hmc5883l_range_t range);

private:
  uint8_t _address;
  TwoWire *_wire;
  float _mgPerLsb;

  int16_t _rawX;
  int16_t _rawY;
  int16_t _rawZ;

  void writeRegister(uint8_t reg, uint8_t val);
  uint8_t readRegister(uint8_t reg);
};

#endif