
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <cctype>
#include <iomanip>
using namespace std;

const int BF_START = 7 * 60 + 30; // 07:30 – 09:00
const int BF_END = 9 * 60;

const int LU_START = 11 * 60 + 30; // 11:30 – 13:00
const int LU_END = 13 * 60;

const int DI_START = 16 * 60 + 30; // 16:30 – 18:00
const int DI_END = 18 * 60;

struct Student
{
    string fullName;
    int age;
    char sex;
    string id;
    int year;
    string department;
    string telephone;
    string emergencyContact;
    int rollNo;
};

struct MealRecord
{
    int rollNo;
    string date;
    string time;
    string session;
};

struct MealTrackingResult
{
    string fullName;
    int rollNo;
    string message;
};
struct Credential
{
    char username[30];
    char password[30];
    int role; // 1 = Admin, 2 = Staff
};

void accept(vector<Student> &s);
void acceptCredentials(string &userName, string &password);
int action();
int adminDashboard();
void analyticsDetail(unsigned int bf_count, unsigned int lu_count, unsigned int di_count);
bool authenticateBinary(string user, string pass, int role);

bool canEatSession(const vector<string> &sessionsEaten, const string &currentSession);

bool detectCurrentSession(string &currentSession, int &sessionEnd);
void deleteSt(vector<Student> &stDb, int i);

void display(vector<Student> stDb);
void display(Student st, int i);
void display(MealTrackingResult st);
void display(int id);

void edit(vector<Student> &stDb, int i);

vector<Student> filter(vector<Student> &stDb, int &year);
vector<Student> filter(vector<Student> &stDb, string &d);
vector<Student> filter(vector<Student> &stDb, char &sx);
void fillForm(vector<Student> &s);
string formatName(string name);

int getCurrentMinutes();

int inputValidater(int lowerBound, int upperBound);
bool isFileEmpty(const string &filename);

int loginPage();

string nowTime();

void read(vector<Student> &sDb);
void readAnalytics(unsigned int &bf_count, unsigned int &lu_count, unsigned int &di_count);
void readCredentials(string &adminUsername, string &adminPassword, string &staffUsername, string &staffPassword);
int readLastId();
void readStatus(vector<MealRecord> &status, int targetMealNo);
vector<string> readTodaySessions(string mealNo, string todayDate);
void runActionPage(vector<Student> &db);
void runAdminDashboard(vector<Student> &db);
void runAnalyticsPage();
void runFilterPage(vector<Student> &db);
void runLoginFlow(vector<Student> &db);
void runRegisterPage(vector<Student> &db);
void runSearchPage(vector<Student> &db);
void runSelectPage(vector<Student> &db);
void runSortPage(vector<Student> &db);
void runStaffDashboard(vector<Student> &db);
void runStudentListPage(vector<Student> &db);
void runTrackingPage(vector<Student> &db);

int search(vector<Student> &stDb);
int search(vector<Student> &stDb, string mealNo);
void selectedSt(vector<Student> &stDb);
void selectedSt(vector<Student> &stDb, int i);
char sexValidator();
void sortByName(vector<Student> &stDb);

string TodayDate();
void track(vector<Student> &stDb, string currentSession);

void update(vector<Student> &stDb, int i);

void write(Student s);
void writeAfterEdit(vector<Student> &stDb);
void writeLastId(int newId);
void writeTodaySession(MealRecord st);

int main()
{
    vector<Student> studentDb;
    runLoginFlow(studentDb);
    return 0;
}
void runLoginFlow(vector<Student> &db)
{
    while (true)
    {
        int role = loginPage();
        if (role == 3)
            exit(0);

        string user, pass;
        acceptCredentials(user, pass);

        // Use binary authentication
        bool valid = authenticateBinary(user, pass, role);

        if (!valid)
        {
            cout << "Invalid login.\n";
            continue;
        }

        // Redirect based on role
        if (role == 1)
            runAdminDashboard(db);
        else
            runStaffDashboard(db);
    }
}

