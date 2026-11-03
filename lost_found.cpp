// Smart Campus Lost & Found Management System
// BCA I Semester - Lab Sheet 10
// Concepts: Arrays, Structures, File Handling, Functions, Loops, Searching, Sorting, Strings

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

const int MAX_ITEMS = 500;
const string DATA_FILE = "records.txt";
const string REPORT_FILE = "report.txt";

// ---------- Structure ----------
struct Item {
    int id;
    char type;          // 'L' = lost, 'F' = found
    bool matched;       // true once auto-matched with the opposite type
    string name;
    string category;
    string description;
    string location;
    string date;
    string reporterRoll;
};

// ---------- Global data ----------
Item items[MAX_ITEMS];
int itemCount = 0;
int nextId = 1;
string currentRoll;

// Sample student database (roll number + password)
const int STUDENT_COUNT = 5;
string rollList[STUDENT_COUNT] = {"BCA001", "BCA002", "BCA003", "BCA004", "BCA005"};
string passList[STUDENT_COUNT] = {"coer1", "coer2", "coer3", "coer4", "coer5"};

// ---------- String helpers ----------
string toLower(string s) {
    for (size_t i = 0; i < s.length(); i++) {
        if (s[i] >= 'A' && s[i] <= 'Z') s[i] = s[i] + 32;
    }
    return s;
}

// true if a contains b (case-insensitive)
bool containsText(const string &a, const string &b) {
    return toLower(a).find(toLower(b)) != string::npos;
}

string readLine(const string &prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    // '|' is our file separator, so replace it to keep the file safe
    for (size_t i = 0; i < s.length(); i++)
        if (s[i] == '|') s[i] = '/';
    return s;
}

int readInt(const string &prompt) {
    string s;
    while (true) {
        cout << prompt;
        getline(cin, s);
        stringstream ss(s);
        int n;
        if (ss >> n) return n;
        cout << "Invalid number, try again.\n";
    }
}

string typeName(char t) { return t == 'L' ? "LOST" : "FOUND"; }

// ---------- File handling ----------
void saveData() {
    ofstream fout(DATA_FILE.c_str());
    for (int i = 0; i < itemCount; i++) {
        fout << items[i].id << '|' << items[i].type << '|' << items[i].matched << '|'
             << items[i].name << '|' << items[i].category << '|' << items[i].description << '|'
             << items[i].location << '|' << items[i].date << '|' << items[i].reporterRoll << '\n';
    }
}

void loadData() {
    ifstream fin(DATA_FILE.c_str());
    if (!fin) return;   // first run, no file yet
    string line;
    while (getline(fin, line) && itemCount < MAX_ITEMS) {
        stringstream ss(line);
        string f[9];
        int k = 0;
        while (k < 9 && getline(ss, f[k], '|')) k++;
        if (k < 9) continue;   // skip broken lines
        Item it;
        it.id = atoi(f[0].c_str());
        it.type = f[1][0];
        it.matched = (f[2] == "1");
        it.name = f[3];
        it.category = f[4];
        it.description = f[5];
        it.location = f[6];
        it.date = f[7];
        it.reporterRoll = f[8];
        items[itemCount++] = it;
        if (it.id >= nextId) nextId = it.id + 1;
    }
}

// ---------- Display ----------
void printItem(const Item &it) {
    cout << "ID: " << it.id << " | " << typeName(it.type)
         << (it.matched ? " (MATCHED)" : "") << "\n"
         << "  Name: " << it.name << " | Category: " << it.category << "\n"
         << "  Desc: " << it.description << "\n"
         << "  Place: " << it.location << " | Date: " << it.date
         << " | By: " << it.reporterRoll << "\n";
}

// ---------- Module 1: Student Login ----------
bool login() {
    for (int attempt = 1; attempt <= 3; attempt++) {
        string roll = readLine("Roll No: ");
        string pass = readLine("Password: ");
        for (int i = 0; i < STUDENT_COUNT; i++) {
            if (rollList[i] == roll && passList[i] == pass) {
                currentRoll = roll;
                cout << "Login successful. Welcome, " << roll << "!\n";
                return true;
            }
        }
        cout << "Wrong credentials. Attempts left: " << 3 - attempt << "\n";
    }
    return false;
}

// ---------- Modules 2 & 3: Report Lost / Found ----------
void reportItem(char type) {
    if (itemCount >= MAX_ITEMS) {
        cout << "Storage full!\n";
        return;
    }
    Item it;
    it.id = nextId++;
    it.type = type;
    it.matched = false;
    it.name = readLine("Item name: ");
    it.category = readLine("Category (ID Card/Wallet/Keys/Laptop/Phone/Other): ");
    it.description = readLine("Description (colour, brand, marks): ");
    it.location = readLine(type == 'L' ? "Where did you lose it? " : "Where did you find it? ");
    it.date = readLine("Date (DD-MM-YYYY): ");
    it.reporterRoll = currentRoll;
    items[itemCount++] = it;
    saveData();
    cout << typeName(type) << " item reported. Record ID = " << it.id << "\n";
}

