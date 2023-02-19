#ifndef _TERMISTOR_HPP_
# define _TERMISTOR_HPP_

# include <Arduino.h>
# include <math.h>

// отключает ненужное, но в случае нужды функций быстрого переключения внутрених переменных, закоментируйте define
#define __TERMISTOR_LOW_PERFOMANCE_MODE
class Termistor {
    // в случае если ресурсы ограничены, то лучше использовать define, а не переменные
    #ifdef LOW_PERFOMANCE_MODE
    # define __TERMISTOR_LOW_PERFOMANCE_MODE
    #endif

    #ifdef __TERMISTOR_LOW_PERFOMANCE_MODE
    /* НАСТРОЙКИ ФУНКЦИИ ПОД ОПРЕДЕНЕННЫЙ АППАРАТ */
    // Параметры передаются конструктору класса. Здесь они не нуждаются
    // # define _beta 3950         // тепловой кофф. 
    // # define _tempBase 25       // температура при которой термистор дает _base
    // # define _baseDivRes (_base / _resistance)
    // # define _base 10000        // сопротивление при 25 градусах цельсия
    // # define _resistance 10000  // сопротивление баласта, стоящего напротив термистора(который подтянут к плюсу)
    
    # define _ADC_BITS_ 12 // кол-во бит АЦП. ESP32 = 12, arduino uno/nano = 10
    # define _max_ADC ((1<<_ADC_BITS_) - 1) // 1023.0f для 10-ти битного АЦП, 4095.0f для 12-ти битного
    # define _T_SAMPLE_AVERAGE 20   // количество чтений для усреднения
    #endif

    public:
        Termistor(uint8_t pin, uint16_t beta = 3950, uint32_t resistance = 10000, uint8_t tempBase = 25, uint32_t base = 10000):
                _pin(pin), _beta(beta), _tempBase(tempBase), _baseDivRes((float)base / resistance) {}
        float getAverage();
        float computeTemp(float analog);
        float getTemp();
        void begin();

        uint8_t get_pin(){return _pin;};

        #ifndef __TERMISTOR_LOW_PERFOMANCE_MODE
            void ADC_BIT_SET(uint8_t adc_bit):_max_ADC((float)((1<<adc_bit) - 1)){}
            void SAMPLE_AVERAGE(uint16_t average): _T_SAMPLE_AVERAGE(average){}
        #endif

    private:
        const uint8_t _pin = 0;
        const uint16_t _beta = 3950;
        const uint8_t _tempBase = 25;
        const float _baseDivRes;
        
        #ifndef __TERMISTOR_LOW_PERFOMANCE_MODE
        float _max_ADC = 4095.0f;
        uint16_t _T_SAMPLE_AVERAGE = 20;
        #endif
};

float getTemp(uint8_t);

#endif