void runAdminDashboard(vector<Student> &db)
{
    cout << "-----------------------------------------------------------------\n";
    cout << "Welcome, Admin\n";
    while (true)
    {
        int opt = adminDashboard();
        switch (opt)
        {
        case 1:
            runRegisterPage(db);
            break;
        case 2:
            runStudentListPage(db);
            break;
        case 3:
            runTrackingPage(db);
            break;
        case 4:
            runAnalyticsPage();
            break;
        case 5:
            return;
        case 6:
            exit(0);
        default:
            break;
        }
    }
}
void runStaffDashboard(vector<Student> &db)
{
    while (true)
    {
        cout << "Staff Menu:\n1.Track\n2.Logout\n=> ";
        int sel = inputValidater(1, 2);
        if (sel == 1)
            runTrackingPage(db);
        else
            return;
    }
}
void runRegisterPage(vector<Student> &db)
{
    accept(db);
}
void runStudentListPage(vector<Student> &db)
{
    read(db);
    if (db.size() == 0)
    {
        cout << "No students \n";
        return;
    }
    runActionPage(db);
}
void runTrackingPage(vector<Student> &db)
{
    string currentSession;
    int sessionEnd;

    if (!detectCurrentSession(currentSession, sessionEnd))
    {
        cout << "No active meal session right now.\n";
        return;
    }

    cout << "------------------------------------------\n";
    cout << "1. Session: " << currentSession << endl;
    cout << "0. Exit.\n";
    cout << "------------------------------------------\n";
    int option = inputValidater(0, 1);

    read(db);

    while (option != 0)
    {
        if (getCurrentMinutes() > sessionEnd)
        {
            cout << "\nSession ended.\n";
            break;
        }

        track(db, currentSession);
        cout << "Track other student.\n";
        cout << "1. Yes\n";
        cout << "0. no\n";
        option = inputValidater(0, 1);
    }
}

void runAnalyticsPage()
{
    unsigned int bf = 0, lu = 0, di = 0;
    readAnalytics(bf, lu, di);
    analyticsDetail(bf, lu, di);
}
void runActionPage(vector<Student> &db)
{
    while (true)
    {
        display(db);
        int option = action();

        switch (option)
        {
        case 1:
            runSelectPage(db);
            break;
        case 2:
            runSortPage(db);
            break;
        case 3:
            runFilterPage(db);
            break;
        case 4:
            runSearchPage(db);
            break;
        default:
            return;
        }
    }
}
void runSelectPage(vector<Student> &db)
{
    selectedSt(db);
}
void runSortPage(vector<Student> &db)
{
    vector<Student> temp = db;
    sortByName(temp);
    display(temp);
    cout << "Enter a character to back\n";
    string option;
    cin >> option;
}
void runFilterPage(vector<Student> &db)
{
    cout << "----------------------------------------------------------\n";
    while (true)
    {
        cout << "---------------------------------------------\n";
        cout << "1.Filter by Department\n";
        cout << "2.Filter by Year\n";
        cout << "3.Filter by sex\n";
        cout << "4.back\n";
        int option = inputValidater(1, 4);
        vector<Student> f;

        if (option == 1)
        {
            string d;
            cout << "Department: ";
            cin.ignore(10000, '\n');
            getline(cin, d);
            d = formatName(d);
            f = filter(db, d);
        }
        else if (option == 2)
        {
            int y;
            cout << "Year: ";
            y = inputValidater(1, 8);
            f = filter(db, y);
        }
        else if (option == 3)
        {
            char sx;
            cout << "Sex (M/F): ";
            sx = sexValidator();
            f = filter(db, sx);
        }
        else
        {
            return;
        }
        if (f.empty())
        {
            cout << "\nNo students found for the given department.\n";
            return;
        }
        sortByName(f);
        display(f);
    }
}
void runSearchPage(vector<Student> &db)
{
    int i = search(db);

    if (i != -1)
    {
        display(db[i], i + 1);
        cout << "\n-----------------------------------------------------\n";
        cout << "1. Select a Student\n";
        cout << "2. back\n";
        int option = inputValidater(1, 2);
        if (option == 1)
        {
            selectedSt(db, i);
        }
        else
        {
            return;
        }
    }
    else
    {
        cout << "Not found\n";
    }
}

