#include <iostream>
#include <unordered_map>
#include <string>

// This program generates an mconfig.h file. It is used in the build process.

// Map of variable name to value. Variables are passed via command line and stored
// in this map.
std::unordered_map<std::string, std::string> vars;

char to_hex_digit(int i)
{
    if (i < 10) return i + '0';
    return i - 10 + 'A';
}

// turn a string into a C++-source string, eg:
//     he said "hello"
//  becomes
//    "he said \"hello\""
static std::string stringify(std::string a)
{
    std::string out = "\"";

    for (std::string::size_type i = 0; i < a.length(); i++) {
        char c = a[i];
        if (c == '\n') out += "\\n";
        else if (c == '\t') out += "\\t";
        else if (c == '\"') out += "\\\"";
        else if (c < 0x20) {
            out += "\\x" ;
            out += to_hex_digit((c & 0xF0) >> 4);
            out += to_hex_digit((c & 0x0F));
        }
        else out += c;
    }

    out += "\"";
    return out;
}

// parse a NAME=VALUE argument and store in the variable map
void parse_arg(std::string arg)
{
    auto idx = arg.find("=", 0, 1);
    if (idx == std::string::npos) {
        throw std::string("Couldn't parse argument: ") + arg;
    }

    auto name = arg.substr(0, idx);
    auto value = arg.substr(idx + 1);
    vars.emplace(std::move(name), std::move(value));
}

int main(int argc, char **argv)
{
    for (int i = 1; i < argc; i++) {
        parse_arg(argv[i]);
    }

    using namespace std;
    cout << "// This file is auto-generated by mconfig-gen.cc." << endl;
    cout << "constexpr static char SYSCONTROLSOCKET[] = " << stringify(vars["SYSCONTROLSOCKET"]) << ";" << endl;
    cout << "constexpr static char SBINDIR[] = " << stringify(vars["SBINDIR"]) << ";" << endl;
    return 0;
}
