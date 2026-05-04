// main.cpp - Entry Point & Menu System
// OOP: Namespace, RTTI, Exception handling, Object references
#include "manager.h"

using namespace std;
using Hall::HallManager; using Hall::Person; using Hall::Student;
using Hall::HallError; using Hall::AuthError; using Hall::StuType; using Hall::Role;

void adminMenu(HallManager& hm, Person* p) {
    int ch; Role r = p->getRole();
    do {
        cls();
        cout << "\n" << MAG << BLD << "  [" << p->getName() << "] - " << p->info() << RST << "\n";
        cout << YEL << "  1" << RST << ".View Students " << YEL << "2" << RST << ".View Staff  " << YEL << "3" << RST << ".View Rooms\n";
        cout << YEL << "  4" << RST << ".Notices       " << YEL << "5" << RST << ".Complaints  " << YEL << "6" << RST << ".Stats\n";
        if (r==Hall::PROVOST || r==Hall::HALL_MANAGER) cout << YEL << "  7" << RST << ".Alloc Room    " << YEL << "8" << RST << ".Resolve Comp\n";
        if (r==Hall::PROVOST || r==Hall::MEAL_MANAGER) cout << YEL << "  9" << RST << ".Serve Meal    " << YEL << "10" << RST<<".Toggle Meal\n";
        if (r==Hall::PROVOST) cout << RED << " 11" << RST << ".Add Person    " << RED << "12" << RST<<".Rem Person\n";
        cout << RED << "  0" << RST << ".Logout\n";
        ask("Choice > "); cin >> ch;
        try {
            string s, d; int v;
            switch(ch) {
            case 1: hm.showStudents(); break; case 2: hm.showStaff(); break; case 3: hm.showRooms(); break;
            case 4: hm.showNotices(); break;  case 5: hm.showComplaints(); break; case 6: hm.showStats(); break;
            case 7: ask("ID/Rm: "); cin>>s>>v; hm.allocRoom(r, s, v); break;
            case 8: hm.showComplaints(); ask("Index(-1 to skip): "); cin>>v; if(v>=0) hm.resolveComplaint(r, v); break;
            case 9: ask("ID: "); cin>>s; ask("Dt: "); cin>>d; ask("Guest?(1/0): "); cin>>v; hm.serveMeal(r, s, d, v); break;
            case 10: ask("ID/Dt/ON(1/0): "); cin>>s>>d>>v; hm.toggleMeal(r, s, d, v); break;
            case 11:
                ask("1.Student 2.Staff: "); cin>>v;
                if (v==1) { ask("Name ID Pass Type(0/1): "); string n,p; cin>>n>>s>>p>>v; hm.addStudent(r,n,s,p,(StuType)v); }
                else { ask("Name ID Pass Role(0-4) Sal: "); double sal; string n,p; cin>>n>>s>>p>>v>>sal; hm.addStaff(r,n,s,p,(Role)v,sal); }
                break;
            case 12:
                ask("1.Student 2.Staff: "); cin>>v;
                if (v==1) { ask("Student ID: "); cin>>s; hm.removeStudent(r, s); }
                else { ask("Staff ID: "); cin>>s; hm.removeStaff(r, s); }
                break;
            }
        } catch(HallError& e) { err(e.what()); }
        if (ch != 0) { cout << "\n" << DIM << "  [Press Enter to continue...]" << RST; cin.ignore(1000, '\n'); cin.get(); }
    } while (ch != 0);
}

void studentMenu(HallManager& hm, Student& stu) {
    int ch;
    do {
        cls();
        cout << "\n" << CYN << BLD << "  [STUDENT: " << stu.getName() << "]" << RST << "\n";
        cout << YEL << "  1" << RST << ".Profile   " << YEL << "2" << RST << ".Meal Off  " << YEL << "3" << RST << ".Meal On\n";
        cout << YEL << "  4" << RST << ".Check Day " << YEL << "5" << RST << ".Calendar  " << YEL << "6" << RST << ".My Dues\n";
        cout << YEL << "  7" << RST << ".Notices   " << YEL << "8" << RST << ".Complaint " << RED << "0" << RST << ".Logout\n";
        ask("Choice > "); cin >> ch;
        try {
            string d, t;
            switch(ch) {
            case 1: cout<<"\n"; stu.display(); break;
            case 2: ask("Dt(DD-MM): "); cin>>d; stu.mealOff(d); ok("Meal OFF"); break;
            case 3: ask("Dt(DD-MM): "); cin>>d; stu.mealOn(d);  ok("Meal ON");  break;
            case 4: ask("Dt: "); cin>>d; cout<<"  "<<d<<": "<<(stu.isMealOn(d)?GRN+"ON":RED+"OFF")<<RST<<"\n"; break;
            case 5: stu.viewCalendar(); break;
            case 6: cout<<"  Dues: Tk."<<stu.dues()<<"\n"; break;
            case 7: hm.showNotices(); break;
            case 8: ask("Text: "); cin.ignore(); getline(cin,t); ask("Dt: "); cin>>d; hm.fileComplaint(stu.getName(),t,d); break;
            }
        } catch(HallError& e) { err(e.what()); }
        if (ch != 0) { cout << "\n" << DIM << "  [Press Enter to continue...]" << RST; cin.ignore(1000, '\n'); cin.get(); }
    } while (ch != 0);
}

int main() {
    HallManager hm("Dr. M.A Rashid Hall");
    int ch;
    do {
        cls();
        header("Dr. M.A Rashid Hall Management");
        cout << "  " << YEL << "1. Admin Login" << RST << "\n";
        cout << "  " << YEL << "2. Student Login" << RST << "\n";
        cout << "  " << RED << "0. Exit Application" << RST << "\n";
        askW("Select Portal > "); cin >> ch;

        if (ch == 1) { // Admin sub-menu
            cls();
            header("Admin Login Portal");
            cout << YEL << "  [1]" << RST << " Provost       " << YEL << "[4]" << RST << " Ward Boy\n";
            cout << YEL << "  [2]" << RST << " Hall Manager  " << YEL << "[5]" << RST << " Gardener\n";
            cout << YEL << "  [3]" << RST << " Meal Manager  " << RED << "[0]" << RST << " Back\n";
            int roleChoice;
            ask("Select Rank > "); cin >> roleChoice;
            if (roleChoice == 0) continue;
            int expected = roleChoice - 1; // Maps 1-5 to Role enum 0-4
            string id, pass;
            ask("ID: "); cin >> id; ask("Password: "); cin >> pass;
            try {
                Person* user = hm.login(id, pass, true, expected);
                ok("Login Success!"); adminMenu(hm, user);
            } catch(AuthError& e) { err(e.what()); }
        } else if (ch == 2) {
            cls();
            header("Student Login Portal");
            cout << DIM << "  [0] Back to Home" << RST << "\n";
            string id, pass;
            ask("ID: "); cin >> id;
            if (id == "0") continue; // Navigate Back
            ask("Password: "); cin >> pass;
            try {
                Person* user = hm.login(id, pass, false);
                ok("Login Success!"); studentMenu(hm, *(dynamic_cast<Student*>(user)));
            } catch(AuthError& e) { err(e.what()); }
        }
    } while (ch != 0);

    cout << "\n" << YEL << "  Goodbye!\n" << RST;
    return 0;
}
