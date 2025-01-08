#include "argx.h"

#include <iostream>

using namespace std;

int main( int argc, char** argv ) {
    auto result = argx::parse(argc, argv);
    cout << "Arguments: " << result.arg_size() << endl;
    for (int i=0;i<result.arg_size();i++) {
        cout << "   [" << i << "] : " << result.argument(i) << endl;
    }
    cout << "Options: " << result.options().size() << endl;
    for (auto& [key, values] : result.options()) {
        cout << "   " << key << " : " << values.size() << endl;
        for (auto& value : values) {
            cout << "      " << value << endl;
        }
    }
    cout << "Flags: " << result.flags().size() << endl;
    for (auto& flag : result.flags()) {
        cout << "   " << flag << endl;
    }
}