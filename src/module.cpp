#include <module.h>

Module::Module(QObject* parent) : SerializableDataObject(parent) {}
QString Module::getName() const {
  return name;
}

void Module::setName(const QString& name) {
  if (name == this->name)
    return;

  this->name = name;
  emit nameChanged(name);
}

QString Module::getOrigin() const {
  return origin;
}

void Module::setOrigin(const QString& origin) {
  if (origin == this->origin)
    return;

  this->origin = origin;
  emit originChanged(origin);
}

QString Module::getNumber() const {
  return number;
}

void Module::setNumber(const QString& number) {
  if (number == this->number)
    return;

  this->number = number;
  emit numberChanged(number);
}

bool Module::getActive() const {
  return active;
}

void Module::setActive(const bool active) {
  if (active == this->active)
    return;

  this->active = active;
  emit activeChanged(active);
}

QString Module::getExamType() const {
  return examType;
}

void Module::setExamType(QString examType) {
  if (this->examType == examType)
    return;

  if (examType != "P" && examType != "K")
    return;

  this->examType = examType;
  emit examTypeChanged(examType);
}

unsigned int Module::getExamDuration() const {
  return examDuration;
}

void Module::setExamDuration(unsigned int examDuration) {
  if (this->examDuration == examDuration)
    return;

  if (examDuration == 0)
    return;

  this->examDuration = examDuration;
  emit examDurationChanged(this->examDuration);
}

QList<Group*> Module::getConstraints() const {
  return constraints;
}

void Module::setConstraints(QList<Group*> constraints) {
  if (this->constraints == constraints)
    return;

  this->constraints = constraints;
  emit constraintsChanged(this->constraints);
}

QList<Group*> Module::getGroups() const {
  return groups;
}

void Module::setGroups(QList<Group*> groups) {
  if (this->groups == groups)
    return;

  this->groups = groups;
  emit groupsChanged(this->groups);
}

void Module::removeGroup(Group* group) {
  int removed = groups.removeAll(group);
  // int removed = 0;
  if (removed > 0) {
    emit groupsChanged(this->groups);
  }
}

void Module::removeConstraint(Group* constraint) {
  int removed = constraints.removeAll(constraint);
  // int removed = 0;
  if (removed > 0) {
    emit constraintsChanged(this->constraints);
  }
}

void Module::fromJsonObject(const QJsonObject& content) {
  simpleValuesFromJsonObject(content);
  Plan* activePlan = (Plan*)this->parent();

  groups =
      fromIdJsonArray<Group>(content.value("groups"), activePlan->getGroups());
  constraints = fromIdJsonArray<Group>(content.value("constraints"),
                                       activePlan->getConstraints());
}

QJsonObject Module::toJsonObject() const {
  return recursiveToJsonObject();
}
