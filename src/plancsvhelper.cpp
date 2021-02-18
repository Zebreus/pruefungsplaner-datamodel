#include <plancsvhelper.h>

PlanCsvHelper::PlanCsvHelper(QString path) : basePath(path) {
  initializeFilePaths();
}

PlanCsvHelper::PlanCsvHelper() : temporaryDirectory(new QTemporaryDir()) {
  basePath = temporaryDirectory->path();
  initializeFilePaths();
}

Plan* PlanCsvHelper::readPlan(QObject* parent) {
  QScopedPointer<Plan> newPlan(new Plan(parent));
  // TODO add support for custom names
  newPlan->setName("new plan");

  // Add base
  QList<Week*> weeks;
  weeks.append(new Week(newPlan.get()));
  weeks.append(new Week(newPlan.get()));
  weeks.append(new Week(newPlan.get()));
  weeks[0]->setName("Woche 1");
  weeks[1]->setName("Woche 2");
  weeks[2]->setName("Woche 3");
  newPlan->setWeeks(weeks);
  for (Week* week : weeks) {
    QList<Day*> days;
    for (int x = 0; x < 6; x++) {
      days.append(new Day(week));
    }
    days[0]->setName("Montag");
    days[1]->setName("Dienstag");
    days[2]->setName("Mittwoch");
    days[3]->setName("Donnerstag");
    days[4]->setName("Freitag");
    days[5]->setName("Samstag");
    for (Day* day : days) {
      QList<Timeslot*> timeslots;
      for (int x = 0; x < 6; x++) {
        timeslots.append(new Timeslot(day));
      }
      timeslots[0]->setName("Block 1");
      timeslots[1]->setName("Block 2");
      timeslots[2]->setName("Block 3");
      timeslots[3]->setName("Block 4");
      timeslots[4]->setName("Block 5");
      timeslots[5]->setName("Block 6");
      day->setTimeslots(timeslots);
    }
    week->setDays(days);
  }

  if (!readExamsIntervalsFile(newPlan.get())) {
    return nullptr;
  }

  if (!readGroupsExamsFile(newPlan.get())) {
    return nullptr;
  }

  if (!readExamsFile(newPlan.get())) {
    return nullptr;
  }

  if (!readGroupsExamsPrefFile(newPlan.get())) {
    return nullptr;
  }

  return newPlan.take();
}

bool PlanCsvHelper::writePlan(Plan* plan) {
  if (plan == nullptr) {
    return false;
  }
  return writeExamsIntervalsFile(plan) && writeExamsFile(plan) &&
         writeGroupsExamsFile(plan) && writeGroupsExamsPrefFile(plan) &&
         writePlanningExamsResultFile(plan);
}

bool PlanCsvHelper::isWritten() {
  return examsIntervalsFile.exists() && examsFile.exists() &&
         groupsExamsFile.exists() && groupsExamsPrefFile.exists();
}

bool PlanCsvHelper::isScheduled() {
  return planningExamsResultFile.exists() && groupsExamsResultFile.exists();
}

QString PlanCsvHelper::getPath() {
  return basePath;
}

