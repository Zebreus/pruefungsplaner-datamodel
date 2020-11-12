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
  Q_PROPERTY(bool active READ getActive WRITE setActive NOTIFY activeChanged)
  Q_PROPERTY(bool small READ getSmall WRITE setSmall NOTIFY smallChanged)
  Q_PROPERTY(
      bool obsolete READ getObsolete WRITE setObsolete NOTIFY obsoleteChanged)
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
  void setExamsPerDay(const unsigned int newExamsPerDay);
  void setActive(const bool active);
  bool getActive() const;
  void setSmall(const bool small);
  bool getSmall() const;
  void setObsolete(const bool obsolete);
  bool getObsolete() const;

 signals:
  void nameChanged(const QString name);
  void selectedChanged(const bool selected);
  void examsPerDayChanged(const unsigned int examsPerDay);
  void activeChanged(const bool active);
  void smallChanged(const bool small);
  void obsoleteChanged(const bool obsolete);

 private:
  QString name;
  bool selected;
  unsigned int examsPerDay;
  bool active;
  bool small;
  bool obsolete;
};

#endif  // CONSTRAINT_H
