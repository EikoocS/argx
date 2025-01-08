# argx

A simple command line argument parser library for C++. just a header file, no need to compile anything.

## Features
- No dependencies
- Header only
- Simple to use
- Support for flags, options(multiple to multiple) and arguments
## Install

argx is a header only library, just include the `argx.h` file in your project and you are good to go.

## Usage

argx uses a simple syntax to parse command line arguments.

- Start with `-` will be parsed as an option.
  - After the option, the next argument will be parsed as the value of the option.
  - You can use multiple same options, and it will be stored in a list.
  - You can also use options without values, and it will be stored as an empty list.
- Start with `--` or more `-` will be parsed as a flag.
- Anything else will be parsed as an argument.
- argx with only `-` will be ignored.

For example:

```shell
argx arg0 arg1 -multiple value -multiple value -option value -empty_option --flag
```

result:
```
Arguments:
   [0] : argx
   [1] : arg0
   [2] : arg1
Options:
   empty_option :
   multiple :
      value
      value
   option :
      value
Flags:
   flag

```

## Example

```cpp
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
```
