// U8g2lib.h  --- Qt shim that draws into a QImage
#ifndef U8G2LIB_SHIM_H
#define U8G2LIB_SHIM_H

#include <QImage>
#include <QPainter>
#include <QPolygon>
#include <QColor>
#include <memory>

#define U8G2_R0      0
#define U8G2_R1      1
#define U8G2_R2      2
#define U8G2_R3      3
#define U8X8_PIN_NONE 255

class U8G2_SSD1306_128X64_NONAME_F_HW_I2C {
public:
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C(int = 0, int = 255, int = 0, int = 0) {
        _image = QImage(128, 64, QImage::Format_ARGB32);
        _image.fill(Qt::black);
    }

    const QImage& image() const { return _image; }

    void setI2CAddress(int) {}
    void begin()            {}

    void clearBuffer() {
        if (_painter && _painter->isActive()) _painter->end();
        _painter.reset();
        _image.fill(Qt::black);
        _painter = std::make_unique<QPainter>(&_image);
        _painter->setRenderHint(QPainter::Antialiasing, false);
        _color = Qt::white;
    }

    void sendBuffer() {
        if (_painter) { _painter->end(); _painter.reset(); }
    }

    // void setDrawColor(int c) { _color = (c == 0) ? Qt::black : Qt::white; }
    // void setDrawColor(int c) { _color = (c == 0) ? Qt::black : QColor(0,255,180); }
    void setDrawColor(int c) { _color = (c == 0) ? Qt::black : Qt::cyan; }


    void drawPixel(int x, int y) {
        if (!_painter || x < 0 || x >= 128 || y < 0 || y >= 64) return;
        _painter->setPen(_color); _painter->drawPoint(x, y);
    }
    void drawHLine(int x, int y, int w) {
        if (!_painter || w <= 0 || y < 0 || y >= 64) return;
        _painter->setPen(_color);
        _painter->drawLine(x, y, x + w - 1, y);
    }
    void drawVLine(int x, int y, int h) {
        if (!_painter || h <= 0 || x < 0 || x >= 128) return;
        _painter->setPen(_color);
        _painter->drawLine(x, y, x, y + h - 1);
    }
    void drawBox(int x, int y, int w, int h) {
        if (!_painter || w <= 0 || h <= 0) return;
        _painter->fillRect(x, y, w, h, _color);
    }
    void drawFrame(int x, int y, int w, int h) {
        if (!_painter) return;
        _painter->setPen(_color); _painter->setBrush(Qt::NoBrush);
        _painter->drawRect(x, y, w - 1, h - 1);
    }
    void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2) {
        if (!_painter) return;
        QPolygon p; p << QPoint(x0,y0) << QPoint(x1,y1) << QPoint(x2,y2);
        _painter->setPen(_color); _painter->setBrush(_color);
        _painter->drawPolygon(p);
    }

private:
    QImage  _image;
    std::unique_ptr<QPainter> _painter;
    QColor  _color = Qt::white;
};

#endif