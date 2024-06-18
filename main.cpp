#include <iostream>
#include <string>

#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

vector<string> split(const string &s, char delim) {
  vector<string> result;
  stringstream ss(s);
  string item;
  while (getline(ss, item, delim)) {
    result.push_back(item);
  }
  return result;
}

int main(int argc, char *argv[]) {
  string namesInput = "Hugo,Émile,Greg,Killian";
  /*
  cout << "Enter a string of names separated by commas: ";
  getline(cin, namesInput);
  */
  vector<string> names = split(namesInput, ',');

  string tasksInput =
      "lavabo,arrosage,aspirateur,panosse,surface,poubelle,tiroirs,Lessive";
  /*
  cout << "Enter a string of tasks separated by commas: ";
  getline(cin, tasksInput);
  */
  vector<string> tasks = split(tasksInput, ',');

  int numberOfWeeks = 16; /*
   cout << "Enter a numberOfWeeks between 1 and 52: ";
   cin >> numberOfWeeks;
 */
  while (numberOfWeeks < 1 || numberOfWeeks > 52) {
    cout << "Invalid number of weeks. Please enter a number of weeks between 1 "
            "and "
            "52: ";
    cin >> numberOfWeeks;
  }

  int startingWeek = 4;

  unordered_map<string, int> numberOfTaskDone;

  for (const auto &name : names) {
    numberOfTaskDone[name] = 0;
  }

  vector<vector<string>> tasksPerPerson(numberOfWeeks,
                                        vector<string>(tasks.size()));

  unordered_map<string, unordered_map<string, int>> taskNumber;

  for (int i = 0; i < names.size(); i++) {
    for (int j = 0; j < tasks.size(); j++) {
      taskNumber[names[i]][tasks[j]] = 0;
    }
  }

  for (int i = 0; i < numberOfWeeks; i++) {
    vector<string> namesCopy = names;

    for (int j = 0; j < tasks.size(); j++) {

      int randomIndex = rand() % namesCopy.size();
      for (int k = 0; k < namesCopy.size(); k++) {
        if (numberOfTaskDone[namesCopy[randomIndex]] >
                numberOfTaskDone[namesCopy[k]] ||
            taskNumber[namesCopy[randomIndex]][tasks[j]] >
                taskNumber[namesCopy[k]][tasks[j]]) {
          randomIndex = k;
        }
      }

      tasksPerPerson[i][j] = namesCopy[randomIndex];
      numberOfTaskDone[namesCopy[randomIndex]]++;
      taskNumber[namesCopy[randomIndex]][tasks[j]]++;

      namesCopy.erase(namesCopy.begin() + randomIndex);

      if (namesCopy.size() < tasks.size() - j) {
        namesCopy = names;
      }
    }
  }

  // Export tasksPerPerson to a CSV file
  ofstream outputFile("./tasks.csv");
  if (outputFile.is_open()) {
    // Write header
    outputFile << "Week,";
    for (const auto &task : tasks) {
      outputFile << task << ",";
    }
    outputFile << endl;

    // Write data
    for (int i = 0; i < numberOfWeeks; i++) {
      outputFile << i + startingWeek << ",";
      for (const auto &task : tasksPerPerson[i]) {
        outputFile << task << ",";
      }
      outputFile << endl;
    }

    outputFile.close();
    cout << "Tasks exported to tasks.csv" << endl;
  } else {
    cout << "Failed to open tasks.csv for writing" << endl;
  }

  for (int i = 0; i < names.size(); i++) {
    cout << names[i] << " has " << numberOfTaskDone[names[i]] << " tasks"
         << endl;
  }
  for (const auto &name : names) {
    for (const auto &task : tasks) {
      cout << name << " has done " << task << " " << taskNumber[name][task]
           << endl;
    }
  }

  return 0;
}
