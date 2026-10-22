/*
  Bonezegei_HMC5883L.cpp
  Implementation file for HMC5883L 3-Axis Digital Compass
*/

#include "Bonezegei_HMC5883L.h"

Bonezegei_HMC5883L::Bonezegei_HMC5883L(uint8_t addr) {
  _address = addr;
  _mgPerLsb = 0.92; // Default scale for ±1.3 Ga
  _rawX = 0;
  _rawY = 0;
  _rawZ = 0;
}

bool Bonezegei_HMC5883L::begin(TwoWire &wirePort) {
  _wire = &wirePort;
  _wire->begin();

  // Verify chip ID (Registers 0x0A, 0x0B, 0x0C return "H43")
  uint8_t idA = readRegister(HMC5883L_REG_IDA);
  if (idA != 'H') {
    return false;
  }

  // Config A: 8-sample average, 15 Hz default output rate, normal measurement
  writeRegister(HMC5883L_REG_CONFIG_A, 0x70);

  // Set gain range to ±1.3 Gauss default
  setRange(HMC5883L_RANGE_1_3GA);

  // Set Mode: Continuous Measurement Mode
  writeRegister(HMC5883L_REG_MODE, HMC5883L_MODE_CONTINUOUS);

  return true;
}

void Bonezegei_HMC5883L::setRange(hmc5883l_range_t range) {
  switch (range) {
    case HMC5883L_RANGE_0_88GA: _mgPerLsb = 0.73; break;
    case HMC5883L_RANGE_1_3GA:  _mgPerLsb = 0.92; break;
    case HMC5883L_RANGE_1_9GA:  _mgPerLsb = 1.22; break;
    case HMC5883L_RANGE_2_5GA:  _mgPerLsb = 1.52; break;
    case HMC5883L_RANGE_4_0GA:  _mgPerLsb = 2.27; break;
    case HMC5883L_RANGE_4_7GA:  _mgPerLsb = 2.56; break;
    case HMC5883L_RANGE_5_6GA:  _mgPerLsb = 3.03; break;
    case HMC5883L_RANGE_8_1GA:  _mgPerLsb = 4.35; break;
  }

  writeRegister(HMC5883L_REG_CONFIG_B, range << 5);
}

bool Bonezegei_HMC5883L::read() {
  _wire->beginTransmission(_address);
  _wire->write(HMC5883L_REG_OUT_X_M);
  if (_wire->endTransmission() != 0) {
    return false;
  }

  if (_wire->requestFrom(_address, (uint8_t)6) != 6) {
    return false;
  }

  // Note: HMC5883L register order is X, Z, Y
  _rawX = (int16_t)(_wire->read() << 8 | _wire->read());
  _rawZ = (int16_t)(_wire->read() << 8 | _wire->read());
  _rawY = (int16_t)(_wire->read() << 8 | _wire->read());

  return true;
}

int16_t Bonezegei_HMC5883L::getRawX() { return _rawX; }
int16_t Bonezegei_HMC5883L::getRawY() { return _rawY; }
int16_t Bonezegei_HMC5883L::getRawZ() { return _rawZ; }

float Bonezegei_HMC5883L::getX() { return _rawX * _mgPerLsb; }
float Bonezegei_HMC5883L::getY() { return _rawY * _mgPerLsb; }
float Bonezegei_HMC5883L::getZ() { return _rawZ * _mgPerLsb; }

float Bonezegei_HMC5883L::getHeading(float declinationAngleRad) {
  float heading = atan2((float)_rawY, (float)_rawX) + declinationAngleRad;

  // Correct for angle wrap-around
  if (heading < 0) {
    heading += 2 * PI;
  }
  if (heading > 2 * PI) {
    heading -= 2 * PI;
  }

  // Convert radians to degrees
  return heading * (180.0 / PI);
}

void Bonezegei_HMC5883L::writeRegister(uint8_t reg, uint8_t val) {
  _wire->beginTransmission(_address);
  _wire->write(reg);
  _wire->write(val);
  _wire->endTransmission();
}

uint8_t Bonezegei_HMC5883L::readRegister(uint8_t reg) {
  _wire->beginTransmission(_address);
  _wire->write(reg);
  _wire->endTransmission();
  _wire->requestFrom(_address, (uint8_t)1);
  return _wire->read();
}