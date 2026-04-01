#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <sstream>
#include <bits/stdc++.h>

#ifdef _WIN32
#include <windows.h>  // For Windows-specific clearConsole function
void clearConsole() {
    system("cls");
}
#else
void clearConsole() {
    system("clear");
}
#endif

using namespace std;

// Define constants
const int LUNCH_FIXED_COST = 20;
const int DINNER_FIXED_COST = 20;

// Function prototypes
void displayMenu(const map<string, int>& menu, const string& mealType);
void selectItems(const map<string, int>& menu, vector<string>& selectedItems);
bool isValidMenuItem(int itemNumber, int menuSize);

class User {
public:
    string username;
    string password;

    User(const string& uname, const string& pwd) : username(uname), password(pwd) {}
};

class Authentication {
private:
    unordered_map<string, User> users;

public:
    // Load user data from a file
    void loadUsersFromFile(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                istringstream iss(line);
                string username, password;
                iss >> username >> password;
                users.emplace(username, User(username, password));
            }
            file.close();
        }
    }

    // Save user data to a file
    void saveUsersToFile(const string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& entry : users) {
                file << entry.second.username << " " << entry.second.password << "\n";
            }
            file.close();
        }
    }

    // Add a new user to the system with a unique username
    void signUp() {
        while (true) {
            string username, password;
            cout << "Enter a unique username (type 'back' to go back): ";
            cin >> username;

            if (username == "back") {
                return;  // Go back to the main menu
            }

            // Check if the username is already taken
            if (users.find(username) != users.end()) {
                cout << "Username is not unique. Please choose a different username.\n";
            } else {
                // If the username is unique, proceed with the signup process
                cout << "Enter password: ";
                cin >> password;
                users.emplace(username, User(username, password));
                cout << "User signed up successfully!\n";

                // Save users to file after each signup
                saveUsersToFile("users.txt");
                return;
            }
        }
    }

    // Validate user credentials
    bool login(const string& username, const string& password) {
        auto it = users.find(username);
        if (it != users.end() && it->second.password == password) {
            cout << "Login successful!\n";
            return true;
        }
        cout << "Invalid credentials. Login failed.\n";
        return false;
    }
};

class MealBookingSystem {
private:
    string filename;
    map<string, int> lunchMenu;
    map<string, int> dinnerMenu;

public:
    MealBookingSystem(const string& filename, const map<string, int>& lunchMenu, const map<string, int>& dinnerMenu)
        : filename(filename), lunchMenu(lunchMenu), dinnerMenu(dinnerMenu) {}

    // Book a meal
    void bookMeal(const string& name, const vector<string>& selectedItems, bool lunch, bool dinner, const string& hall, const string& time) {
        ofstream outputFile(filename, ios::app);
        if (!outputFile.is_open()) {
            cout << "Error: Unable to open file for writing." << endl;
            return;
        }

        int totalCost = 0;
        if (lunch) {
            totalCost += LUNCH_FIXED_COST;
            for (const auto& item : selectedItems) {
                totalCost += lunchMenu[item];
            }
        }
        if (dinner) {
            totalCost += DINNER_FIXED_COST;
            for (const auto& item : selectedItems) {
                totalCost += dinnerMenu[item];
            }
        }

        string meals = lunch ? "Lunch" : "";
        meals += dinner ? (lunch ? " + Dinner" : "Dinner") : "";

        outputFile << name << "," << meals << "," << hall << "," << totalCost << "," << time << endl;
        outputFile.close();
        cout << "Booking made successfully!" << endl;
        showOrderDetails(name, selectedItems, lunch, dinner, hall,time, totalCost);
    }

    // Sort and save bookings
    void sortAndSaveBookings() {
        vector<string> lines;
        ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            cout << "Error: Unable to open file for reading." << endl;
            return;
        }

        string line;
        while (getline(inputFile, line)) {
            lines.push_back(line);
        }
        inputFile.close();

        sort(lines.begin(), lines.end());

        ofstream outputFile(filename);
        if (!outputFile.is_open()) {
            cout << "Error: Unable to open file for writing." << endl;
            return;
        }

        for (const auto& line : lines) {
            outputFile << line << endl;
        }
        outputFile.close();
        cout << "Bookings sorted and saved successfully!" << endl;
    }

    // Search bookings by name
    void searchByName(const string& name) {
        ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            cout << "Error: Unable to open file for reading." << endl;
            return;
        }

        string line;
        bool found = false;
        while (getline(inputFile, line)) {
            size_t pos = line.find(name);
            if (pos != string::npos) {
                cout << line << endl;
                found = true;
            }
        }
        inputFile.close();
        if (!found) {
            cout << "No bookings found for the name: " << name << endl;
        }
    }

    // Cancel a booking by name
    void cancelOrder(const string& name) {
        ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            cout << "Error: Unable to open file for reading." << endl;
            return;
        }

        ofstream tempFile("temp.txt");
        if (!tempFile.is_open()) {
            cout << "Error: Unable to open temporary file." << endl;
            return;
        }

        string line;
        bool found = false;
        while (getline(inputFile, line)) {
            size_t pos = line.find(name);
            if (pos == string::npos) {
                tempFile << line << endl;
            } else {
                found = true;
            }
        }
        inputFile.close();
        tempFile.close();

        if (found) {
            remove(filename.c_str());
            rename("temp.txt", filename.c_str());
            cout << "Booking canceled successfully!" << endl;
        } else {
            cout << "No bookings found for the name: " << name << endl;
            remove("temp.txt");
        }
    }

    // Show order details
    void showOrderDetails(const string& name, const vector<string>& selectedItems, bool lunch, bool dinner, const string& hall,const string& time, int totalCost) {
        cout << "\n=== Order Details ===" << endl;
        cout << "Name: " << name << endl;
        cout << "Meal Type: ";
        if (lunch && dinner) {
            cout << "Lunch + Dinner" << endl;
        } else if (lunch) {
            cout << "Lunch" << endl;
        } else if (dinner) {
            cout << "Dinner" << endl;
        }
        cout << "Selected Items: ";
        for (const auto& item : selectedItems) {
            cout << item << ", ";
        }
        cout << endl;
        cout << "Hall: " << hall << endl;
       // cout << "Date: " << date << endl;
        cout << "Time: " << time << endl;
        cout << "Total Cost: Tk." << totalCost << endl;
    }
};

