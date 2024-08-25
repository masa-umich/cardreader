#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

struct Student {
    string name;
    string email;
    string umid;
    string cardID;
};

string getCurrentTime() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    std::ostringstream oss;
    oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

Student parseStudent(string input) {
    // Example input: %B6008475938410415^HAMMERBERG/J^2806120JACKMH?;6008475938410415=2806120=816877446?
    // Name: J HAMMERBERG
    // Email: jackmh@umich.edu
    // UMID: 59384104
    // Card ID: 6008475938410415
    Student student;
    size_t start, end;
    // Get the card ID
    start = input.find("%") + 2;
    end = input.find("^");
    student.cardID = input.substr(start, end - start);
    // Get the last name
    start = input.find("^") + 1;
    end = input.find("/");
    string lastName = input.substr(start, end - start);
    // Get the first name
    start = end + 1;
    end = input.find("^", start);
    // Concatenate the first and last name to get the full name
    student.name = input.substr(start, end - start) + " " + lastName;
    // Get the UMID
    start = input.find("^") - 10;
    end = input.find("^") - 2;
    student.umid = input.substr(start, end - start);
    // Get the email
    start = input.find("^", input.find("/")) + 8; // Find the next ^ after the the / character
    end = input.find("?;");
    student.email = input.substr(start, end - start) + "@umich.edu";
    return student;
}

int main() {
    string header = "Time,Name,Email,UMID,Card ID";
    string filename = (fs::current_path() / "Festifall Cardreader.csv").string();
    ifstream infile(filename);
    bool headerMatches = false;

    if (infile.is_open()) {
        string firstLine;
        getline(infile, firstLine);
        if (firstLine == header) {
            headerMatches = true;
        }
        infile.close();
    }

    ofstream file;
    if (headerMatches) {
        file.open(filename, ios::app);
        cout << "Existing file found" << endl;
        file.close();
    } else {
        file.open(filename);
        file << header << endl;
        cout << "New file created" << endl;
        file.close();
    }

    while (true) {
        string input;
        cout << "Type 'exit' to stop" << endl;
        cout << "Enter student info: ";
        cin >> input;
        if (input == "exit" || input == "Exit" || input == "EXIT") {
            break;
        }
        Student student = parseStudent(input);
        string time = getCurrentTime();
        file.open(filename, ios::app);
        if (!file.is_open()) {
            cout << "Error opening file" << endl;
            return 1;
        }
        file << time << "," << student.name << "," << student.email << "," << student.umid << "," << student.cardID << endl;
        cout << time << ", " << student.name << ", " << student.email << ", " << student.umid << ", " << student.cardID << endl;
        file.close();
    }
    return 0;
}