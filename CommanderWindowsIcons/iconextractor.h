#pragma once
#include <vector>
#include <string>
using namespace std;

extern void initialize();
extern void uninitialize();
extern const vector<char> extract_icon(const string& icon_path);