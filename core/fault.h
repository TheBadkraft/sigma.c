#ifndef FAULT_H
#define FAULT_H

/*
	Pure C - no windows code
	removed any win32/cpp defines, typedefs, etc
*/

// Used for compile-time checking for array sizes. On Windows VC++, you get 
// an "error C2118: negative subscript" error.
#ifndef C_ASSERT
#define C_ASSERT(expr)  {char uname[(expr)?1:-1];uname[0]=0;}
#endif

#define ASSERT() \
	FaultHandler(__FILE__, (unsigned short) __LINE__)

#define ASSERT_TRUE(condition) \
	do {if (!(condition)) FaultHandler(__FILE__, (unsigned short) __LINE__);} while (0)

	/// Handles all software assertions in the system.
	/// @param[in] file - the file name that the software assertion occurred on
	/// @param[in] line - the line number that the software assertion occurred on
	void FaultHandler(const char* file, unsigned short line);

#endif 	// FAULT_H
