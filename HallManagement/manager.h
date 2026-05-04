// manager.h - Hall Manager Class & Generic Algorithms
// OOP: Aggregation, RTTI, Exception handling, STL algorithms, Generic programming
#pragma once
#include "models.h"
#include <typeinfo>

namespace Hall {
// GENERIC ALGORITHM: Function template with predicate
template<typename T, typename Pred>
int countIf(vector<T*>& v, Pred p) { int c=0; for (auto* x:v) if (p(x)) c++; return c; }

class HallManager {
    DataList<Student> students;      // Aggregation
    DataList<Staff>   staff;         // Aggregation
    vector<Room>      rooms;         // ARRAY OF OBJECTS
    vector<string>    notices;       // STL VECTOR
    vector<Complaint> complaints;    // Composition
    string hallName;
public:
    HallManager(const string& name) : hallName(name) {
        staff.add(new Staff("Dr. Rahman","P001","admin",PROVOST,80000));
        staff.add(new Staff("Mr. Karim","HM01","admin",HALL_MANAGER,50000));
        staff.add(new Staff("Mr. Hasan","MM01","admin",MEAL_MANAGER,30000));
        staff.add(new Staff("Rahim Mia","WB01","staff",WARD_BOY,15000));
        staff.add(new Staff("Jabbar Ali","GD01","staff",GARDENER,12000));
        students.add(new Student("Sakib Ahmed","S001","1111",RESIDENT,101));
        students.add(new Student("Tamim Iqbal","S002","2222",RESIDENT,101));
        students.add(new Student("Mushfiq R.","S003","3333",NON_RESIDENT,0));
        students.add(new Student("Liton Das","S004","4444",RESIDENT,102));
        students.add(new Student("Soumya S.","S005","5555",NON_RESIDENT,0));
        for (int i=101; i<=110; i++) rooms.push_back(Room(i,2));
        rooms[0].add("S001"); rooms[0].add("S002"); rooms[1].add("S004");
        notices.push_back("Welcome! Meal:Tk.50 | Guest:Tk.100 | Hall:Tk.5000/sem");
    }
    Person* login(const string& id, const string& pass, bool isAdmin, int expectedRole = -1) {
        if (isAdmin) {
            for (auto* s : staff.all()) if (s->getId()==id && s->getPass()==pass) {
                if (expectedRole != -1 && s->getRole() != expectedRole) throw AuthError();
                return s;
            }
        } else {
            for (auto* s : students.all()) if (s->getId()==id && s->getPass()==pass) return s;
        } throw AuthError();
    }
    // Provost Only Features
    void addStudent(Role r, const string& n, const string& i, const string& p, StuType t) {
        if (r != PROVOST) throw HallError("Permission Denied: Provost Only!");
        if (findById(students.all(), i)) throw HallError("ID exists!");
        students.add(new Student(n,i,p,t,0)); ok("Student "+n+" added!");
    }
    void removeStudent(Role r, const string& id) {
        if (r != PROVOST) throw HallError("Permission Denied: Provost Only!");
        if (students.remove(id)) ok("Student removed!"); else err("Student not found!");
    }
    void addStaff(Role r, const string& n, const string& i, const string& p, Role nr, double s) {
        if (r != PROVOST) throw HallError("Permission Denied: Provost Only!");
        if (findById(staff.all(), i)) throw HallError("ID exists!");
        staff.add(new Staff(n,i,p,nr,s)); ok("Staff added!");
    }
    void removeStaff(Role r, const string& id) {
        if (r != PROVOST) throw HallError("Permission Denied: Provost Only!");
        if (staff.remove(id)) ok("Staff removed!"); else err("Staff not found!");
    }
    // Shared / Hall Manager Features
    void allocRoom(Role r, const string& sid, int rno) {
        if (r!=PROVOST && r!=HALL_MANAGER) throw HallError("Permission Denied!");
        Student* s = findById(students.all(), sid); if (!s) throw HallError("No Student!");
        for (auto& rm : rooms) {
            if (rm.getNo()==rno) {
                if (rm.full()) throw RoomError();
                rm.add(sid); s->setRoom(rno); ok("Room "+to_string(rno)+" Alloc."); return;
            }
        } throw HallError("Room not found!");
    }
    // Meal Manager Features
    void serveMeal(Role r, const string& sid, const string& date, bool guest=false) {
        if (r!=PROVOST && r!=MEAL_MANAGER) throw HallError("Permission Denied!");
        Student* s = findById(students.all(), sid); if (!s) throw HallError("No Student!");
        if (guest) { s->addGuest(); ok("Guest meal served!"); return; }
        if (!s->isMealOn(date)) throw HallError("Meal OFF for "+date+"!");
        s->eat(); ok("Meal served! Count: "+to_string(s->getMeals()));
    }
    void toggleMeal(Role r, const string& sid, const string& date, bool on) {
        if (r!=PROVOST && r!=MEAL_MANAGER) throw HallError("Permission Denied!");
        Student* s = findById(students.all(), sid); if (!s) throw HallError("No Student!");
        on ? s->mealOn(date) : s->mealOff(date); ok(string("Meal ")+(on?"ON":"OFF"));
    }
    void bulkMealOff(Role r, const string& sid, const vector<string>& dates) {
        if (r!=PROVOST && r!=MEAL_MANAGER) throw HallError("Permission Denied!");
        Student* s = findById(students.all(), sid); if (!s) throw HallError("No Student!");
        for (const auto& d : dates) s->mealOff(d); 
        ok(to_string(dates.size())+" days turned OFF");
    }
    // Display & Generics
    void showStudents() { header("Students"); students.show(); }
    void showStaff()    { header("Staff");    staff.show(); }
    void showRooms()    { header("Rooms");    for (auto& r:rooms) r.display(); }
    void addNotice(const string& t) { notices.push_back(t); ok("Posted!"); }
    void showNotices()  { header("Notice Board"); for (size_t i=0; i<notices.size(); i++) cout<<"  "<<CYN<<i+1<<"."<<RST<<" "<<notices[i]<<"\n"; }
    void fileComplaint(const string& f, const string& t, const string& d) { complaints.push_back(Complaint(f,t,d)); ok("Complaint filed!"); }
    void showComplaints() {
        header("Complaints"); if (complaints.empty()) { warn("No complaints."); return; }
        for (size_t i=0; i<complaints.size(); i++) { cout<<DIM<<"  #"<<i<<RST<<" "; complaints[i].display(); }
    }
    void resolveComplaint(Role r, int i) {
        if (r!=PROVOST && r!=HALL_MANAGER) throw HallError("Permission Denied!");
        if (i<0 || i>=(int)complaints.size()) throw HallError("Invalid index!");
        complaints[i].resolve(); ok("Complaint resolved!");
    }
    void identify(Person* p) {
        cout<<YEL<<"  typeid: "<<typeid(*p).name()<<RST<<"\n";
        if (dynamic_cast<Student*>(p))    ok("Identified: Student");
        else if (dynamic_cast<Staff*>(p)) ok("Identified: Staff");
    }
    void showStats() {
        header(hallName+" Statistics");
        int res = countIf(students.all(), [](Student* s){ return s->getType()==RESIDENT; });
        cout<<"  Students : "<<BLD<<students.size()<<RST<<" ("<<GRN<<"Res:"<<res<<RST<<" "<<YEL<<"Non:"<<students.size()-res<<RST<<")\n"
            <<"  Staff    : "<<BLD<<staff.size()<<RST<<" | Rooms: "<<BLD<<rooms.size()<<RST<<"\n";
        int tm=0; double td=0; for (auto* s : students.all()) { tm += (int)(*s); td += s->dues(); } // Conversion
        cout<<"  Meals    : "<<GRN<<tm<<RST<<" | Dues: "<<BLD<<"Tk."<<td<<RST<<"\n"; line();
    }
};
}
