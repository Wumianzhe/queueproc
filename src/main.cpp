#include <iostream>
#include "message.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Input file is required as a command line parameter";
        return 1;
    }
    string filename = argv[1];
    cout << filename << endl;
    string line = "08:48 1 client1";
    message m = parse(line);
    cout << m.str();
    return 0;
}
