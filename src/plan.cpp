#include <plan.h>

Plan::Plan(QObject* parent) : SerializableDataObject(parent) {}

QString Plan::getName() const {
  return name;
}

void Plan::setName(const QString& name) {
  if (this->name == name)
    return;

  this->name = name;
  emit nameChanged(this->name);
}

QList<Group*> Plan::getConstraints() const {
  return constraints;
}

void Plan::setConstraints(QList<Group*> constraints) {
  if (this->constraints == constraints)
    return;

  this->constraints = constraints;
  emit constraintsChanged(this->constraints);
}

QList<Group*> Plan::getGroups() const {
  return groups;
}

void Plan::setGroups(QList<Group*> groups) {
  if (this->groups == groups)
    return;

  this->groups = groups;
  emit groupsChanged(this->groups);
}

QList<Week*> Plan::getWeeks() const {
  return this->weeks;
}

void Plan::setWeeks(QList<Week*> weeks) {
  if (this->weeks == weeks)
    return;

  this->weeks = weeks;
  emit weeksChanged(this->weeks);
}

QList<Module*> Plan::getModules() const {
  return modules;
}

void Plan::setModules(QList<Module*> modules) {
  // TODO correct for removed modules or something
  if (this->modules == modules)
    return;

  this->modules = modules;
  emit modulesChanged(this->modules);
}

void Plan::addNewGroup(const QString& name) {
  Group* gp = new Group(this);
  gp->setName(name);
  groups.append(gp);
  emit groupsChanged(this->groups);
}

void Plan::removeGroup(Group* gp) {
  if (groups.removeAll(gp) > 0) {
    for (Week* week : weeks) {
      for (Day* day : week->getDays()) {
        for (Timeslot* slot : day->getTimeslots()) {
          slot->removeActiveGroup(gp);
        }
      }
    }
  }
  emit groupsChanged(this->groups);
}

void Plan::addNewConstraint(const QString& name) {
  Group* gp = new Group(this);
  gp->setName(name);
  constraints.append(gp);
  emit constraintsChanged(this->groups);
}

void Plan::removeConstraint(Group* gp) {
  if (constraints.removeAll(gp) > 0) {
    for (Week* week : weeks) {
      for (Day* day : week->getDays()) {
        for (Timeslot* slot : day->getTimeslots()) {
          slot->removeActiveGroup(gp);
        }
      }
    }
  }
  emit constraintsChanged(this->groups);
}

void Plan::fromJsonObject(const QJsonObject& content) {
  simpleValuesFromJsonObject(content);

  QJsonArray groupsJsonArray = content.value("groups").toArray();
  groups = fromObjectJsonArray<Group>(groupsJsonArray);

  QJsonArray constraintsJsonArray = content.value("constraints").toArray();
  constraints = fromObjectJsonArray<Group>(constraintsJsonArray);

  QJsonArray modulesJsonArray = content.value("modules").toArray();
  modules = fromObjectJsonArray<Module>(modulesJsonArray);

  QJsonArray weeksJsonArray = content.value("weeks").toArray();
  weeks = fromObjectJsonArray<Week>(weeksJsonArray);
}

QJsonObject Plan::toJsonObject() const {
  return recursiveToJsonObject();
}
