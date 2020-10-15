#include <semester.h>

Semester::Semester(QObject* parent) : SerializableDataObject(parent) {}
QString Semester::getName() const {
  return name;
}

void Semester::setName(const QString& name) {
  if (this->name == name)
    return;

  this->name = name;
  emit nameChanged(this->name);
}

QList<Plan*> Semester::getPlans() const {
  return this->plans;
}

void Semester::setPlans(QList<Plan*> plans) {
  if (this->plans == plans)
    return;

  this->plans = plans;
  emit plansChanged(this->plans);
}

void Semester::fromJsonObject(const QJsonObject& content) {
  simpleValuesFromJsonObject(content);

  QJsonArray plansJsonArray = content.value("plans").toArray();
  plans = fromObjectJsonArray<Plan>(plansJsonArray);
}

QJsonObject Semester::toJsonObject() const {
  return recursiveToJsonObject();
}
