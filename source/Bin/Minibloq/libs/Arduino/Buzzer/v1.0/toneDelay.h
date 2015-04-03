#ifndef toneDelay_h
#define toneDelay_h

inline void toneWithDelay(uint8_t _pin, unsigned int frequency, unsigned long duration)
{
    tone(_pin, frequency, duration);
    delay(duration * 1.30);
}

#endif