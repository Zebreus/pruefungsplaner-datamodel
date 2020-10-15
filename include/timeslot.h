#ifndef TIMESLOT_H
#define TIMESLOT_H

class Timeslot;

#include <QObject>
#include <QString>
#include "group.h"
#include "module.h"
#include "plan.h"
#include "serializabledataobject.h"

class Timeslot : public SerializableDataObject {
  Q_OBJECT
  Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(QList<Module*> modules READ getModules WRITE setModules NOTIFY
                 modulesChanged)
  Q_PROPERTY(QList<Group*> activeGroups READ getActiveGroups WRITE
                 setActiveGroups NOTIFY activeGroupsChanged)
 public:
  using SerializableDataObject::SerializableDataObject;
  explicit Timeslot(QObject* parent = nullptr);

  // SerializableDataObject interface
  void fromJsonObject(const QJsonObject& content);
  QJsonObject toJsonObject() const;

  QString getName() const;
  void setName(const QString& name);
  QList<Module*> getModules() const;
  void setModules(QList<Module*> modules);
  QList<Group*> getActiveGroups() const;
  void setActiveGroups(QList<Group*> activeGroups);

 public slots:
  bool containsActiveGroup(Group* gp);
  void addActiveGroup(Group* gp);
  void removeActiveGroup(Group* gp);
  bool containsModule(Module* gp);
  void addModule(Module* gp);
  void removeModule(Module* gp);

 signals:
  void nameChanged(const QString name);
  void modulesChanged(const QList<Module*> modules);
  void activeGroupsChanged(const QList<Group*> activeGroups);

 private:
  QString name;
  QList<Module*> modules;
  QList<Group*> activeGroups;
};

#endif  // TIMESLOT_H
