#ifndef SEMESTER_H
#define SEMESTER_H

class Semester;

#include <QObject>
#include <QString>
#include "plan.h"

class Semester : public SerializableDataObject {
  using SerializableDataObject::SerializableDataObject;
  Q_OBJECT
  Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(
      QList<Plan*> plans READ getPlans WRITE setPlans NOTIFY plansChanged)

 public:
  explicit Semester(QObject* parent = nullptr);

  // SerializableDataObject interface
  void fromJsonObject(const QJsonObject& content);
  QJsonObject toJsonObject() const;

  QString getName() const;
  void setName(const QString& name);
  QList<Plan*> getPlans() const;
  void setPlans(QList<Plan*> plans);

 signals:
  void nameChanged(const QString name);
  void plansChanged(const QList<Plan*> plans);

 private:
  QString name;
  QList<Plan*> plans;
};

#endif  // SEMESTER_H
