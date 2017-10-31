#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include <string>
#include <iostream>
#include "iconextractor.h"
using namespace std;

int main(int argc, char* argv[]) {
	if (argc < 2) 
		return -1;

	_setmode(_fileno(stdout), _O_BINARY);
	initialize();

	auto bytes = extract_icon(argv[1]);
	cout.write(bytes.data(), bytes.size());
	cout.flush();
	uninitialize();
	return 0;
}