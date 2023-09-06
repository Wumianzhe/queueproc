#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Input file is required as a command line parameter";
        return 1;
    }
    string filename = argv[1];
    cout << filename << endl;
    return 0;
}
