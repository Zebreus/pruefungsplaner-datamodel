#include <group.h>

Group::Group(QObject* parent)
    : SerializableDataObject(parent),
      selected(false),
      examsPerDay(0),
      active(true),
      small(false),
      obsolete(false) {}

QString Group::getName() const {
  return name;
}

void Group::setName(const QString& name) {
  if (this->name == name)
    return;

  this->name = name;
  emit nameChanged(name);
}

bool Group::getSelected() const {
  return selected;
}

void Group::setSelected(const bool selected) {
  if (selected == this->selected)
    return;

  this->selected = selected;
  emit selectedChanged(selected);
}

unsigned int Group::getExamsPerDay() const {
  return examsPerDay;
}

void Group::setExamsPerDay(unsigned int examsPerDay) {
  if (examsPerDay == this->examsPerDay)
    return;

  this->examsPerDay = examsPerDay;
  emit examsPerDayChanged(examsPerDay);
}

bool Group::getActive() const {
  return active;
}

void Group::setActive(bool active) {
  if (this->active == active)
    return;

  this->active = active;
  emit activeChanged(active);
}

bool Group::getSmall() const {
  return small;
}

void Group::setSmall(bool small) {
  if (this->small == small)
    return;

  this->small = small;
  emit smallChanged(small);
}

bool Group::getObsolete() const {
  return obsolete;
}

void Group::setObsolete(bool obsolete) {
  if (this->obsolete == obsolete)
    return;

  this->obsolete = obsolete;
  emit obsoleteChanged(obsolete);
}

void Group::fromJsonObject(const QJsonObject& content) {
  simpleValuesFromJsonObject(content);
}

QJsonObject Group::toJsonObject() const {
  return recursiveToJsonObject();
}
