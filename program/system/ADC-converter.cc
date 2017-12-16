#include "ADC-converter.h"

std::vector<Adafruit_ADS1115> ADC_Converter::ADS1105_list;

void ADC_Converter::begin() {
    for(auto & ADS1105: ADS1105_list) {
        ADS1105.begin();
    }
}

void ADC_Converter::addModule(uint8_t i2cAddress) {
    ADS1105_list.push_back(Adafruit_ADS1115(i2cAddress));
}

uint16_t ADC_Converter::readADC(uint8_t module, uint8_t channel) {
    return ADS1105_list[module].readADC_SingleEnded(channel);
}

float ADC_Converter::readVoltage(uint8_t module, uint8_t chanel) {
    switch (ADS1105_list[module].m_gain) {
        case GAIN_TWOTHIRDS: {
            return (float)(readADC(module, chanel)*ADS1015_RESOLUTION_6_144V/1000);
        } break;
        case GAIN_ONE: {
            return (float)(readADC(module, chanel)*ADS1015_RESOLUTION_4_096V/1000);
        } break;
        case GAIN_TWO: {
            return (float)(readADC(module, chanel)*ADS1015_RESOLUTION_2_048V/1000);
        } break;
        case GAIN_FOUR: {
            return (float)(readADC(module, chanel)*ADS1015_RESOLUTION_1_024V/1000);
        } break;
        case GAIN_EIGHT: {
            return (float)(readADC(module, chanel)*ADS1015_RESOLUTION_0_512V/1000);
        } break;
        case GAIN_SIXTEEN: {
            return (float)(readADC(module, chanel)*ADS1015_RESOLUTION_0_256V/1000);
        } break;
        default: return -100;   // not real
    }
}

float ADC_Converter::readCurrent(uint8_t module, uint8_t chanel, uint16_t resgister) {
    return readVoltage(module, chanel)/resgister;
}
