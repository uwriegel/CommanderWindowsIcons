#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include <string>
#include <iostream>
#include "iconextractor.h"
using namespace std;

#include <Windows.h>

int main(int argc, char* argv[]) {
	if (argc < 2) 
		return -1;

	_setmode(_fileno(stdout), _O_BINARY);
	initialize();
//	while (true) {
		//string input;
		//getline(cin, input);
		//if (input == "exit")
		//	break;
		auto bytes = extract_icon(argv[1]);
		cout.write(bytes.data(), bytes.size());
	//}
	uninitialize();
	return 0;
}