bool PlanCsvHelper::readSchedule(Plan* plan) {
  if (!planningExamsResultFile.open(QFile::ReadOnly)) {
    return false;
  }
  QTextStream fileStream(&planningExamsResultFile);

  // Check, that firstline is valid
  if (fileStream.readLine() !=
      QString("BelegNr;Zug;Modul;Import;Prüfungsform;Zuordnung;Tag;Block;")) {
    planningExamsResultFile.close();
    return false;
  }

  // The modules will only be added to the slots, if the file is correct. Until
  // then they will be stored here.
  QList<QPair<Module*, Timeslot*>> modulesToAdd;

  while (!fileStream.atEnd()) {
    QString line = fileStream.readLine();
    if (line == "") {
      planningExamsResultFile.close();
      return true;
    }
    QList<QString> words = line.split(";");
    if (words.size() != 9) {
      planningExamsResultFile.close();
      return false;
    }

    // Generate module number (BelegNr[,Zug])
    QString moduleNumber = words[0];
    if (words[1] != "") {
      moduleNumber.append(",").append(words[1]);
    }

    // Find matching module
    Module* matchingModule = nullptr;
    for (Module* module : plan->getModules()) {
      if (module->getNumber() == moduleNumber) {
        if (module->getName() == words[2]) {
          if (module->getExamType() == words[4]) {
            matchingModule = module;
            break;
          }
        }
      }
    }
    if (matchingModule == nullptr) {
      planningExamsResultFile.close();
      return false;
    }

    // Find matching timeslot
    Timeslot* matchingTimeslot;
    bool dayOk = false;
    int day = words[6].toInt(&dayOk) - 1;
    if (!dayOk) {
      planningExamsResultFile.close();
      return false;
    }
    bool slotOk = false;
    int slot = words[7].toInt(&slotOk) - 1;
    if (!slotOk) {
      planningExamsResultFile.close();
      return false;
    }

    if (day < 0 || day / 7 >= plan->getWeeks().size()) {
      planningExamsResultFile.close();
      return false;
    }
    Week* weekPointer = plan->getWeeks()[day / 7];

    if (day % 7 >= weekPointer->getDays().size()) {
      planningExamsResultFile.close();
      return false;
    }
    Day* dayPointer = weekPointer->getDays()[day % 7];

    if (slot < 0 || slot >= dayPointer->getTimeslots().size()) {
      planningExamsResultFile.close();
      return false;
    }
    matchingTimeslot = dayPointer->getTimeslots()[slot];

    modulesToAdd.append(
        QPair<Module*, Timeslot*>(matchingModule, matchingTimeslot));
  }

  // Finally add modules to timeslots
  for (auto moduleTimeslotPair : modulesToAdd) {
    for (Week* week : plan->getWeeks()) {
      for (Day* day : week->getDays()) {
        for (Timeslot* timeslot : day->getTimeslots()) {
          timeslot->removeModule(moduleTimeslotPair.first);
        }
      }
    }
    moduleTimeslotPair.second->addModule(moduleTimeslotPair.first);
  }

  planningExamsResultFile.close();
  return true;
}

void PlanCsvHelper::initializeFilePaths() {
  examsIntervalsFile.setFileName(basePath + "/pruef-intervalle.csv");
  examsFile.setFileName(basePath + "/pruefungen.csv");
  groupsExamsFile.setFileName(basePath + "/zuege-pruef.csv");
  groupsExamsPrefFile.setFileName(basePath + "/zuege-pruef-pref2.csv");
  planningExamsResultFile.setFileName(basePath +
                                      "/SPA-ERGEBNIS-PP/SPA-planung-pruef.csv");
  groupsExamsResultFile.setFileName(basePath +
                                    "/SPA-ERGEBNIS-PP/SPA-zuege-pruef.csv");
}

bool PlanCsvHelper::writeExamsIntervalsFile(Plan* plan) {
  if (!examsIntervalsFile.open(QFile::ReadWrite)) {
    return false;
  }
  QTextStream fileStream(&examsIntervalsFile);

  fileStream << "Block;";
  for (auto constraint : plan->getConstraints()) {
    fileStream << constraint->getName() << ";";
  }
  fileStream << "-ENDE-\n";

  fileStream << QString("Maximale Prü/Tag;");
  for (Group* constraint : plan->getConstraints()) {
    fileStream << constraint->getExamsPerDay() << ";";
  }
  fileStream << "\n";

  // TODO check that plan has the right amount of days
  for (int week = 0; week < 3; week++) {
    for (int day = 0; day < 6; day++) {
      for (int timeslot = 0; timeslot < 6; timeslot++) {
        fileStream << blockNames[week * 6 * 6 + day * 6 + timeslot] << ";";
        for (Group* constraint : plan->getConstraints()) {
          if (plan->getWeeks()[week]
                  ->getDays()[day]
                  ->getTimeslots()[timeslot]
                  ->containsActiveGroup(constraint)) {
            fileStream << "FREI;";
          } else {
            fileStream << "BLOCKIERT;";
          }
        }
        fileStream << "\n";
      }
    }
  }

  /*
//TODO check if correct blocknames are neccesary
for (Week* week : plan->weeks) {
  for (Day* day : week->getDays()) {
    for (Timeslot* timeslot : day->getTimeslots()) {
      fileStream << day->name().chopped(2) << week->name().chopped(1) << "_"
                 << timeslot->name() << ";";
      for (Group* constraint : plan->constraints) {
        if (timeslot->containsActiveGroup(constraint)) {
          fileStream << "FREI;";
        } else {
          fileStream << "BLOCKIERT;";
        }
      }
    }
  }
}
*/

  fileStream << "-ENDE-;";
  for (int i = 0; i < plan->getConstraints().size(); i++) {
    fileStream << ";";
  }

  examsIntervalsFile.close();
  return true;

  // TODO Check if the second part of the csv file is needed
}

