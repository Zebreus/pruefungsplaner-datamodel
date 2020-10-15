#ifndef TESTDATA_HELPER_H
#define TESTDATA_HELPER_H

/**
 * This file contains functions to load testdata
 */

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QQueue>
#include <QSharedPointer>
#include <QString>
#include "plan.h"

/**
 *  @brief Load a schedulable Plan object
 *  @return A QSharedPointer to a Plan object
 *
 * If there are problems loading the file or the file is incorrect the
 * getValidPlanWorks test will fail
 */
inline QSharedPointer<Plan> getValidPlan() {
  QFile file(":/data/plan.json");
  EXPECT_TRUE(file.exists())
      << "Example plan json file (" << file.fileName().constData()
      << ") does not exist.";
  EXPECT_TRUE(file.open(QFile::ReadOnly | QFile::Text))
      << "Cannot open example plan json file (" << file.fileName().constData()
      << ").";
  QString jsonString = QTextStream(&file).readAll();
  QJsonDocument document = QJsonDocument::fromJson(jsonString.toUtf8());
  EXPECT_TRUE(document.isObject()) << "Json file does not contain an object.";
  QSharedPointer<Plan> plan(new Plan());
  plan->fromJsonObject(document.object());
  return plan;
}

/**
 *  @brief Load an unschedulable Plan object
 *  @return A QSharedPointer to a Plan object
 *
 * The returned Plan object can not be scheduled without conflicts
 * If there are problems loading the file or the file is incorrect the
 * getInvalidPlanWorks test will fail
 */
inline QSharedPointer<Plan> getInvalidPlan() {
  // TODO create invalid plan json
  QFile file(":/data/unschedulableplan.json");
  EXPECT_TRUE(file.exists())
      << "Unschedulable plan json file (" << file.fileName().constData()
      << ") does not exist.";
  EXPECT_TRUE(file.open(QFile::ReadOnly | QFile::Text))
      << "Cannot open unschedulable plan json file ("
      << file.fileName().constData() << ").";
  QString jsonString = QTextStream(&file).readAll();
  QJsonDocument document = QJsonDocument::fromJson(jsonString.toUtf8());
  EXPECT_TRUE(document.isObject())
      << "Unschedulable Plan json file does not contain an object.";
  QSharedPointer<Plan> plan(new Plan());
  plan->fromJsonObject(document.object());
  return plan;
}

/**
 *  @brief Load a schedulable plan as a QJsonObject
 *  @return A QJsonObject containing the plan
 *
 * If there are problems loading the file or the file is incorrect the
 * getValidJsonPlanWorks test will fail
 */
inline QJsonObject getValidJsonPlan() {
  QFile file(":/data/plan.json");
  EXPECT_TRUE(file.exists())
      << "Example plan json file (" << file.fileName().constData()
      << ") does not exist.";
  EXPECT_TRUE(file.open(QFile::ReadOnly | QFile::Text))
      << "Cannot open example plan json file (" << file.fileName().constData()
      << ").";
  QString jsonString = QTextStream(&file).readAll();
  QJsonDocument document = QJsonDocument::fromJson(jsonString.toUtf8());
  EXPECT_TRUE(document.isObject()) << "Json file does not contain an object.";
  return document.object();
}

/**
 *  @brief Load an unschedulable Plan as a QJsonObject
 *  @return A QJsonObject containing the plan
 *
 * The returned Plan can not be scheduled without conflicts
 * If there are problems loading the file or the file is incorrect the
 * getInvalidJsonPlanWorks test will fail
 */
inline QJsonObject getInvalidJsonPlan() {
  // TODO create invalid plan json
  QFile file(":/data/unschedulableplan.json");
  EXPECT_TRUE(file.exists())
      << "Unschedulable plan json file (" << file.fileName().constData()
      << ") does not exist.";
  EXPECT_TRUE(file.open(QFile::ReadOnly | QFile::Text))
      << "Cannot open unschedulable plan json file ("
      << file.fileName().constData() << ").";
  QString jsonString = QTextStream(&file).readAll();
  QJsonDocument document = QJsonDocument::fromJson(jsonString.toUtf8());
  EXPECT_TRUE(document.isObject())
      << "Unschedulable Plan json file does not contain an object.";
  return document.object();
}

/**
 *  @brief Load all the files that are expected after successful scheduling into
 * a directory
 *  @return A bool indicating success
 *
 * If there are problems loading the files the prepareScheduledDirectoryWorks
 * test will fail
 */
inline bool prepareScheduledDirectory(QString destination) {
  QString source = ":/data/scheduled/";
  QQueue<QString> paths;
  paths.enqueue(".");

  while (!paths.isEmpty()) {
    QString path = paths.dequeue();
    QDir dir(source + QDir::separator() + path);
    if (!QDir(source + QDir::separator() + path).exists()) {
      return false;
    }
    if (!QDir(destination + QDir::separator() + path).exists()) {
      return false;
    }

    foreach (QString directory,
             dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
      QDir destdir(destination + QDir::separator() + path + QDir::separator() +
                   directory);
      if (!destdir.mkpath(".")) {
        return false;
      }
      paths.enqueue(path + QDir::separator() + directory);
    }

    foreach (QString f, dir.entryList(QDir::Files)) {
      if (!QFile::copy(
              source + QDir::separator() + path + QDir::separator() + f,
              destination + QDir::separator() + path + QDir::separator() + f)) {
        return false;
      }
    }
  }

  return true;
}

#endif
