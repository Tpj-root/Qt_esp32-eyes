// Arduino.h  --- Qt shim for ESP32-Eyes
#ifndef ARDUINO_SHIM_H
#define ARDUINO_SHIM_H

#include <cstdint>
#include <QElapsedTimer>
#include <QRandomGenerator>

typedef unsigned char byte;
typedef bool          boolean;

#ifndef HIGH
#  define HIGH 1
#  define LOW  0
#endif
#ifndef INPUT
#  define INPUT        1
#  define OUTPUT       2
#  define INPUT_PULLUP 0
#endif

template <typename T> inline T min(T a, T b) { return a < b ? a : b; }
template <typename T> inline T max(T a, T b) { return a > b ? a : b; }

inline unsigned long millis() {
    static QElapsedTimer t;
    if (!t.isValid()) t.start();
    return static_cast<unsigned long>(t.elapsed());
}

inline long random(long hi) {
    if (hi <= 0) return 0;
    return static_cast<long>(QRandomGenerator::global()->bounded(static_cast<int>(hi)));
}
inline long random(long lo, long hi) {
    if (hi <= lo) return lo;
    return static_cast<long>(QRandomGenerator::global()
                             ->bounded(static_cast<int>(lo), static_cast<int>(hi)));
}

#endif