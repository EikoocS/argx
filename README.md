# argx

A simple command line argument parser library for C++. just a header file, no need to compile anything.

## Features
- No dependencies
- Header only
- Simple to use
- Support for flags, options(multiple to multiple) and arguments
## Usage

Example:

```shell
argx arg0 arg1 -key value -option --flag
```

```cpp
#include "argx.h"

#include <iostream>

using namespace std;

int main( int argc, char** argv ) {
    auto result = argx::parse(argc, argv);
    cout << "Arguments: " << result.argSize() << endl;
    for (int i=0;i<result.argSize();i++) {
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
```
