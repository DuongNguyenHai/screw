// g++ -std=c++11 -Wall lib/base/Wire.o lib/Adafruit_ADS1015.o system/ADC-converter.o test-pressure-vacuum.cc -Ilib -Ilib/base -Isystem -o test-vacuum

#include <iostream>
#include <unistd.h>
#include "ADC-converter.h"


float convertKpa(float x) {
  return (float)((x-min_val) * (pressure_max - pressure_min) / (max_val-min_val) + pressure_min);
}

int main(int argc, char const *argv[]) {

    ADC_Converter adc;
    adc.addModule(0x48);    // add the first module. index of this module will be 0
    adc.begin();

    for(;;) {
        // uint16_t a = adc.readADC(MODULE_1, CHANEL_0);

        // std::cout << "Raw ADC: " << a << ","
        //           << "P: " << convertKpa(a*0.1875/1000) << " (kpa),"
        //           << "Voltage: " << a*0.1875/1000 << " (V),"
        //           << "Current: " << (a*0.1875/1000)*1000/220 << " (mA)" << std::endl;
        //
        // float vol = adc.readVoltage(MODULE_1, CHANEL_0);
        // std::cout << "Voltage: " << vol << " (V)"
        //             << ", P: " << convertKpa(vol) << " kpa" << std::endl;

        std::cout << "P: " << convertKpa(adc.readVoltage(MODULE_1, CHANEL_0)) << std::endl;

        // uint16_t b = adc.readADC(MODULE_1, CHANEL_1);
        // std::cout << "Raw ADC: " << b << ","
        //         << "Voltage: " << b*0.1875/1000 << " (V)" << std::endl;
        usleep(100000);
    }
    return 0;
}
