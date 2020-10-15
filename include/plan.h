#ifndef PLAN_H
#define PLAN_H

class Plan;

#include <QJsonObject>
#include <QMetaObject>
#include <QMetaProperty>
#include <QObject>
#include <QString>
#include <QVariant>
#include <iostream>
#include "group.h"
#include "module.h"
#include "week.h"

using namespace std;

class Plan : public SerializableDataObject {
  using SerializableDataObject::SerializableDataObject;
  Q_OBJECT
  Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(QList<Group*> constraints READ getConstraints WRITE setConstraints
                 NOTIFY constraintsChanged)
  Q_PROPERTY(
      QList<Group*> groups READ getGroups WRITE setGroups NOTIFY groupsChanged)
  Q_PROPERTY(
      QList<Week*> weeks READ getWeeks WRITE setWeeks NOTIFY weeksChanged)
  Q_PROPERTY(QList<Module*> modules READ getModules WRITE setModules NOTIFY
                 modulesChanged)

 public:
  explicit Plan(QObject* parent = nullptr);

  // SerializableDataObject interface
  void fromJsonObject(const QJsonObject& content);
  QJsonObject toJsonObject() const;

  QString getName() const;
  void setName(const QString& name);
  QList<Group*> getConstraints() const;
  void setConstraints(QList<Group*> constraints);
  QList<Group*> getGroups() const;
  void setGroups(QList<Group*> groups);
  QList<Week*> getWeeks() const;
  void setWeeks(QList<Week*> weeks);
  QList<Module*> getModules() const;
  void setModules(QList<Module*> modules);

 public slots:
  void addNewGroup(const QString& name);
  void removeGroup(Group* gp);
  void addNewConstraint(const QString& name);
  void removeConstraint(Group* gp);

 signals:
  void nameChanged(const QString name);
  void modulesChanged(const QList<Module*> modules);
  void constraintsChanged(const QList<Group*> groups);
  void groupsChanged(const QList<Group*> groups);
  void weeksChanged(const QList<Week*> weeks);

 private:
  QString name;
  QList<Group*> constraints;
  QList<Group*> groups;
  QList<Module*> modules;
  QList<Week*> weeks;
};

#endif  // PLAN_H
