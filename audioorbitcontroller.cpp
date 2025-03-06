#include "audioorbitcontroller.h"

#include "ui_audioorbitcontroller.h"

AudioOrbitController::AudioOrbitController(QWidget *parent)
    : QWidget(parent), ui(new Ui::AudioOrbitController) {
  ui->setupUi(this);
}

AudioOrbitController::~AudioOrbitController() { delete ui; }
