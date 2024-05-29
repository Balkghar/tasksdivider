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
  string namesInput;
  cout << "Enter a string of names separated by commas: ";
  getline(cin, namesInput);

  vector<string> names = split(namesInput, ',');

  string tasksInput;
  cout << "Enter a string of names separated by commas: ";
  getline(cin, tasksInput);

  vector<string> tasks = split(tasksInput, ',');

  int numberOfWeeks;
  cout << "Enter a numberOfWeeks between 1 and 52: ";
  cin >> numberOfWeeks;

  while (numberOfWeeks < 1 || numberOfWeeks > 52) {
    cout << "Invalid numberOfWeeks. Please enter a numberOfWeeks between 1 and "
            "52: ";
    cin >> numberOfWeeks;
  }

  unordered_map<string, int> hashMap;

  for (const auto &name : names) {
    hashMap[name] = 0;
  }

  vector<vector<string>> tasksPerPerson(numberOfWeeks,
                                        vector<string>(tasks.size()));

  for (int i = 0; i < numberOfWeeks; i++) {
    vector<string> namesCopy = names;

    for (int j = 0; j < tasks.size(); j++) {
      int randomIndex = rand() % namesCopy.size();
      tasksPerPerson[i][j] = namesCopy[randomIndex];
      hashMap[namesCopy[randomIndex]]++;
      namesCopy.erase(namesCopy.begin() + randomIndex);
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
      outputFile << i + 1 << ",";
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

  return 0;
}
