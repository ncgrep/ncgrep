#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "grep.h"

using namespace std;

vector<match_files> match_pattern(string file, string parttern)
{
    int line_max_byte = 102400, line = 0;
    int lastgetp = -2; // the default get position in file is `-1`, in order to distingguish to assign -2
    char buffer[line_max_byte];
    string tmp;
    ifstream in(file);
    vector<match_files> mfv;
    if (!in.is_open())
    {
        cout <<"Open file failed."<<endl;
    }
    while (!in.eof())
    {
        ++line;
        in.getline(buffer, line_max_byte);
        if (lastgetp == in.tellg()) {
            throw runtime_error("Current line " + to_string(line) + " is more than " + to_string(line_max_byte) + " bytes at file " + file);
        } else {
            lastgetp = in.tellg();
        }
        tmp.assign(buffer);
        if (tmp.find(parttern) != tmp.npos) {
            // replace special chars
            tmp = replace_all_distinct(tmp, "	", "    "); // tab -> 4 spaces
            //On Windows, the EOL sequence is ^M^J (CRLF, or '\r\n').
            //On Linux it is just ^J (a single LF, or '\n').
            tmp = replace_all_distinct(tmp, "\r", ""); // \r -> ''
            tmp = replace_all_distinct(tmp, "%", "%%"); // \r -> ''
            match_files mf;
            mf.line = line;
            mf.filename = file;
            mf.content = tmp;
            mfv.push_back(mf);
        }
    }
    in.close();

    return mfv;
}

string replace_all_distinct(string  str, const string old_value, const string new_value)
{
    for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
        if ((pos = str.find(old_value, pos)) != string::npos) {
            str.replace(pos, old_value.length(), new_value);
        } else {
            break;
        }
    }
    return str;
}
