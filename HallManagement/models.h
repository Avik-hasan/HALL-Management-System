// models.h - Core Classes, Templates & Exceptions
// OOP: Interface, Abstract class, Inheritance, Constructors/Destructors,
//   Operator overloading, Type conversion, Templates, Namespace, STL, Composition
#pragma once
#include "color.h"
#include <vector>
#include <map>
#include <exception>

namespace Hall {
// ===== INTERFACE: Pure abstract class =====
class IDisplayable { public: virtual void display() const = 0; virtual ~IDisplayable() {} };

// ===== Enums & Helper =====
enum Role { PROVOST, HALL_MANAGER, MEAL_MANAGER, WARD_BOY, GARDENER, STU_ROLE };
enum StuType { RESIDENT, NON_RESIDENT };
inline string roleStr(Role r) {
    const char* s[] = {"Provost","Hall Mgr","Meal Mgr","Ward Boy","Gardener","Student"};
    return s[r];
}

// ===== ABSTRACT BASE CLASS (Default/Param/Copy Constructors, Virtual Destructor) =====
class Person : public IDisplayable {
protected: string name, id, pass; Role role;
public:
    Person(string n="", string i="", string p="1234", Role r=STU_ROLE)
        : name(n), id(i), pass(p), role(r) {}                                      // Default+Param
    Person(const Person& o) : name(o.name), id(o.id), pass(o.pass), role(o.role) {} // Copy
    virtual ~Person() {}                                                             // Destructor
    string getName() const { return name; } string getId() const { return id; }
    string getPass() const { return pass; } Role getRole() const { return role; }
    // OPERATOR OVERLOADING
    bool operator==(const Person& o) const { return id == o.id; }
    friend ostream& operator<<(ostream& os, const Person& p) { p.display(); return os; }
    virtual string info() const = 0;   // PURE VIRTUAL
    virtual double dues() const = 0;   // PURE VIRTUAL
};

// ===== STUDENT: Public Inheritance (Hierarchical) =====
class Student : public Person {
    StuType stype; int room, mCnt, gCnt; map<string,bool> meals; // STL MAP
public:
    Student(string n="", string i="", string p="", StuType t=RESIDENT, int r=0)
        : Person(n,i,p,STU_ROLE), stype(t), room(r), mCnt(0), gCnt(0) {}
    Student(const Student& s) : Person(s), stype(s.stype), room(s.room),
        mCnt(s.mCnt), gCnt(s.gCnt), meals(s.meals) {}    // Copy constructor
    ~Student() override {}
    StuType getType() const { return stype; } int getRoom() const { return room; }
    int getMeals() const { return mCnt; } void setRoom(int r) { room = r; }
    void eat() { mCnt++; } void addGuest() { gCnt++; }
    // Meal management
    void mealOff(const string& d) { meals[d] = false; }
    void mealOn(const string& d)  { meals[d] = true; }
    bool isMealOn(const string& d) const { auto it=meals.find(d); return it==meals.end()||it->second; }
    void viewCalendar() const {
        cout << YEL << "  OFF dates: " << RST; bool a = false;
        for (auto it=meals.begin(); it!=meals.end(); ++it)
            if (!it->second) { cout << RED << it->first << " " << RST; a = true; }
        cout << (a ? "" : GRN+"None"+RST) << "\n";
    }
    Student operator+(int m) const { Student s(*this); s.mCnt+=m; return s; } // Op +
    operator int() const { return mCnt + gCnt; }                               // Type conversion
    // VIRTUAL FUNCTION OVERRIDING
    string info() const override { return stype==RESIDENT ? "Resident" : "Non-Resident"; }
    double dues() const override { return mCnt*50.0 + gCnt*100.0 + (stype==RESIDENT?5000:0); }
    void display() const override {
        cout << "  " << CYN << BLD << name << RST << " | " << id << " | " << MAG << info() << RST
             << " | Rm:" << room << " | M:" << GRN << mCnt << RST << " G:" << YEL << gCnt
             << RST << " | " << BLD << "Tk." << dues() << RST << "\n";
    }
};

// ===== STAFF: Public Inheritance =====
class Staff : public Person {
    double salary;
public:
    Staff(string n="", string i="", string p="", Role r=PROVOST, double s=0) : Person(n,i,p,r), salary(s) {}
    ~Staff() override {}
    string info() const override { return roleStr(role); }
    double dues() const override { return 0; }
    void display() const override {
        cout << "  " << CYN << BLD << name << RST << " | " << id << " | "
             << MAG << info() << RST << " | " << GRN << "Tk." << salary << RST << "\n";
    }
};

// ===== ROOM: Composition =====
class Room {
    int no, cap; vector<string> occ;
public:
    Room(int n=0, int c=2) : no(n), cap(c) {}
    int getNo() const { return no; } bool full() const { return (int)occ.size()>=cap; }
    bool add(const string& s) { if (full()) return false; occ.push_back(s); return true; }
    void display() const {
        cout << "  Rm " << BLD << no << RST << " [" << (full()?RED:GRN) << occ.size() << "/" << cap << RST << "]\n";
    }
};

// ===== Complaint: Composition =====
class Complaint {
    string from, txt, date; bool done;
public:
    Complaint(string f, string t, string d) : from(f), txt(t), date(d), done(false) {}
    void resolve() { done = true; }
    void display() const {
        cout << "  [" << (done?GRN+"DONE":RED+"OPEN") << RST << "] " << BLD << from << RST
             << ": " << txt << " (" << date << ")\n";
    }
};

// ===== FUNCTION TEMPLATE =====
template<typename T> T* findById(vector<T*>& v, const string& id) {
    for (auto* x : v) if (x->getId() == id) return x; 
    return nullptr;
}
// ===== CLASS TEMPLATE =====
template<typename T> class DataList {
    vector<T*> data;
public:
    void add(T* x) { data.push_back(x); }
    bool remove(const string& id) {
        for (auto it = data.begin(); it != data.end(); ++it) {
            if ((*it)->getId() == id) { delete *it; data.erase(it); return true; }
        } return false;
    }
    int size() const { return (int)data.size(); }
    vector<T*>& all() { return data; }
    void show() const { for (auto* x : data) x->display(); }              // FUNCTION OVERLOADING
    void show(Role r) const { for (auto* x : data) if (x->getRole()==r) x->display(); }
    ~DataList() { for (auto* x : data) delete x; }                        // Destructor frees memory
};

// ===== EXCEPTION HIERARCHY (Multi-level: AuthError -> HallError -> exception) =====
class HallError : public exception {
    string msg;
public: HallError(const string& m) : msg(m) {}
    const char* what() const noexcept override { return msg.c_str(); }
};
class AuthError : public HallError { public: AuthError() : HallError("Wrong ID or Password!") {} };
class RoomError : public HallError { public: RoomError() : HallError("Room is full!") {} };
} // namespace Hall
