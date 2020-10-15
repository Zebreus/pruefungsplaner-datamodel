#ifndef DAY_H
#define DAY_H

class Day;

#include <QObject>
#include <QString>
#include "serializabledataobject.h"
#include "timeslot.h"

class Day : public SerializableDataObject {
  using SerializableDataObject::SerializableDataObject;
  Q_OBJECT
  Q_PROPERTY(QString getName READ getName WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(QList<Timeslot*> timeslots READ getTimeslots WRITE setTimeslots
                 NOTIFY timeslotsChanged)
 public:
  explicit Day(QObject* parent = nullptr);

  // SerializableDataObject interface
  void fromJsonObject(const QJsonObject& content);
  QJsonObject toJsonObject() const;

  QString getName() const;
  void setName(const QString& name);
  QList<Timeslot*> getTimeslots() const;
  void setTimeslots(QList<Timeslot*> timeslots);

 signals:
  void nameChanged(const QString name);
  void timeslotsChanged(const QList<Timeslot*> timeslots);

 private:
  QString name;
  QList<Timeslot*> timeslots;
};

#endif  // DAY_H
