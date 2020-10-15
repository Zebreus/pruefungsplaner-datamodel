#ifndef CONSTRAINT_H
#define CONSTRAINT_H

class Group;

#include <QObject>
#include <QString>
#include "serializabledataobject.h"

class Group : public SerializableDataObject {
  using SerializableDataObject::SerializableDataObject;
  Q_OBJECT
  Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(
      bool selected READ getSelected WRITE setSelected NOTIFY selectedChanged)
  Q_PROPERTY(unsigned int examsPerDay READ getExamsPerDay WRITE setExamsPerDay
                 NOTIFY examsPerDayChanged)
 public:
  explicit Group(QObject* parent = nullptr);

  // SerializableDataObject interface
  void fromJsonObject(const QJsonObject& content);
  QJsonObject toJsonObject() const;

  QString getName() const;
  void setName(const QString& name);
  bool getSelected() const;
  void setSelected(const bool selected);
  unsigned int getExamsPerDay() const;
  void setExamsPerDay(unsigned int newExamsPerDay);

 signals:
  void nameChanged(const QString name);
  void selectedChanged(const bool selected);
  void examsPerDayChanged(const unsigned int examsPerDay);

 private:
  QString name;
  bool selected;
  unsigned int examsPerDay;
};

#endif  // CONSTRAINT_H
