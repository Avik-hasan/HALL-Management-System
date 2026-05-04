// color.h - ANSI Color Constants & UI Helpers
#pragma once
#include <iostream>
#include <string>
using namespace std;

// ANSI Color Codes (work on Windows 10+ Terminal)
const string RST = "\033[0m";    // Reset
const string RED = "\033[91m";   // Bright Red
const string GRN = "\033[92m";   // Bright Green
const string YEL = "\033[93m";   // Bright Yellow
const string BLU = "\033[94m";   // Bright Blue
const string MAG = "\033[95m";   // Bright Magenta
const string CYN = "\033[96m";   // Bright Cyan
const string BLD = "\033[1m";    // Bold
const string DIM = "\033[2m";    // Dim

#include <cstdlib>
inline void cls()    { system("cls||clear"); }
inline void line()   { cout << CYN << "  ========================================" << RST << "\n"; }
inline void header(const string& t) { line(); cout << BLD << CYN << "    " << t << RST << "\n"; line(); }
inline void ok(const string& m)   { cout << GRN << "  [*] " << m << RST << "\n"; }
inline void err(const string& m)  { cout << RED << "  [X] " << m << RST << "\n"; }
inline void warn(const string& m) { cout << YEL << "  [!] " << m << RST << "\n"; }
inline void ask(const string& m)  { cout << YEL << "  " << m << RST; }
inline void askW(const string& m) { cout << "\n  " << BLD << m << RST; }
