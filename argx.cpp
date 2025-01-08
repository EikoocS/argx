#include <iostream>
#include "argx.h"

using namespace std;

int main( int argc, char** argv ) {
    auto result = argx::parse(argc, argv);

    cout << "Arguments: " << result.arg_size() << endl;
    for (int i=0;i<result.arg_size();i++)
        cout << "   [" << i << "] : " << result.argument(i) << endl;

    cout << "Options: " << result.option_size() << endl;
    for (auto& [option, values] : result.options()) {
        cout << "   " << option << " : " << values.size() << endl;
        for (auto& value : values)
            cout << "      " << value << endl;
    }

    cout << "Flags: " << result.flag_size() << endl;
    for (auto& flag : result.flags())
        cout << "   " << flag << endl;
}