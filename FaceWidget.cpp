#include "FaceWidget.h"
#include "Common.h"      // gives us  extern U8G2_... u8g2;
#include <QPainter>

FaceWidget::FaceWidget(QWidget* parent) : QWidget(parent) {
    setMinimumSize(256, 128);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setStyleSheet("background-color:#000;");
}

void FaceWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::SmoothPixmapTransform, false);
    const QRect r = rect();
    int scale = qMin(r.width() / 128, r.height() / 64);
    if (scale < 1) scale = 1;
    const int w = 128 * scale, h = 64 * scale;
    const int x = (r.width()  - w) / 2;
    const int y = (r.height() - h) / 2;
    p.drawImage(QRect(x, y, w, h), u8g2.image());
}