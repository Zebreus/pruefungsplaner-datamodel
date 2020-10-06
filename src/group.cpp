#include <group.h>

Group::Group(QObject *parent) : SerializableDataObject(parent)
{

}

QString Group::name()
{
    return constraintName;
}

void Group::setName(const QString &name)
{
    if (name == constraintName)
        return;

    constraintName = name;
    emit nameChanged();
}

bool Group::getSelected()
{
    return selected;
}

void Group::setSelected(const bool selected)
{
    if (selected == this->selected)
        return;

    this->selected = selected;
    emit selectedChanged();
}

unsigned int Group::getExamsPerDay()
{
    return examsPerDay;
}

void Group::setExamsPerDay(unsigned int examsPerDay)
{
    if (examsPerDay == this->examsPerDay)
        return;

    this->examsPerDay = examsPerDay;
    emit examsPerDayChanged();
}

void Group::fromJsonObject(const QJsonObject &content)
{
    simpleValuesFromJsonObject(content);
}

QJsonObject Group::toJsonObject() const
{
    return recursiveToJsonObject();
}