bool PlanCsvHelper::writeExamsFile(Plan* plan) {
  if (!examsFile.open(QFile::ReadWrite)) {
    return false;
  }
  QTextStream fileStream(&examsFile);

  for (Module* module : plan->getModules()) {
    // Comment inactive modules
    if (module->getActive() == false) {
      fileStream << "//";
    }

    // TODO Check somewhere else
    if (module->getOrigin() == "EIT") {
      // SPA-algorithmus fails if there are EIT exams, because "Prüfungen von
      // EIT müssen fest abgesprochen sein!!"
      continue;
    }

    // Only one constraint is possible, because the legacy algorithm does not
    // support more
    if (module->getConstraints().size() >= 1) {
      fileStream << module->getConstraints()[0]->getName();
    }
    fileStream << ";";

    // TODO Check somewhere, that groupnames do not contain commas
    QString divider = "";
    for (Group* group : module->getGroups()) {
      fileStream << divider << group->getName();
      divider = ",";
    }
    fileStream << ";";

    fileStream << module->getName() << ";";
    fileStream << module->getNumber() << ";";
    fileStream << module->getOrigin() << ";";

    if (module->getExamType() == "K" || module->getExamType() == "P") {
      fileStream << module->getExamType() << ";";
    } else {
      examsFile.close();
      return false;
    }

    // The duration can also be omitted if it is 1, but we dont do that
    fileStream << module->getExamDuration() << ";";

    fileStream << "";

    fileStream << "\n";
  }
  fileStream << "-ENDE-;;;;;;";

  examsFile.close();
  return true;
}

bool PlanCsvHelper::writeGroupsExamsFile(Plan* plan) {
  if (!groupsExamsFile.open(QFile::ReadWrite)) {
    return false;
  }
  QTextStream fileStream(&groupsExamsFile);

  fileStream << "Block;";
  for (Group* group : plan->getGroups()) {
    fileStream << group->getName() << ";";
  }
  fileStream << "-ENDE-\n";

  fileStream << QString("Maximale Prü/Tag;");
  for (Group* group : plan->getGroups()) {
    fileStream << group->getExamsPerDay() << ";";
  }
  fileStream << "\n";

  // TODO check that plan has the right amount of days
  for (int week = 0; week < 3; week++) {
    for (int day = 0; day < 6; day++) {
      for (int timeslot = 0; timeslot < 6; timeslot++) {
        fileStream << blockNames[week * 6 * 6 + day * 6 + timeslot] << ";";
        for (Group* group : plan->getGroups()) {
          if (plan->getWeeks()[week]
                  ->getDays()[day]
                  ->getTimeslots()[timeslot]
                  ->containsActiveGroup(group)) {
            fileStream << "FREI;";
          } else {
            fileStream << "BLOCKIERT;";
          }
        }
        fileStream << "\n";
      }
    }
  }

  /*
//TODO check if correct blocknames are neccesary
for (Week* week : plan->weeks) {
  for (Day* day : week->getDays()) {
    for (Timeslot* timeslot : day->getTimeslots()) {
      fileStream << day->name().chopped(2) << week->name().chopped(1) << "_"
                 << timeslot->name() << ";";
      for (Group* group : plan->groups) {
        if (timeslot->containsActiveGroup(group)) {
          fileStream << "FREI;";
        } else {
          fileStream << "BLOCKIERT;";
        }
      }
    }
  }
}
*/

  fileStream << "-ENDE-;";
  for (int i = 0; i < plan->getGroups().size(); i++) {
    fileStream << ";";
  }

  groupsExamsFile.close();
  return true;
}

