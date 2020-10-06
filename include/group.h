#ifndef CONSTRAINT_H
#define CONSTRAINT_H

class Group;

#include <QObject>
#include <QString>
#include "serializabledataobject.h"

class Group : public SerializableDataObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool selected READ getSelected WRITE setSelected NOTIFY selectedChanged)
    Q_PROPERTY(unsigned int examsPerDay READ getExamsPerDay WRITE setExamsPerDay NOTIFY examsPerDayChanged)
public:
    using SerializableDataObject::SerializableDataObject;
    explicit Group(QObject *parent = nullptr);
    QString name();
    void setName(const QString &name);
    bool getSelected();
    void setSelected(const bool selected);
    unsigned int getExamsPerDay();
    void setExamsPerDay(unsigned int newExamsPerDay);

signals:
    void nameChanged();
    void selectedChanged();
    void examsPerDayChanged();

private:
    QString constraintName;
    bool selected;
    unsigned int examsPerDay;


    // SerializableDataObject interface
public:
    void fromJsonObject(const QJsonObject &content);
    QJsonObject toJsonObject() const;
};

#endif // CONSTRAINT_H
