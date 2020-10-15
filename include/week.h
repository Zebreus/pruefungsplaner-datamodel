#ifndef WEEK_H
#define WEEK_H

class Week;

#include <QObject>
#include <QString>
#include "day.h"

class Week : public SerializableDataObject {
  using SerializableDataObject::SerializableDataObject;
  Q_OBJECT
  Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(QList<Day*> days READ getDays WRITE setDays NOTIFY daysChanged)

 public:
  explicit Week(QObject* parent = nullptr);

  // SerializableDataObject interface
  void fromJsonObject(const QJsonObject& content);
  QJsonObject toJsonObject() const;

  QString getName() const;
  void setName(const QString& name);
  QList<Day*> getDays() const;
  void setDays(QList<Day*> days);

 signals:
  void nameChanged(const QString name);
  void daysChanged(const QList<Day*> days);

 private:
  QString name;
  QList<Day*> days;
};

#endif  // WEEK_H
