#pragma once

// Useful I/O tools

#include <stdafx.h>

void pause(unsigned int milliseconds);

// Eat any existing "enter" presses so user actually has to hit enter again.
void forceGetchar();

// Check that a statement is true. If it is not, print a useful error message 
// and exit. This is like assert(...), but with the error message included to 
// avoid reproducing cerr << "Error messag stuff" << endl; exit(1); everywhere.
void ensure(const bool & statement, const char * functionName,
	const char * errorDescription);

// Equivalent to ensure(false, ...). Prints a useful error message then exits.
// This function never returns, so it avoids compiler warnings due to control
// paths never returning.
#ifdef _MSC_VER
__declspec(noreturn)
#else
__attribute__((noreturn))
#endif
void exit(const char * functionName, const char * errorDescription);

// Ask a yes/no question and return true if the answer is yes. The question is
// privded in s.
bool yesNo(const std::string & s, std::istream & in, std::ostream & out);

// Get next line of .csv and split into tokens. 
std::vector<std::string> csv_getNextLineAndSplitIntoTokens(
	std::istream& str);

// Eat leading whitespace on a string. The characters that count as whitespace
// should be provided in the variable "whitespace".
std::string eatWhite(const std::string & s, const std::string & whitespace = " \t");

// For the menu system: ask a question and return the answer. The "..." 
// arguments are numOptions const char * objects that contain the possible 
// answers.
int getSelection(const char * name, int numOptions, ...);

////////////////////////////////////////////////////////////////////////////////
///// Templated functions, which must be defined in header
////////////////////////////////////////////////////////////////////////////////

// Like std::to_string, but taking precision for real numbers.
template <typename T>
std::string to_string_with_precision(const T & a_value, const int n = 3) {
	std::ostringstream out;
	out << std::setprecision(n) << a_value;
	return out.str();
}