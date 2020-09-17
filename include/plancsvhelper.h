#ifndef PLANCSVHELPER_H
#define PLANCSVHELPER_H

#include <plan.h>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QString>
#include <QTemporaryDir>

/**
 *  @class PlanCsvHelper
 *  @brief A helper for converting plans to csv files
 *
 *  A helper for converting plans to the csv files required by sp-automatisch
 * and for converting the resulting files from sp-automatisch to a Plan object.
 */
class PlanCsvHelper {
 private:
  QString basePath;
  QScopedPointer<QTemporaryDir> temporaryDirectory;

 public:
  /**
   *  @brief Creates a PlanCsvHelper for the given path
   *  @param path is the directory the csv files are located
   */
  PlanCsvHelper(QString path);

  /**
   *  @brief Creates a PlanCsvHelper
   *
   *  Creates a PlanCsvHelper with a temporary directory for the files. The
   * directory will be removed, once this PlanCsvHelper gets destroyed.
   */
  PlanCsvHelper();

  /**
   *  @brief Create a plan from the files in path
   *  @return The plan generated from the files in path
   */
  QSharedPointer<Plan> readPlan();

  /**
   *  @brief Write plan to csv files
   *  @param [in] plan is the plan, that will be written
   *  @return True if the plan was written successfully
   *
   *  Writes the plan to csv files.
   *  Any scheduling information in plan will not be written.
   *  If csv files already exist in the path, everything there will be deleted
   * and new csv files will be written.
   */
  bool writePlan(QSharedPointer<Plan> plan);

  /**
   *  @brief Check if the files required by sp-automatisch exist
   *  @return True, if the files required by sp-automatisch exist
   *
   *  These files are required:
   *   - pruef-intervalle.csv
   *   - pruefungen.csv
   *   - zuege-pruef.csv
   *   - zuege-pruef-pref2.csv
   */
  bool isWritten();

  /**
   *  @brief Check if the plan is scheduled
   *  @return True, if the files that are generated by sp-automatisch exist
   *
   *  These files are checked:
   *   - SPA-ERGEBNIS-PP/SPA-planung-pruef.csv
   *   - SPA-ERGEBNIS-PP/SPA-zuege-pruef.cs
   */
  bool isScheduled();

  /**
   *  @brief Get the path of the directory containing the csv files
   *  @return A QString containing the path
   */
  QString getPath();
};

#endif  // PLANCSVHELPER_H
