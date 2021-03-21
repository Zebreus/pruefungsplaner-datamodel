#ifndef TESTDATA_TEST_CPP
#define TESTDATA_TEST_CPP

#include "plan.h"
#include "testdatahelper.h"
#include <QFile>
#include <QJsonDocument>
#include <QSharedPointer>
#include <QString>
#include <QTemporaryDir>
#include <QUuid>
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

using namespace testing;

TEST(testDataTests, getValidPlanWorks) {
  QSharedPointer<Plan> plan = getValidPlan();
  ASSERT_NE(plan, nullptr);
}

TEST(testDataTests, getInvalidPlanWorks) {
  QSharedPointer<Plan> plan = getInvalidPlan();
  ASSERT_NE(plan, nullptr);
}

TEST(testDataTests, getValidJsonPlanWorks) {
  QJsonValue plan = getValidJsonPlan();
  ASSERT_TRUE(plan.isObject());
}

TEST(testDataTests, getInvalidJsonPlanWorks) {
  QJsonValue plan = getInvalidJsonPlan();
  ASSERT_TRUE(plan.isObject());
}

TEST(testDataTests, validPlanSeemsToLoadCorrectly) {
  QJsonValue jsonPlan = getValidJsonPlan();
  Plan plan;

  plan.fromJsonObject(jsonPlan.toObject());
  ASSERT_GT(plan.getConstraints().size(), 0);
  Group *group = plan.getConstraints()[0];
  ASSERT_EQ(group->getId(), QUuid::fromString(QString(
                                "{00000000-0000-0000-0000-000000000130}")));
  ASSERT_EQ(group->getName(), "Woche1");
}

TEST(testDataTests, invalidPlanSeemsToLoadCorrectly) {
  QJsonValue jsonPlan = getInvalidJsonPlan();
  Plan plan;

  plan.fromJsonObject(jsonPlan.toObject());
  ASSERT_GT(plan.getConstraints().size(), 0);
  Group *group = plan.getConstraints()[0];
  ASSERT_EQ(group->getId(), QUuid::fromString(QString(
                                "{00000000-0000-0000-0000-000000000130}")));
  ASSERT_EQ(group->getName(), "Woche1");
}

TEST(testDataTests, prepareScheduledDirectoryWorks) {
  QTemporaryDir directory;
  ASSERT_TRUE(prepareScheduledDirectory(directory.path()));
  ASSERT_TRUE(
      QFile(directory.path() + "/pruef-intervalle.csv").open(QFile::ReadOnly));
  ASSERT_TRUE(
      QFile(directory.path() + "/pruefungen.csv").open(QFile::ReadOnly));
  ASSERT_TRUE(
      QFile(directory.path() + "/zuege-pruef.csv").open(QFile::ReadOnly));
  ASSERT_TRUE(
      QFile(directory.path() + "/zuege-pruef-pref2.csv").open(QFile::ReadOnly));
  ASSERT_TRUE(QFile(directory.path() + "/SPA-ERGEBNIS-PP/SPA-planung-pruef.csv")
                  .open(QFile::ReadOnly));
  ASSERT_TRUE(QFile(directory.path() + "/SPA-ERGEBNIS-PP/SPA-zuege-pruef.csv")
                  .open(QFile::ReadOnly));
}

#endif
