#ifndef FACEWIDGET_H
#define FACEWIDGET_H

#include <QWidget>

class FaceWidget : public QWidget {
    Q_OBJECT
public:
    explicit FaceWidget(QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent*) override;
};

#endif