bool PlanCsvHelper::writeGroupsExamsPrefFile(Plan* plan) {
  if (!groupsExamsPrefFile.open(QFile::ReadWrite)) {
    return false;
  }

  QTextStream fileStream(&groupsExamsPrefFile);

  for (Group* group : plan->getGroups()) {
    if (!group->getActive()) {
      fileStream << group->getName() << "\n";
    }
  }
  fileStream << "-ENDE-\n";

  for (Group* group : plan->getGroups()) {
    if (group->getSmall()) {
      fileStream << group->getName() << "\n";
    }
  }
  fileStream << "-ENDE-\n";

  for (Group* group : plan->getGroups()) {
    if (group->getObsolete()) {
      fileStream << group->getName() << "\n";
    }
  }
  fileStream << "-ENDE-\n";

  groupsExamsPrefFile.close();
  return true;
}

bool PlanCsvHelper::writePlanningExamsResultFile(Plan* plan) {
  if (!QDir(basePath).exists()) {
    return false;
  }
  QDir().mkpath(basePath + "/SPA-ERGEBNIS-PP");
  if (!planningExamsResultFile.open(QFile::ReadWrite)) {
    return false;
  }
  QTextStream fileStream(&planningExamsResultFile);
  fileStream << "BelegNr;Zug;Modul;Import;Prüfungsform;Zuordnung;Tag;Block;\n";
  int weekId = -1;
  for (Week* week : plan->getWeeks()) {
    weekId++;
    int dayId = -1;
    for (Day* day : week->getDays()) {
      dayId++;
      int timeslotId = -1;
      for (Timeslot* timeslot : day->getTimeslots()) {
        timeslotId++;
        for (Module* module : timeslot->getModules()) {
          // TODO find a better name for this list
          QList<QString> moduleNumberParts =
              module->getNumber().split(',', Qt::SkipEmptyParts);
          if (moduleNumberParts.size() >= 1) {
            fileStream << moduleNumberParts[0];
          }
          fileStream << ";";
          if (moduleNumberParts.size() >= 2) {
            fileStream << moduleNumberParts[1];
          }
          fileStream << ";";
          fileStream << module->getName() << ";";
          if (module->getOrigin() == "") {
            fileStream << "0";
          } else {
            fileStream << "1";
          }
          fileStream << ";";
          if (module->getExamType() == "K" || module->getExamType() == "P") {
            fileStream << module->getExamType() << ";";
          } else {
            planningExamsResultFile.close();
            return false;
          }
          for (Group* group : module->getGroups()) {
            fileStream << group->getName() << "/";
          }
          // TODO find out what these words mean
          fileStream << "ALLE ("
                     << blockNames[weekId * 6 * 6 + dayId * 6 + timeslotId]
                     << ");";

          fileStream << weekId * 7 + dayId + 1 << ";" << timeslotId + 1
                     << ";\n";
        }
      }
    }
    planningExamsResultFile.close();
    return true;
  }

  for (Module* module : plan->getModules()) {
    // TODO Check somewhere else
    if (module->getOrigin() == "EIT") {
      // SPA-algorithmus fails if there are EIT exams, because "Prüfungen von
      // EIT müssen fest abgesprochen sein!!"
      continue;
    }

    // Only one constraint is possible, because the legacy algorithm does not
    // support more
    if (module->getConstraints().size() >= 1) {
      fileStream << module->getConstraints()[0]->getName();
    }
    fileStream << ";";

    // TODO Check somewhere, that groupnames do not contain commas
    QString divider = "";
    for (Group* group : module->getGroups()) {
      fileStream << divider << group->getName();
      divider = ",";
    }
    fileStream << ";";

    fileStream << module->getName() << ";";
    fileStream << module->getNumber() << ";";
    fileStream << module->getOrigin() << ";";
    // TODO find out what K or P means and add to datamodel
    fileStream << "K"
               << ";";
    // TODO add duration to datamodel and add it here
    fileStream << "";

    fileStream << "\n";
  }
  fileStream << "-ENDE-;;;;;;";

  examsFile.close();
  return true;
}

