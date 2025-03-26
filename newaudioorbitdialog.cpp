#include "newaudioorbitdialog.h"

#include "AudioManager.h"
#include "ui_newaudioorbitdialog.h"

NewAudioOrbitDialog::NewAudioOrbitDialog(
    std::unordered_map<int, std::shared_ptr<XOutputDevice>> *devs,
    QWidget *parent)
    : QDialog(parent), ui(new Ui::NewAudioOrbitDialog) {
  ui->setupUi(this);
  devices = devs;
  for (const auto &[device_id, device] : (*devs)) {
    if (device_id != -1) {
      ui->device_chooser->addItem(QString::fromStdString(device->device_name),
                                  QVariant(device_id));
    }
  }
}

NewAudioOrbitDialog::~NewAudioOrbitDialog() { delete ui; }

// 设置音频名
void NewAudioOrbitDialog::set_audio_name(const QString &name) {
  ui->name_value->setText(name);
}
// 设置音频时长title(ms)
void NewAudioOrbitDialog::set_audio_duration(size_t milliseconds) {
  ui->duration_value->setText(QString::number(milliseconds) + "ms");
}
// 设置音频内存(b)
void NewAudioOrbitDialog::set_audio_memory(size_t bytes) {
  ui->memory_value->setText(QString::number(bytes) + "bytes");
}

// 获取设定
int NewAudioOrbitDialog::get_volume_setting() const {
  return ui->volume_spin_control->value();
}
int NewAudioOrbitDialog::get_speed_setting() const {
  return ui->speed_spin_control->value();
}
bool NewAudioOrbitDialog::isautoplay() const {
  return ui->autoplay_control->isChecked();
}
bool NewAudioOrbitDialog::isloop() const {
  return ui->loop_control->isChecked();
}

std::shared_ptr<XOutputDevice> NewAudioOrbitDialog::choosed_device() const {
  auto it = (*devices).find(ui->device_chooser->currentData().toInt());
  return it->second;
}