// Display menu options
void displayMenu(const map<string, int>& menu, const string& mealType) {
    clearConsole();
    cout << "Menu for " << mealType << ":" << endl;
    int count = 1;
    for (const auto& item : menu) {
        cout << count++ << ". " << item.first << " - Tk." << item.second << endl;
    }
    cout << endl;
}

// Select items from the menu
void selectItems(const map<string, int>& menu, vector<string>& selectedItems) {
    int numItems;
    cout << "Enter number of items from the menu(Rice is included with every meal): ";
    cin >> numItems;
    selectedItems.resize(numItems);
    cout << "Enter item numbers (separated by spaces): ";
    for (int i = 0; i < numItems; ++i) {
        int itemNumber;
        cin >> itemNumber;
        if (isValidMenuItem(itemNumber, menu.size())) {
            auto it = menu.begin();
            advance(it, itemNumber - 1);
            selectedItems[i] = it->first;
        } else {
            cout << "Invalid item number." << endl;
            break;
        }
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
}

// Check if the selected menu item number is valid
bool isValidMenuItem(int itemNumber, int menuSize) {
    return (itemNumber >= 1 && itemNumber <= menuSize);
}
string getCurrentTimeAsString() {
    // Get current time
    time_t rawTime;
    struct tm * timeinfo;
    char buffer[80];

    time(&rawTime);
    timeinfo = localtime(&rawTime);

    // Format time as a string
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    string currentTime(buffer);

    return currentTime;
}

void afterSignedIn()
{
     map<string, int> lunchMenu = {
        {"Chicken Curry", 60},
        {"Beef Curry", 100},
        {"Fish Curry", 50}
    };

    map<string, int> dinnerMenu = {
        {"Chicken Roast", 80},
        {"Beef Resela", 120},
        {"Fish Fry", 50}
    };

    MealBookingSystem bookingSystem("bookings.txt", lunchMenu, dinnerMenu);

    char choice;
    string name, hall, date, time;
    bool lunch, dinner;

    vector<string> selectedItems;

    do {
        clearConsole();
        cout << "=== Meal Booking System ===" << endl;
        cout << "1. Book a Meal" << endl;
        cout << "2. Search Booking by Name" << endl;
        cout << "3. Cancel Booking by Name" << endl;
        cout << "4. Log Out" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer

        switch (choice) {
            case '1':
                clearConsole();
                cout << "Enter name: ";
                getline(cin, name);
                cout << "\n===Select meal type:===" << endl;
                lebel:
                cout << "1. Lunch" << endl;
                cout << "2. Dinner" << endl;
                cout << "Enter choice: ";
                cin >> choice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
                if (choice == '1') {
                    lunch = true;
                    dinner = false;
                    displayMenu(lunchMenu, "Lunch");
                    selectItems(lunchMenu, selectedItems);
                } else if (choice == '2') {
                    dinner = true;
                    lunch = false;
                    displayMenu(dinnerMenu, "Dinner");
                    selectItems(dinnerMenu, selectedItems);
                } else {
                    cout << "Invalid choice." << endl;
                    clearConsole();
                    goto lebel;
                    break;
                }
                cout << "Enter hall: ";
                getline(cin, hall);
               // cout << "Enter date (YYYY/MM/DD): ";
               // getline(cin, date);
               // cout << "Enter time (HH:MM:SS): ";
            //getline(cin, time);
              //  cout<<getCurrentTimeAsString()<<endl;

                bookingSystem.bookMeal(name, selectedItems, lunch, dinner, hall,getCurrentTimeAsString());
                bookingSystem.sortAndSaveBookings();
                break;

            case '2':
                clearConsole();
                cout << "Enter name to search: ";
                getline(cin, name);
                bookingSystem.searchByName(name);
                break;

            case '3':
                clearConsole();
                cout << "Enter name to cancel booking: ";
                getline(cin, name);
                bookingSystem.cancelOrder(name);
                break;

            case '4':
                clearConsole();
                cout << "Exiting program." << endl;
                return ;
                break;

            default:
                cout << "Invalid choice. Please enter a valid option." << endl;
                break;
        }

        cout << "Press Enter to continue to main menu...";
        //cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
        cin.get();    // Wait for Enter key
    } while (choice != '4');


}

int main() {
     Authentication auth;

    // Load existing users from file
    auth.loadUsersFromFile("users.txt");

    while (true) {
        clearConsole();
        cout<<"\n===Welcome to Meal Ordering System==="<<endl;
        cout << "1. Sign Up\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";

        char choice;
        cin >> choice;
        switch (choice) {
            case '1':
                clearConsole();
                auth.signUp();
                break;
            case '2': {
                clearConsole();
                string username, password;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;

                if (auth.login(username, password)) {
                    afterSignedIn();
                }
                else {
                        clearConsole();
                    cout<<"Invalid username or password..."<<endl;
                    cout<<"Press Enter to go to Log In page..."<<endl;
                    cin.get();
                    cin.get();

                }

                break;
            }
            case '3':
                // Save users to file and exit
                auth.saveUsersToFile("users.txt");
                cout << "Exiting program.\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