bool PlanCsvHelper::readExamsIntervalsFile(Plan* plan) {
  if (!examsIntervalsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return false;
  }

  QTextStream fileStream(&examsIntervalsFile);

  // Read and check first two lines
  int wordsPerLine;
  QList<QString> firstLine = fileStream.readLine().split(";");
  if (firstLine.size() < 2 || firstLine[0] != "Block" ||
      firstLine.last() != "-ENDE-") {
    while (firstLine.size() > 0 && firstLine.last() != "-ENDE-") {
      firstLine.takeLast();
    }
    if (firstLine.size() == 0) {
      examsIntervalsFile.close();
      return false;
    }
  }
  wordsPerLine = firstLine.size();

  QList<QString> secondLine = fileStream.readLine().split(";");
  if (secondLine[0] != QString("Maximale Prü/Tag") || secondLine.last() != "") {
    examsIntervalsFile.close();
    return false;
  }

  QList<Group*> constraints = plan->getConstraints();
  for (int i = 1; i < wordsPerLine - 1; i++) {
    Group* group = new Group(plan);
    group->setName(firstLine[i]);
    bool parseIntWorked;
    unsigned int examsPerDay = secondLine[i].toUInt(&parseIntWorked);
    if (parseIntWorked) {
      group->setExamsPerDay(examsPerDay);
    } else if (secondLine[i] == "") {
      // If the field is empty there are unlimited exams per day allowed
      group->setExamsPerDay(99);
    } else {
      examsIntervalsFile.close();
      return false;
    }
    constraints.append(group);
  }

  // Read a line for each timeslot
  for (Week* week : plan->getWeeks()) {
    for (Day* day : week->getDays()) {
      for (Timeslot* timeslot : day->getTimeslots()) {
        QList<QString> words = fileStream.readLine().split(";");
        if (words.size() != wordsPerLine) {
          examsIntervalsFile.close();
          return false;
        }
        for (int i = 0; i < constraints.size(); i++) {
          if (words[i + 1] == "FREI") {
            timeslot->addActiveGroup(constraints[i]);
          } else if (words[i + 1] != "BLOCKIERT") {
            examsIntervalsFile.close();
            return false;
          }
        }
      }
    }
  }

  plan->setConstraints(constraints);

  examsIntervalsFile.close();
  return true;
}

