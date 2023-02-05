#include "Termistor.hpp"

float Termistor::computeTemp(float analog) {
  analog = _baseDivRes / (_max_ADC / analog - 1.0);
  analog = (log(analog) / _beta) + 1.0 / (_tempBase + 273.15);
  return (1.0 / analog - 273.15);
}

float Termistor::getAverage() {
  uint16_t aver = 0;
  for (uint8_t i = 0; i < _T_SAMPLE_AVERAGE; i++) aver += analogRead(_pin);
  return (float)aver / _T_SAMPLE_AVERAGE;
}

float Termistor::getTemp() {
  return computeTemp(getAverage());
}

void Termistor::begin() {
    pinMode(_pin, INPUT);
}


// Code to use on C without use classes
/* 
float getAverage(uint8_t _pin) {
  #define _T_SAMPLE_AVERAGE 20   // количество чтений для усреднения
  
  uint16_t aver = 0;
  for (uint8_t i = 0; i < _T_SAMPLE_AVERAGE; i++) aver += analogRead(_pin);
  return (float)aver / _T_SAMPLE_AVERAGE;
}

float computeTemp(float analog) {
  // НАСТРОЙКИ ФУНКЦИИ ПОД ОПРЕДЕНЕННЫЙ АППАРАТ
  #define _beta 3950         // тепловой кофф
  #define _tempBase 25       // температура при которой термистор дает _base
  #define _base 10000        // сопротивление при 25 градусах цельсия
  #define _resistance 10000  // сопротивление баласта, стоящего напротив термистора(который подтянут к плюсу)
  #define _ADC_BITS_ 12 // кол-во бит АЦП. ESP32 = 12, arduino uno/nano = 10
  #define _max_ADC ((1<<_ADC_BITS_) - 1) // 1023.0f для 10-ти битного АЦП, 4095.0f для 12-ти битного
  #define _baseDivRes (_base / _resistance)
  
  analog = _baseDivRes / (_max_ADC / analog - 1.0);
  analog = (log(analog) / _beta) + 1.0 / (_tempBase + 273.15);
  return (1.0 / analog - 273.15);
}

float getTemp(uint8_t pin) {
  return computeTemp(getAverage(pin));
}
*/