int loginPage()
{
    cout << "---------------------------------------------------------------------\n";
    cout << "--------------------------Login Page---------------------------------\n";
    cout << "---------------------------------------------------------------------\n";
    cout << "Role         \n";
    cout << "1. Admin\n";
    cout << "2. Staff\n";
    cout << "3. Exit\n";
    int option = inputValidater(1, 3);
    return option;
}
void acceptCredentials(string &userName, string &password)
{
    cout << "---------------------------------------------\n";
    cout << "Username: ";
    cin >> userName;
    cout << "Password: ";
    cin >> password;
}
void readCredentials(string &adminUsername, string &adminPassword, string &staffUsername, string &staffPassword)
{
    ifstream in;
    in.open("credentials.csv", ios::in);
    if (!in)
    {
        cerr << "No file\n";
        return;
    }
    string header;
    getline(in, header);
    getline(in, adminUsername, ',');
    getline(in, adminPassword);
    if (!adminPassword.empty() && (adminPassword.back() == '\r' || adminPassword.back() == '\n'))
        adminPassword.pop_back();
    getline(in, staffUsername, ',');
    getline(in, staffPassword);
    if (!staffPassword.empty() && (staffPassword.back() == '\r' || staffPassword.back() == '\n'))
        staffPassword.pop_back();

    in.close();
}
bool authenticateBinary(string user, string pass, int role)
{
    ifstream in("credentials.dat", ios::binary);
    if (!in)
    {
        cerr << "Credentials file missing. Contact IT department.\n";
        return false;
    }

    Credential c;
    while (in.read((char *)&c, sizeof(c)))
    {
        if (user == c.username && pass == c.password && role == c.role)
        {
            in.close();
            return true;
        }
    }

    in.close();
    return false;
}

int adminDashboard()
{
    int option;
    cout << "----------------------------------------------------\n";
    cout << "1. Register\n";
    cout << "2. View Students List\n";
    cout << "3. Track student\n";
    cout << "4. Analytics\n";
    cout << "5. Logout (Return to login)\n";
    cout << "6. Exit System\n";
    option = inputValidater(1, 6);
    return option;
}
void analyticsDetail(unsigned int bf_count, unsigned int lu_count, unsigned int di_count)
{
    cout << "**************************************************************\n";
    cout << "Today's Students served at each session\n";
    cout << "---------------------------------------------------\n";
    cout << left << setw(12) << "Session" << setw(20) << "Total Students Served" << endl;

    cout << "---------------------------------------------------\n";
    cout << left << setw(12) << "Breakfast" << setw(10) << bf_count << endl;
    cout << left << setw(12) << "Lunch" << setw(10) << lu_count << endl;
    cout << left << setw(12) << "Dinner" << setw(10) << di_count << endl;
}
void readAnalytics(unsigned int &bf_count, unsigned int &lu_count, unsigned int &di_count)
{
    ifstream in;
    in.open("meal_history.csv", ios::in);
    if (!in)
    {
        cerr << "No file\n";
        return;
    }
    string todayDate = TodayDate();
    string id, date, time, session, header;
    getline(in, header);
    while (getline(in, id, ','))
    {
        getline(in, date, ',');
        getline(in, time, ',');
        getline(in, session);
        if (date == todayDate)
        {
            if (session == "Breakfast")
            {
                bf_count++;
            }
            else if (session == "Lunch")
            {
                lu_count++;
            }
            else if (session == "Dinner")
            {
                di_count++;
            }
        }
    }
    in.close();
}
string formatName(string name)
{
    string cleaned;
    bool space = false;

    // 1. Trim & remove extra spaces
    for (int i = 0; i < name.length(); i++)
    {
        if (isalpha(name[i]))
        {
            cleaned += name[i];
            space = false;
        }
        else if (name[i] == ' ' && !space && !cleaned.empty())

        {
            cleaned += ' ';
            space = true;
        }
    }

    // Remove trailing space if exists
    if (!cleaned.empty() && cleaned.back() == ' ')
        cleaned.pop_back();

    // 2. Capitalize each word
    bool newWord = true;
    for (int i = 0; i < cleaned.length(); i++)
    {
        if (isalpha(cleaned[i]))
        {
            if (newWord)
            {
                cleaned[i] = toupper(cleaned[i]);
                newWord = false;
            }
            else
            {
                cleaned[i] = tolower(cleaned[i]);
            }
        }
        else
        {
            newWord = true;
        }
    }

    return cleaned;
}