bool PlanCsvHelper::readExamsFile(Plan* plan,
                                  bool parseComments,
                                  bool addMissingGroups) {
  if (!examsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return false;
  }

  QTextStream fileStream(&examsFile);

  QList<QString> words = fileStream.readLine().split(";");
  QList<Module*> modules = plan->getModules();

  // If the first line is the generated header, skip it
  if (words.size() >= 3 && (words[1] == "Kategorie" || words[2] == "Modul")) {
    words = fileStream.readLine().split(";");
  }

  while (!words.isEmpty() && words.first() != "-ENDE-") {
    // If the line is a commented, but still a valid line it is loaded as an
    // inactive module
    bool comment = false;
    if (words.first().startsWith("//")) {
      if (parseComments) {
        comment = true;
        words[0] = words[0].right(words[0].size() - 2).trimmed();
      } else {
        words = fileStream.readLine().split(";");
        continue;
      }
    }

    if (words.size() != 7 && words.size() != 8) {
      if (comment) {
        words = fileStream.readLine().split(";");
        continue;
      } else {
        examsFile.close();
        return false;
      }
    }
    Module* module = new Module(plan);

    if (comment) {
      module->setActive(false);
    }

    module->setName(words[2]);
    module->setOrigin(words[4]);
    module->setNumber(words[3]);
    bool foundAllGroups = true;
    for (QString groupName : words[1].split(",")) {
      bool foundGroup = false;
      for (Group* group : plan->getGroups()) {
        if (group->getName() == groupName) {
          auto groups = module->getGroups();
          groups.append(group);
          module->setGroups(groups);
          foundGroup = true;
          break;
        }
      }
      if (!foundGroup) {
        // TODO Surprising behaviour, when a line is commented and a group is
        // added, but that line gets discarded later on
        if (addMissingGroups) {
          qDebug() << "Adding missing group " << groupName;
          Group* newGroup = new Group(plan);
          newGroup->setName(groupName);
          auto planGroups = plan->getGroups();
          planGroups.append(newGroup);
          plan->setGroups(planGroups);
          auto moduleGroups = module->getGroups();
          moduleGroups.append(newGroup);
          module->setGroups(moduleGroups);
        } else {
          foundAllGroups = false;
          break;
        }
      }
    }
    if (!foundAllGroups) {
      if (comment) {
        words = fileStream.readLine().split(";");
        continue;
      } else {
        examsFile.close();
        return false;
      }
    }

    bool foundConstraint = false;
    if (words[0] != "") {
      for (Group* constraint : plan->getConstraints()) {
        if (constraint->getName() == words[0]) {
          auto constraints = module->getConstraints();
          constraints.append(constraint);
          module->setGroups(constraints);
          foundConstraint = true;
          break;
        }
      }
      if (!foundConstraint) {
        if (addMissingGroups) {
          qDebug() << "Adding missing constraint " << words[0];
          Group* newConstraint = new Group(plan);
          newConstraint->setName(words[0]);
          auto planConstraints = plan->getConstraints();
          planConstraints.append(newConstraint);
          plan->setConstraints(planConstraints);
          auto moduleConstraints = module->getConstraints();
          moduleConstraints.append(newConstraint);
          module->setConstraints(moduleConstraints);
        } else {
          if (comment) {
            words = fileStream.readLine().split(";");
            continue;
          } else {
            examsFile.close();
            return false;
          }
        }
      }
    }

    if (words[5] == "P" || words[5] == "K" || words[5] == "-") {
      module->setExamType(words[5]);
    } else {
      if (comment) {
        words = fileStream.readLine().split(";");
        continue;
      } else {
        examsFile.close();
        return false;
      }
    }

    if (words[6] != "") {
      bool ok;
      unsigned int examDuration = words[6].toUInt(&ok);

      if (!ok) {
        if (comment) {
          words = fileStream.readLine().split(";");
          continue;
        } else {
          examsFile.close();
          return false;
        }
      }
      if (examDuration != 1 && examDuration != 2) {
        qDebug()
            << "Mysterious bug appeared, where the exam duration is not 1 or 2";
      }
      module->setExamDuration(examDuration);
    } else {
      // No value for examDuration implicitly means 1
      module->setExamDuration(1);
    }

    modules.append(module);

    words = fileStream.readLine().split(";");
  }

  plan->setModules(modules);

  return true;
}

bool PlanCsvHelper::readGroupsExamsFile(Plan* plan) {
  if (!groupsExamsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return false;
  }

  QTextStream fileStream(&groupsExamsFile);

  // Read and check first two lines
  int wordsPerLine;
  QList<QString> firstLine = fileStream.readLine().split(";");
  if (firstLine.size() < 2 || firstLine[0] != "Block" ||
      firstLine.last() != "-ENDE-") {
    while (firstLine.size() > 0 && firstLine.last() != "-ENDE-") {
      firstLine.takeLast();
    }
    if (firstLine.size() == 0) {
      examsIntervalsFile.close();
      return false;
    }
  }
  wordsPerLine = firstLine.size();

  QList<QString> secondLine = fileStream.readLine().split(";");
  if (secondLine[0] != QString("Maximale Prü/Tag") || secondLine.last() != "") {
    groupsExamsFile.close();
    return false;
  }

  QList<Group*> groups = plan->getGroups();
  for (int i = 1; i < wordsPerLine - 1; i++) {
    Group* group = new Group(plan);
    group->setName(firstLine[i]);
    bool parseIntWorked;
    unsigned int examsPerDay = secondLine[i].toUInt(&parseIntWorked);
    if (parseIntWorked) {
      group->setExamsPerDay(examsPerDay);
    } else if (secondLine[i] == "") {
      // If the field is empty there are unlimited exams per day allowed
      group->setExamsPerDay(99);
    } else {
      groupsExamsFile.close();
      return false;
    }
    groups.append(group);
  }

  // Read a line for each timeslot
  for (Week* week : plan->getWeeks()) {
    for (Day* day : week->getDays()) {
      for (Timeslot* timeslot : day->getTimeslots()) {
        QList<QString> words = fileStream.readLine().split(";");
        if (words.size() != wordsPerLine) {
          groupsExamsFile.close();
          return false;
        }
        for (int i = 0; i < groups.size(); i++) {
          if (words[i + 1] == "FREI") {
            timeslot->addActiveGroup(groups[i]);
          } else if (words[i + 1] != "BLOCKIERT") {
            groupsExamsFile.close();
            return false;
          }
        }
      }
    }
  }

  plan->setGroups(groups);

  groupsExamsFile.close();
  return true;
}

