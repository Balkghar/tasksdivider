#include <iostream>
#include <string>

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
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

  string namesInput;

  cout << "Enter a string of names separated by commas: ";
  getline(cin, namesInput);
  namesInput.erase(
      std::remove_if(namesInput.begin(), namesInput.end(), ::isspace),
      namesInput.end());
  vector<string> names = split(namesInput, ',');

  string tasksInput;

  cout << "Enter a string of tasks separated by commas: ";
  getline(cin, tasksInput);
  tasksInput.erase(
      std::remove_if(tasksInput.begin(), tasksInput.end(), ::isspace),
      tasksInput.end());
  vector<string> tasks = split(tasksInput, ',');

  int numberOfWeeks;

  cout << "Enter a numberOfWeeks: ";
  cin >> numberOfWeeks;

  int startingWeek;
  cout << "Enter the starting weeks: ";
  ;
  cin >> startingWeek;

  unordered_map<string, int> numberOfTaskDone;

  for (const auto &name : names) {
    numberOfTaskDone[name] = 0;
  }

  vector<vector<string>> tasksPerPerson(numberOfWeeks,
                                        vector<string>(tasks.size()));

  unordered_map<string, unordered_map<string, int>> taskNumber;

  int maxdiff = 2;
  while (maxdiff >= 2) {

    for (int i = 0; i < names.size(); i++) {
      for (int j = 0; j < tasks.size(); j++) {
        taskNumber[names[i]][tasks[j]] = 0;
      }
    }
    for (int i = 0; i < numberOfWeeks; i++) {
      vector<string> namesCopy = names;

      for (int j = 0; j < tasks.size(); j++) {

        if (namesCopy.empty()) {
          namesCopy = names;
        }

        srand(time(0));
        int randomIndex = rand() % namesCopy.size();
        if (i != 0) {

          for (int k = 0; k < namesCopy.size(); k++) {
            if (numberOfTaskDone[namesCopy[randomIndex]] >
                    numberOfTaskDone[namesCopy[k]] ||
                taskNumber[namesCopy[randomIndex]][tasks[j]] >
                    taskNumber[namesCopy[k]][tasks[j]]) {
              randomIndex = k;
            }
          }
        }

        tasksPerPerson[i][j] = namesCopy[randomIndex];
        numberOfTaskDone[namesCopy[randomIndex]]++;
        taskNumber[namesCopy[randomIndex]][tasks[j]]++;

        namesCopy.erase(namesCopy.begin() + randomIndex);
      }
    }
    maxdiff = 0;
    for (int i = 0; i < names.size(); i++) {

      int max = 0;
      int min = 0;
      for (int j = 0; j < tasks.size(); j++) {
        if (taskNumber[names[i]][tasks[j]] > taskNumber[names[i]][tasks[max]]) {
          max = j;
        }
        if (taskNumber[names[i]][tasks[j]] < taskNumber[names[i]][tasks[min]]) {
          min = j;
        }
      }
      if (maxdiff <
          taskNumber[names[i]][tasks[max]] - taskNumber[names[i]][tasks[min]]) {
        maxdiff =
            taskNumber[names[i]][tasks[max]] - taskNumber[names[i]][tasks[min]];
      }
    }
  }
  // Export tasksPerPerson to a CSV file
  ofstream outputFile("./tasks.csv");
  if (outputFile.is_open()) {
    // Write header
    outputFile << "Weeks,";
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