int inputValidater(int lowerBound, int upperBound)
{
    int option;
    while (true)
    {
        cin >> option;
        if (!cin.fail())
        {
            if (option < lowerBound || option > upperBound)
            {
                cout << "Please enter valid Number (" << lowerBound << "-" << upperBound << "):";
            }
            else
            {
                return option;
            }
        }
        else
        {
            cout << "please valid input: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
}

void accept(vector<Student> &s)
{
    while (true)
    {
        cout << "1. Add Student\n";
        cout << "2. Quit\n";
        cout << "=> ";
        int option = inputValidater(1, 2);
        if (option == 2)
        {
            return;
        }
        else
        {
            fillForm(s);
            if (!s.empty())
            {
                Student lastStudent = s.back();
                write(lastStudent);
            }
        }
    }
}
char sexValidator()
{
    char sex;
    while (true)
    {
        cin >> sex;
        sex = toupper(sex);
        if (sex == 'M' || sex == 'F')
        {
            return sex;
        }
        cout << "Incorrect input, please enter again..\n";
    }
}
void fillForm(vector<Student> &s)
{
    Student st;
    cout << "Full name: ";
    cin.ignore(10000, '\n');
    getline(cin, st.fullName);
    st.fullName = formatName(st.fullName); // name format
    cout << "Age: ";
    st.age = inputValidater(15, 50);
    cout << "Sex(M/F): ";
    st.sex = sexValidator();
    cout << "Department: ";
    cin.ignore(10000, '\n');
    getline(cin, st.department);
    st.department = formatName(st.department);
    cout << "Id: ";
    cin >> st.id;
    cout << "Year: ";
    st.year = inputValidater(1, 8);
    cout << "Telephone: ";
    cin >> st.telephone;
    cout << "Emergency contact: ";
    cin >> st.emergencyContact;
    st.rollNo = readLastId();
    cout << "-----------------------------------------------\n";
    cout << "Your Meal Card Number: " << st.rollNo << endl;
    cout << "-----------------------------------------------\n";
    writeLastId(st.rollNo + 1);
    s.push_back(st);
}

bool isFileEmpty(const string &filename)
{
    ifstream in(filename, ios::in);
    if (!in)
        return true; // file does not exist

    in.seekg(0, ios::end);
    return in.tellg() == 0; // file exists but empty
}

void write(Student s)
{
    bool writeHeader = isFileEmpty("records.csv");
    ofstream out;
    out.open("records.csv", ios::app);
    if (!out.is_open())
    {
        cout << "Unable to open file for writing\n";
        return;
    }
    if (writeHeader)
    {
        out << "Full Name" << "," << "Age" << "," << "Sex" << "," << "Department" << "," << "Id" << "," << "Year" << "," << "Telephone" << "," << "Emergency Contact" << "," << "Meal number" << endl; // writing csv header
    }
    out << s.fullName
        << "," << s.age
        << "," << s.sex
        << "," << s.department
        << "," << s.id
        << "," << s.year
        << "," << s.telephone
        << "," << s.emergencyContact
        << "," << s.rollNo << endl; // writing csv file

    out.close();
}
// read last id and return
int readLastId()
{
    string lastId;
    int initializeId = 1000;
    ifstream in;
    in.open("lastId.txt", ios::in);
    if (!in)
    {
        return initializeId;
    }
    in >> lastId;
    return stoi(lastId);
}
// write incremented id
void writeLastId(int newId)
{
    ofstream out;
    out.open("lastId.txt", ios::out);
    if (!out)
    {
        cerr << "There is no such file\n";
        return;
    }
    out << newId;
}

// read and store to db
void read(vector<Student> &stDb)
{
    stDb.clear();
    ifstream in;
    in.open("records.csv", ios::in);
    if (!in)
    {
        cerr << "There is no such file\n";
        return;
    }
    string header;
    Student st;
    getline(in, header);
    while (getline(in, st.fullName, ','))
    {

        string ageStr;
        getline(in, ageStr, ',');
        st.age = stoi(ageStr);
        string sexStr;
        getline(in, sexStr, ',');
        st.sex = sexStr[0];
        getline(in, st.department, ',');
        getline(in, st.id, ',');
        string yearStr;
        getline(in, yearStr, ',');
        st.year = stoi(yearStr);
        getline(in, st.telephone, ',');
        getline(in, st.emergencyContact, ',');
        string rollStr;
        getline(in, rollStr);
        st.rollNo = stoi(rollStr);

        stDb.push_back(st);
    }
}
void display(vector<Student> stDb)
{

    cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";

    cout << left << setw(8) << "Roll"
         << setw(20) << "Full Name"
         << setw(14) << "Age"
         << setw(12) << "Sex"
         << setw(20) << "Department"
         << setw(17) << "Id"
         << setw(12) << "Year"
         << setw(20) << "Telephone"
         << setw(20) << "Emergency Contact"
         << right << setw(20) << "Meal Number"
         << endl;

    cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";

    for (int i = 0; i < stDb.size(); i++)
    {
        cout << left
             << setw(8) << i + 1
             << setw(20) << stDb[i].fullName
             << setw(14) << stDb[i].age
             << setw(12) << stDb[i].sex
             << setw(20) << stDb[i].department
             << setw(17) << stDb[i].id
             << setw(12) << stDb[i].year
             << setw(20) << stDb[i].telephone
             << setw(20) << stDb[i].emergencyContact
             << right << setw(20) << stDb[i].rollNo
             << endl;
    }
}
int action()
{
    int option;
    cout << "#################################################################\n";
    cout << "1. Select a Student\n";
    cout << "2. Sort by Name\n";
    cout << "3. Filter\n";
    cout << "4. Search by Id\n";
    cout << "5. back\n";
    option = inputValidater(1, 5);
    return option;
}
void display(Student st, int i)
{
    cout << "-----------------------------------------------------------------------------------------------------------------------------------------------------------\n";

    cout << left
         << setw(8) << "Roll"
         << setw(20) << "Full Name"
         << setw(14) << "Age"
         << setw(12) << "Sex"
         << setw(20) << "Department"
         << setw(17) << "Id"
         << setw(12) << "Year"
         << setw(20) << "Telephone"
         << setw(20) << "Emergency Contact"
         << right << setw(20) << "Meal Number"
         << endl;

    cout << "-----------------------------------------------------------------------------------------------------------------------------------------------------------\n";

    cout << left
         << setw(8) << i
         << setw(20) << st.fullName
         << setw(14) << st.age
         << setw(12) << st.sex
         << setw(20) << st.department
         << setw(17) << st.id
         << setw(12) << st.year
         << setw(20) << st.telephone
         << setw(20) << st.emergencyContact
         << right << setw(20) << st.rollNo
         << endl;
}
void writeAfterEdit(vector<Student> &stDb)
{
    ofstream out;
    out.open("records.csv", ios::out);
    if (!out.is_open())
    {
        cout << "There is no such file\n";
        return;
    }
    out << "Full Name" << "," << "Age" << "," << "Sex" << "," << "Department" << "," << "Id" << "," << "Year" << "," << "Telephone" << "," << "Emergency Contact" << "," << "Meal number" << endl; // writing csv header

    for (int i = 0; i < stDb.size(); i++)
    {
        out << stDb[i].fullName
            << "," << stDb[i].age
            << "," << stDb[i].sex
            << "," << stDb[i].department
            << "," << stDb[i].id
            << "," << stDb[i].year
            << "," << stDb[i].telephone
            << "," << stDb[i].emergencyContact
            << "," << stDb[i].rollNo << endl;
    }
    out.close();
}
void update(vector<Student> &stDb, int i)
{
    bool hasUnsavedChanges = false;
    int option = -1;
    do
    {
        cout << "-------------------------------------------------------------\n";
        cout << "1. New name\n";
        cout << "2. Age\n";
        cout << "3. Sex\n";
        cout << "4. Department\n";
        cout << "5. Id\n";
        cout << "6. Year\n";
        cout << "7. Telephone\n";
        cout << "8. Emergency contact\n";
        cout << "9. Save\n";
        cout << "10. Back\n";
        option = inputValidater(1, 10);

        switch (option)
        {
        case 1:
            cout << "New name\n";
            cin.ignore(10000, '\n');
            getline(cin, stDb[i].fullName);
            stDb[i].fullName = formatName(stDb[i].fullName);
            hasUnsavedChanges = true;
            break;
        case 2:
            cout << "New age\n";
            stDb[i].age = inputValidater(0, 100);
            hasUnsavedChanges = true;
            break;
        case 3:
            cout << "New Sex\n";
            stDb[i].sex = sexValidator();
            hasUnsavedChanges = true;
            break;
        case 4:
            cout << "New dept\n";
            cin.ignore(10000, '\n');
            getline(cin, stDb[i].department);
            stDb[i].department = formatName(stDb[i].department);
            hasUnsavedChanges = true;
            break;
        case 5:
            cout << "New id\n";
            cin >> stDb[i].id;
            hasUnsavedChanges = true;
            break;
        case 6:
            cout << "New year\n";
            cin >> stDb[i].year;
            hasUnsavedChanges = true;
            break;
        case 7:
            cout << "New tel\n";
            cin >> stDb[i].telephone;
            hasUnsavedChanges = true;
            break;
        case 8:
            cout << "New emergency contact\n";
            cin >> stDb[i].emergencyContact;
            hasUnsavedChanges = true;
            break;
        case 9:
            writeAfterEdit(stDb);
            hasUnsavedChanges = false;
            cout << "Updated successfully\n";
            return;
        }

    } while (option != 10);

    // Handle unsaved changes when user selects Back
    if (hasUnsavedChanges)
    {
        cout << "You have unsaved changes.\n";
        cout << "1. Save changes\n";
        cout << "2. Discard changes\n";
        cout << "3. Cancel\n";
        int choice = inputValidater(1, 3);

        if (choice == 1)
        {
            writeAfterEdit(stDb);
            cout << "Saved successfully\n";
            return;
        }
        else if (choice == 2)
        {
            cout << "Changes discarded\n";
            return;
        }
        else
        {
            update(stDb, i); // stay in edit
            return;
        }
    }
}

void deleteSt(vector<Student> &stDb, int i)
{
    cout << "--------------------------------------------------------------------------\n";
    cout << "Are you sure?\n";
    cout << "1. Yes\n";
    cout << "2. No\n";
    int option = inputValidater(1, 2);
    if (option == 1)
    {
        stDb.erase(stDb.begin() + i);
        cout << "Student deleted successfully\n";
    }
    else
        return;
    writeAfterEdit(stDb);
}
void edit(vector<Student> &stDb, int i)
{
    int option;
    cout << "-----------------------------------------------------------------------\n";
    cout << "1. Update\n";
    cout << "2. Delete\n";
    cout << "3. back\n";
    option = inputValidater(1, 3);
    if (option == 1)
    {
        update(stDb, i);
    }
    else if (option == 2)
    {
        deleteSt(stDb, i);
        return;
    }
    else if (option == 3)
    {
        return;
    }
}
void readStatus(vector<MealRecord> &status, int targetMealNo)
{
    status.clear();
    MealRecord st;
    ifstream in;
    in.open("meal_history.csv", ios::in);
    if (!in)
    {
        cerr << "No history file.\n";
        return;
    }
    string header;
    string id;
    string date;
    string time;
    string session;
    getline(in, header);
    while (getline(in, id, ','))
    {
        getline(in, date, ',');
        getline(in, time, ',');
        getline(in, session);
        if (targetMealNo == stoi(id))
        {
            st.date = date;
            st.time = time;
            st.session = session;
            status.push_back(st);
        }
    }
    in.close();
}

void display(int id)
{
    vector<MealRecord> status;
    readStatus(status, id);
    if (status.size() != 0)
    {
        cout << left
             << setw(10) << "Date"
             << setw(10) << "Time"
             << setw(10) << "Session"
             << endl;

        for (int i = 0; i < status.size(); i++)
        {
            cout << left
                 << setw(10) << status[i].date
                 << setw(10) << status[i].time
                 << setw(10) << status[i].session
                 << endl;
        }
    }
    else
    {
        cout << "Not eaten yet\n";
    }
}
void selectedSt(vector<Student> &stDb, int i)
{
    while (true)
    {
        cout << "---------------------------------------------------------\n";
        cout << "1. Edit\n";
        cout << "2. See meal History\n";
        cout << "3. back\n";
        int option = inputValidater(1, 3);

        switch (option)
        {
        case 1:
        {
            edit(stDb, i);
            break;
        }
        case 2:
        {
            int mealNo = stDb[i].rollNo;
            display(mealNo);
            break;
        }
        default:
            return;
        }
    }
}
void selectedSt(vector<Student> &stDb)
{
    int rno;
    cout << "---------------------------------------------------------\n";
    cout << "Select a student\nenter the roll no\n";
    rno = inputValidater(1, stDb.size());
    display(stDb[rno - 1], rno);

    while (true)
    {
        cout << "---------------------------------------------------------\n";
        cout << "1. Edit\n";
        cout << "2. See meal History\n";
        cout << "3. back\n";
        int option = inputValidater(1, 3);

        switch (option)
        {
        case 1:
        {
            edit(stDb, rno - 1);
            break;
        }
        case 2:
        {
            int mealNo = stDb[rno - 1].rollNo;
            display(mealNo);
            break;
        }
        default:
            return;
        }
    }
}

void sortByName(vector<Student> &stDb)
{
    for (int i = 0; i < stDb.size() - 1; i++)
        for (int j = i + 1; j < stDb.size(); j++)
            if (stDb[i].fullName > stDb[j].fullName)
            {
                Student t = stDb[i];
                stDb[i] = stDb[j];
                stDb[j] = t;
            }
}

vector<Student> filter(vector<Student> &stDb, int &year)
{
    vector<Student> f;

    for (int i = 0; i < stDb.size(); i++)
    {
        if (stDb[i].year == year)
        {
            f.push_back(stDb[i]);
        }
    }
    return f;
}
vector<Student> filter(vector<Student> &stDb, string &d)
{
    vector<Student> f;

    for (int i = 0; i < stDb.size(); i++)
    {
        if (stDb[i].department == d)
        {
            f.push_back(stDb[i]);
        }
    }
    return f;
}
vector<Student> filter(vector<Student> &stDb, char &sx)
{
    vector<Student> f;

    for (int i = 0; i < stDb.size(); i++)
    {
        if (stDb[i].sex == sx)
        {
            f.push_back(stDb[i]);
        }
    }
    return f;
}
int search(vector<Student> &stDb)
{
    string id;
    cout << "Enter id: ";
    cin >> id;
    for (int i = 0; i < stDb.size(); i++)
    {
        if (stDb[i].id == id)
        {
            return i;
        }
    }
    return -1;
}
int search(vector<Student> &stDb, string mealNo)
{
    for (int i = 0; i < stDb.size(); i++)
    {
        if (stDb[i].rollNo == stoi(mealNo))
        {
            return i;
        }
    }
    return -1;
}
int getCurrentMinutes()
{
    time_t now = time(0);
    tm *t = localtime(&now);
    return t->tm_hour * 60 + t->tm_min;
}

bool detectCurrentSession(string &currentSession, int &sessionEnd)
{
    int minutes = getCurrentMinutes();

    if (minutes >= BF_START && minutes <= BF_END)
    {
        currentSession = "Breakfast";
        sessionEnd = BF_END;
    }
    else if (minutes >= LU_START && minutes <= LU_END)
    {
        currentSession = "Lunch";
        sessionEnd = LU_END;
    }
    else if (minutes >= DI_START && minutes <= DI_END)
    {
        currentSession = "Dinner";
        sessionEnd = DI_END;
    }
    else
    {
        return false;
    }

    return true;
}

void display(MealTrackingResult st)
{
    cout << "------------------------------------------------\n";
    cout << left << setw(15) << "FullName:" << st.fullName << endl;
    cout << left << setw(15) << "MealNumber:" << st.rollNo << endl;

    cout << st.message << endl;
}
void track(vector<Student> &stDb, string currentSession)
{
    string mealNo;
    cout << "Tracking.....\n";
    cout << "-------------------------------------------------\n";
    cout << "Enter Meal Card Number: ";
    cin >> mealNo;

    int index = search(stDb, mealNo);
    if (index == -1)
    {
        cout << "Student not registered. You can't eat.\n";
        return;
    }

    string todayDate = TodayDate();
    string time = nowTime();

    MealTrackingResult stu;
    stu.fullName = stDb[index].fullName;
    stu.rollNo = stDb[index].rollNo;

    vector<string> sessionsEaten = readTodaySessions(mealNo, todayDate);

    bool canEat = canEatSession(sessionsEaten, currentSession);

    if (sessionsEaten.empty())
    {
        stu.message = "First meal today. You can eat.\n";
    }
    else if (canEat)
    {
        stu.message = "Different session. You can eat.\n";
    }
    else
    {
        stu.message = "You already ate this session. You can't eat.\n";
    }

    display(stu);

    if (canEat)
    {
        MealRecord servedStudent;
        servedStudent.rollNo = stDb[index].rollNo;
        servedStudent.date = todayDate;
        servedStudent.time = time;
        servedStudent.session = currentSession;

        writeTodaySession(servedStudent);
    }
}

string TodayDate()
{
    time_t now = time(0);
    struct tm *time_info = localtime(&now);
    string mday = to_string(time_info->tm_mday);
    string mon = to_string(1 + time_info->tm_mon);
    string year = to_string(1900 + time_info->tm_year);

    string todayDate = mday + "/" + mon + "/" + year;
    return todayDate;
}
string nowTime()
{
    time_t now = time(0);
    struct tm *time_info = localtime(&now);
    string hr = to_string(time_info->tm_hour);
    string min = to_string(time_info->tm_min);
    string sec = to_string(time_info->tm_sec);
    if (hr.length() == 1)
        hr = "0" + hr;
    if (min.length() == 1)
        min = "0" + min;
    if (sec.length() == 1)
        sec = "0" + sec;

    string time = hr + ":" + min + ":" + sec;
    return time;
}
void writeTodaySession(MealRecord st)
{
    ofstream out;
    bool writeHeader = isFileEmpty("meal_history.csv");
    out.open("meal_history.csv", ios::app);

    if (!out)
    {
        cerr << "No meal_history.csv file\n";
        return;
    }
    if (writeHeader)
    {
        out << "Meal Card Number,Date,Time,Served Session\n";
    }
    out << st.rollNo << ","
        << st.date << ","
        << st.time << ","
        << st.session << endl;

    out.close();
}

vector<string> readTodaySessions(string mealNo, string todayDate)
{
    ifstream in("meal_history.csv");
    vector<string> sessionsEaten;

    if (!in)
        return sessionsEaten; // empty = no records

    string mno, date, time, session, header;
    getline(in, header); // skip header

    while (getline(in, mno, ','))
    {
        getline(in, date, ',');
        getline(in, time, ',');
        getline(in, session);

        if (mno == mealNo && date == todayDate)
        {
            sessionsEaten.push_back(session); // collect all sessions eaten today
        }
    }

    in.close();
    return sessionsEaten;
}

// New check logic: returns true if student can eat current session
bool canEatSession(const vector<string> &sessionsEaten, const string &currentSession)
{
    for (int i = 0; i < sessionsEaten.size(); i++)
    {
        if (sessionsEaten[i] == currentSession)
            return false; // already eaten this session
    }
    return true; // current session not eaten yet
}
