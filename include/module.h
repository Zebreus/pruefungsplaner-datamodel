#ifndef MODULE_H
#define MODULE_H

class Module;

#include <QList>
#include <QObject>
#include <QString>
#include <iostream>
#include <vector>
#include "group.h"
#include "plan.h"
#include "serializabledataobject.h"

using namespace std;

class Module : public SerializableDataObject {
  using SerializableDataObject::SerializableDataObject;
  Q_OBJECT
  Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(QString origin READ getOrigin WRITE setOrigin NOTIFY originChanged)
  Q_PROPERTY(QString number READ getNumber WRITE setNumber NOTIFY numberChanged)
  Q_PROPERTY(bool active READ getActive WRITE setActive NOTIFY activeChanged)
  Q_PROPERTY(QString examType READ getExamType WRITE setExamType NOTIFY
                 examTypeChanged)
  Q_PROPERTY(unsigned int examDuration READ getExamDuration WRITE
                 setExamDuration NOTIFY examDurationChanged)
  Q_PROPERTY(QList<Group*> constraints READ getConstraints WRITE setConstraints
                 NOTIFY constraintsChanged)
  Q_PROPERTY(
      QList<Group*> groups READ getGroups WRITE setGroups NOTIFY groupsChanged)

 public:
  explicit Module(QObject* parent = nullptr);

  // SerializableDataObject interface
  void fromJsonObject(const QJsonObject& content);
  QJsonObject toJsonObject() const;

  QString getName() const;
  void setName(const QString& name);
  QString getOrigin() const;
  void setOrigin(const QString& origin);
  QString getNumber() const;
  void setNumber(const QString& number);
  bool getActive() const;
  void setActive(const bool active);
  QString getExamType() const;
  void setExamType(QString examType);
  unsigned int getExamDuration() const;
  void setExamDuration(unsigned int examDuration);
  QList<Group*> getConstraints() const;
  void setConstraints(QList<Group*> constraints);
  QList<Group*> getGroups() const;
  void setGroups(QList<Group*> groups);

 public slots:
  void removeGroup(Group* group);
  void removeConstraint(Group* constraint);

 signals:
  void nameChanged(const QString name);
  void originChanged(const QString origin);
  void numberChanged(const QString number);
  void activeChanged(const bool active);
  void constraintsChanged(const QList<Group*> constraints);
  void groupsChanged(const QList<Group*> groups);
  void examTypeChanged(const QString examType);
  void examDurationChanged(const unsigned int examDuration);

 private:
  QString name;
  QString origin;
  QString number;
  bool active;
  QString examType;
  unsigned int examDuration;
  QList<Group*> constraints;
  QList<Group*> groups;
};

#endif  // MODULE_H