// ---------- Module 4: Search (linear search) ----------
void searchItem() {
    string key = readLine("Enter keyword (name/category/description/location): ");
    int found = 0;
    for (int i = 0; i < itemCount; i++) {
        if (containsText(items[i].name, key) || containsText(items[i].category, key) ||
            containsText(items[i].description, key) || containsText(items[i].location, key)) {
            printItem(items[i]);
            found++;
        }
    }
    if (found == 0) cout << "No matching records.\n";
    else cout << found << " record(s) found.\n";
}

// ---------- Module 5: Auto Match ----------
bool isMatch(const Item &lost, const Item &found) {
    if (toLower(lost.category) != toLower(found.category)) return false;
    // names match if one contains the other (e.g. "black wallet" vs "wallet")
    return containsText(lost.name, found.name) || containsText(found.name, lost.name);
}

void autoMatch() {
    int matches = 0;
    for (int i = 0; i < itemCount; i++) {
        if (items[i].type != 'L' || items[i].matched) continue;
        for (int j = 0; j < itemCount; j++) {
            if (items[j].type != 'F' || items[j].matched) continue;
            if (isMatch(items[i], items[j])) {
                items[i].matched = true;
                items[j].matched = true;
                matches++;
                cout << "\n--- MATCH #" << matches << " ---\n";
                printItem(items[i]);
                printItem(items[j]);
                break;   // this lost item is done, go to next one
            }
        }
    }
    if (matches == 0) cout << "No new matches found.\n";
    else saveData();
}

// ---------- Module 6: View All (with sorting) ----------
void viewAll() {
    if (itemCount == 0) {
        cout << "No records yet.\n";
        return;
    }
    cout << "Sort by: 1) ID  2) Name (A-Z): ";
    int ch = readInt("");

    // Work on a copy so the original order stays untouched
    Item temp[MAX_ITEMS];
    for (int i = 0; i < itemCount; i++) temp[i] = items[i];

    if (ch == 2) {
        // Bubble sort by name
        for (int i = 0; i < itemCount - 1; i++) {
            for (int j = 0; j < itemCount - i - 1; j++) {
                if (toLower(temp[j].name) > toLower(temp[j + 1].name)) {
                    Item swapTemp = temp[j];
                    temp[j] = temp[j + 1];
                    temp[j + 1] = swapTemp;
                }
            }
        }
    }
    for (int i = 0; i < itemCount; i++) {
        printItem(temp[i]);
        cout << "\n";
    }
}

// ---------- Module 7: Delete ----------
void deleteRecord() {
    int id = readInt("Enter record ID to delete: ");
    for (int i = 0; i < itemCount; i++) {
        if (items[i].id == id) {
            // Only the person who reported it can delete it
            if (items[i].reporterRoll != currentRoll) {
                cout << "You can only delete your own records.\n";
                return;
            }
            for (int j = i; j < itemCount - 1; j++) items[j] = items[j + 1];   // shift left
            itemCount--;
            saveData();
            cout << "Record deleted.\n";
            return;
        }
    }
    cout << "Record ID not found.\n";
}

// ---------- Module 8: Statistics ----------
void showStatistics() {
    int lost = 0, found = 0, matched = 0;
    for (int i = 0; i < itemCount; i++) {
        if (items[i].type == 'L') lost++;
        else found++;
        if (items[i].matched) matched++;
    }
    cout << "Total records : " << itemCount << "\n"
         << "Lost items    : " << lost << "\n"
         << "Found items   : " << found << "\n"
         << "Matched items : " << matched << "\n"
         << "Unmatched     : " << itemCount - matched << "\n";
}

// ---------- Module 9: Export Report ----------
void exportReport() {
    ofstream fout(REPORT_FILE.c_str());
    fout << "===== CAMPUS LOST & FOUND REPORT =====\n\n";
    for (int i = 0; i < itemCount; i++) {
        fout << "ID: " << items[i].id << " | " << typeName(items[i].type)
             << " | " << (items[i].matched ? "Matched" : "Unmatched") << "\n"
             << "Name: " << items[i].name << " | Category: " << items[i].category << "\n"
             << "Description: " << items[i].description << "\n"
             << "Location: " << items[i].location << " | Date: " << items[i].date << "\n"
             << "Reported by: " << items[i].reporterRoll << "\n\n";
    }
    fout << "Total records: " << itemCount << "\n";
    cout << "Report exported to " << REPORT_FILE << "\n";
}

// ---------- Menu ----------
void showMenu() {
    cout << "\n===== SMART CAMPUS LOST & FOUND =====\n"
         << "1. Report Lost Item\n"
         << "2. Report Found Item\n"
         << "3. Search Item\n"
         << "4. Auto Match Lost & Found\n"
         << "5. View All Records\n"
         << "6. Delete Record\n"
         << "7. Item Statistics\n"
         << "8. Export Report\n"
         << "9. Exit\n";
}

int main() {
    loadData();
    cout << "=== Student Login ===\n";
    if (!login()) {
        cout << "Too many failed attempts. Exiting.\n";
        return 0;
    }

    int choice;
    do {
        showMenu();
        choice = readInt("Enter choice: ");
        switch (choice) {
            case 1: reportItem('L'); break;
            case 2: reportItem('F'); break;
            case 3: searchItem(); break;
            case 4: autoMatch(); break;
            case 5: viewAll(); break;
            case 6: deleteRecord(); break;
            case 7: showStatistics(); break;
            case 8: exportReport(); break;
            case 9: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 9);

    return 0;
}
