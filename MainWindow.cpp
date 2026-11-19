#include "MainWindow.h"
#include "FaceWidget.h"
#include "Face.h"
#include "Arduino.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QDockWidget>
#include <QGroupBox>
#include <QTimer>

static QSlider* makeSlider(int lo, int hi, int val) {
    auto* s = new QSlider(Qt::Horizontal);
    s->setRange(lo, hi);
    s->setValue(val);
    return s;
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // ---- Face construction (same sequence as the .ino setup()) ----
    _face = new Face(128, 64, 40);
    _face->Expression.GoTo_Normal();
    _face->RandomBehavior = true;
    _face->RandomBlink    = true;
    _face->RandomLook     = false;
    _face->Blink.Timer.SetIntervalMillis(4000);

    // ---- Central display ----
    _widget = new FaceWidget(this);
    setCentralWidget(_widget);

    // ---- Controls dock ----
    auto* ctl = new QWidget;
    auto* v   = new QVBoxLayout(ctl);

    v->addWidget(new QLabel("Look X  (0 = left  … 100 = centre … 200 = right)"));
    _lookX = makeSlider(0, 200, 100); v->addWidget(_lookX);

    v->addWidget(new QLabel("Look Y  (0 = down  … 100 = centre … 200 = up)"));
    _lookY = makeSlider(0, 200, 100); v->addWidget(_lookY);

    auto* moodsBox = new QGroupBox("Mood weights (0 – 2)");
    auto* mvb = new QVBoxLayout(moodsBox);
    mvb->addWidget(new QLabel("Angry"));     _mAngry = makeSlider(0,200,0); mvb->addWidget(_mAngry);
    mvb->addWidget(new QLabel("Sad"));       _mSad   = makeSlider(0,200,0); mvb->addWidget(_mSad);
    mvb->addWidget(new QLabel("Surprised")); _mSurp  = makeSlider(0,200,0); mvb->addWidget(_mSurp);
    v->addWidget(moodsBox);


    v->addWidget(new QLabel("Force emotion:"));
    _emotionBox = new QComboBox;
    _emotionBox->addItem("Normal",      (int)eEmotions::Normal);
    _emotionBox->addItem("Angry",       (int)eEmotions::Angry);
    _emotionBox->addItem("Glee",        (int)eEmotions::Glee);
    _emotionBox->addItem("Happy",       (int)eEmotions::Happy);
    _emotionBox->addItem("Sad",         (int)eEmotions::Sad);
    _emotionBox->addItem("Worried",     (int)eEmotions::Worried);
    _emotionBox->addItem("Focused",     (int)eEmotions::Focused);
    _emotionBox->addItem("Annoyed",     (int)eEmotions::Annoyed);
    _emotionBox->addItem("Surprised",   (int)eEmotions::Surprised);
    _emotionBox->addItem("Skeptic",     (int)eEmotions::Skeptic);
    _emotionBox->addItem("Frustrated",  (int)eEmotions::Frustrated);
    _emotionBox->addItem("Unimpressed", (int)eEmotions::Unimpressed);
    _emotionBox->addItem("Sleepy",      (int)eEmotions::Sleepy);
    _emotionBox->addItem("Suspicious",  (int)eEmotions::Suspicious);
    _emotionBox->addItem("Squint",      (int)eEmotions::Squint);
    _emotionBox->addItem("Furious",     (int)eEmotions::Furious);
    _emotionBox->addItem("Scared",      (int)eEmotions::Scared);
    _emotionBox->addItem("Awe",         (int)eEmotions::Awe);
    v->addWidget(_emotionBox);

    auto* forceBtn = new QPushButton("Apply emotion");
    connect(forceBtn, &QPushButton::clicked, this, &MainWindow::onForceEmotion);
    v->addWidget(forceBtn);


    auto* btn = new QPushButton("Blink");
    connect(btn, &QPushButton::clicked, this, &MainWindow::onBlink);
    v->addWidget(btn);

    _rand = new QCheckBox("Random Behaviour");
    _rand->setChecked(true);
    connect(_rand, &QCheckBox::toggled, this,
            [this](bool on){ _face->RandomBehavior = on; });
    v->addWidget(_rand);
    v->addStretch();

    auto* dock = new QDockWidget("Controls", this);
    dock->setWidget(ctl);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    resize(900, 400);

    // ---- Render loop ~30 fps ----
    _timer = new QTimer(this);
    _timer->setInterval(33);
    connect(_timer, &QTimer::timeout, this, &MainWindow::onTick);
    _timer->start();
}

MainWindow::~MainWindow() { delete _face; }

void MainWindow::onTick() {
    // Mirrors the throttled look update from the original .ino
    static unsigned long lastMoveTime = 0;
    if (millis() - lastMoveTime > 500) {
        const float x = (_lookX->value() - 100) / 100.0f;   // -1 … +1
        const float y = (_lookY->value() - 100) / 100.0f;   // -1 … +1
        _face->Look.LookAt(x, y);
        lastMoveTime = millis();
    }

    // Mood weights from sliders (0 – 2)
    _face->Behavior.SetEmotion(eEmotions::Angry,     _mAngry->value() / 100.0f);
    _face->Behavior.SetEmotion(eEmotions::Sad,       _mSad  ->value() / 100.0f);
    _face->Behavior.SetEmotion(eEmotions::Surprised, _mSurp ->value() / 100.0f);

    _face->Update();          // runs the whole animation pipeline
    _widget->update();        // schedule a repaint of the OLED image
}

void MainWindow::onBlink() { _face->DoBlink(); }

void MainWindow::onForceEmotion() {
    int idx = _emotionBox->currentData().toInt();
    _face->Behavior.GoToEmotion((eEmotions)idx);   // ← directly triggers Expression.GoTo_*()
    _face->RandomBehavior = false;                 // freeze so the roulette doesn't override it
    _rand->setChecked(false);
}