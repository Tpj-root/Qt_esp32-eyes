#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>

class Face;
class FaceWidget;
class QSlider;
class QCheckBox;
class QTimer;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void onTick();
    void onBlink();
    void onForceEmotion();

private:
    Face*        _face   = nullptr;
    FaceWidget*  _widget = nullptr;
    QTimer*      _timer  = nullptr;
    QSlider*     _lookX  = nullptr;
    QSlider*     _lookY  = nullptr;
    QSlider*     _mAngry = nullptr;
    QSlider*     _mSad   = nullptr;
    QSlider*     _mSurp  = nullptr;
    QCheckBox*   _rand   = nullptr;
    QComboBox* _emotionBox = nullptr;
};

#endif