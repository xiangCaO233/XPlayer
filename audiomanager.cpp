#include "audiomanager.h"

#include "ui_audiomanager.h"

AudioManager::AudioManager(std::shared_ptr<XOutputDevice> device,
                           QWidget *parent)
    : QWidget(parent), outdevice(device), ui(new Ui::AudioManager) {
  ui->setupUi(this);
}

AudioManager::~AudioManager() { delete ui; }