bool PlanCsvHelper::readGroupsExamsPrefFile(Plan* plan, bool addMissingGroups) {
  if (!groupsExamsPrefFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return false;
  }

  QTextStream fileStream(&groupsExamsPrefFile);

  for (Group* group : plan->getGroups()) {
    group->setActive(true);
    group->setSmall(false);
    group->setObsolete(false);
  }

  QList<QString> line = fileStream.readLine().split(";");
  while (line.size() >= 1 && line[0] != "-ENDE-") {
    if (line[0].startsWith("//")) {
      line = fileStream.readLine().split(";");
      continue;
    }
    bool found = false;
    for (Group* group : plan->getGroups()) {
      if (group->getName() == line[0]) {
        group->setActive(false);
        found = true;
        break;
      }
    }
    if (!found) {
      if (addMissingGroups && line[0] != "") {
        qDebug() << "Adding missing group " << line[0];
        Group* newGroup = new Group(plan);
        newGroup->setName(line[0]);
        newGroup->setActive(false);
        auto planGroups = plan->getGroups();
        planGroups.append(newGroup);
        plan->setGroups(planGroups);
      } else {
        groupsExamsPrefFile.close();
        return false;
      }
    }
    line = fileStream.readLine().split(";");
  }

  line = fileStream.readLine().split(";");
  while (line.size() >= 1 && line[0] != "-ENDE-") {
    if (line[0].startsWith("//")) {
      line = fileStream.readLine().split(";");
      continue;
    }
    bool found = false;
    for (Group* group : plan->getGroups()) {
      if (group->getName() == line[0]) {
        group->setSmall(true);
        found = true;
        break;
      }
    }
    if (!found) {
      if (addMissingGroups && line[0] != "") {
        qDebug() << "Adding missing group " << line[0];
        Group* newGroup = new Group(plan);
        newGroup->setName(line[0]);
        newGroup->setSmall(true);
        auto planGroups = plan->getGroups();
        planGroups.append(newGroup);
        plan->setGroups(planGroups);
      } else {
        groupsExamsPrefFile.close();
        return false;
      }
    }
    line = fileStream.readLine().split(";");
  }

  line = fileStream.readLine().split(";");
  while (line.size() >= 1 && line[0] != "-ENDE-") {
    if (line[0].startsWith("//")) {
      line = fileStream.readLine().split(";");
      continue;
    }
    bool found = false;
    for (Group* group : plan->getGroups()) {
      if (group->getName() == line[0]) {
        group->setObsolete(true);
        found = true;
        break;
      }
    }
    if (!found) {
      if (addMissingGroups && line[0] != "") {
        qDebug() << "Adding missing group " << line[0];
        Group* newGroup = new Group(plan);
        newGroup->setName(line[0]);
        newGroup->setObsolete(true);
        auto planGroups = plan->getGroups();
        planGroups.append(newGroup);
        plan->setGroups(planGroups);
      } else {
        groupsExamsPrefFile.close();
        return false;
      }
    }
    line = fileStream.readLine().split(";");
  }

  // The contents of the groupsExamsPrefFile are not relevant for plan
  groupsExamsPrefFile.close();
  return true;
}
