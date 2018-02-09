#ifndef ADC_CONVERTER_H
#define ADC_CONVERTER_H

#include <iostream>
#include <vector>
#include "Adafruit_ADS1015.h"

#define ADDRESS_ADC 0x48
#define R 220 // Ohm
#define pressure_min -100  // kpa
#define pressure_max 100   // kpa
#define min_val R*0.004  // Volt
#define max_val R*0.02 // Volt

#define MODULE_1 0
#define MODULE_2 1

class ADC_Converter {

public:
    void begin();
    void addModule(uint8_t i2cAddress);
    uint16_t readADC(uint8_t module, uint8_t channel);                  // read raw ADC output
    float readVoltage(uint8_t module, uint8_t channel);               // read voltage in chanel. Unit (V)
    float readCurrent(uint8_t module, uint8_t channel, uint16_t resgister);   // current = voltage/R. Unit (A)
    static std::vector<Adafruit_ADS1115> ADS1105_list;
};

#endif
