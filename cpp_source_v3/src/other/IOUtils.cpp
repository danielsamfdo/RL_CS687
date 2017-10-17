// Useful I/O tools

#include <stdafx.h>

using namespace std;
using namespace Eigen;

void pause(unsigned int milliseconds) {
#ifdef _MSC_VER
	Sleep(milliseconds);
#else
	usleep(1000*milliseconds);
#endif
}

// Eat any existing "enter" presses so user actually has to hit enter again.
void forceGetchar() {
	cin.ignore(cin.rdbuf()->in_avail());
	getchar();
}

// Check that a statement is true. If it is not, print a useful error message 
// and exit. This is like assert(...), but with the error message included to 
// avoid reproducing cerr << "Error messag stuff" << endl; exit(1); everywhere.
void ensure(const bool & statement, const char * functionName,
	const char * errorDescription) {
#ifdef NDEBUG
	if (statement)
		return;
	cerr << "Error in " << functionName << endl;
	cerr << "Error description: " << errorDescription << endl;
	forceGetchar();
	exit(1);
#endif
}

// Equivalent to ensure(false, ...). Prints a useful error message then exits.
// This function never returns, so it avoids compiler warnings due to control
// paths never returning.
#ifdef _MSC_VER
__declspec(noreturn)
#else
__attribute__((noreturn))
#endif
void exit(const char * functionName, const char * errorDescription) {
	ensure(false, functionName, errorDescription);
	exit(1); // Ensure doesn't always exit, so let the compiler know that it will.
}

// Ask a yes/no question and return true if the answer is yes. The question is
// privded in s.
bool yesNo(const string & s, istream & in, ostream & out) {
	char c;
	out << s << " [y/n]: ";
	cin >> c;
	return (tolower(c) == 'y');
}

// Get next line of .csv and split into tokens. 
vector<string> csv_getNextLineAndSplitIntoTokens(
	istream& str) {
	vector<string> result;
	string line, cell;
	getline(str, line);
	stringstream lineStream(line);
	while (getline(lineStream, cell, ','))
		result.push_back(cell);
	return result;
}

// Eat leading whitespace on a string. The characters that count as whitespace
// should be provided in the variable "whitespace".
string eatWhite(const string & s, const string & whitespace) {
	int strBegin = (int)(s.find_first_not_of(whitespace));
	if (strBegin == (int)string::npos) return ""; // no content
	return s.substr(strBegin, s.length() - strBegin);
}

// For the menu system: ask a question and return the answer. The "..." 
// arguments are numOptions const char * objects that contain the possible 
// answers.
int getSelection(const char * name, int numOptions, ...) {
	va_list vl;
	va_start(vl, numOptions);
	cout << "Select " << name << ":" << endl;
	for (int i = 0; i < numOptions; i++)
		cout << "[" << i << "] " << va_arg(vl, const char*) << endl;
	cout << "Selection: ";
	int result;
	cin >> result;
	getchar();
	return result;
}