# TreeViewBuilder
A tiny C++ library to help render data as a TreeView string.

## Features
* C++11 compatible, tested with GCC 9.3.0
* No 3rd-party dependency

## Example Build & Run
```
owwlo@owwlo-server:~/repos/TreeViewBuilder$ make
owwlo@owwlo-server:~/repos/TreeViewBuilder$ ./TreeViewBuilder
Style: SIMPLE
|-- 42
|-- 9527
|   |-- foo
|       |-- bar
|       |-- wakaka
|           |-- Catchphrase!
|-- There are three things I love in life:
    |-- Kicking @ss
    |-- TBD
    |-- insert 3rd thing here

Style: EXPAND
+-- 42
+-- 9527
+   +-- foo
+       +-- bar
+       +-- wakaka
+           +-- Catchphrase!
+-- There are three things I love in life:
    +-- Kicking @ss
    +-- TBD
    +-- insert 3rd thing here

Style: ARROW
|-> 42
|-> 9527
|   |-> foo
|       |-> bar
|       |-> wakaka
|           |-> Catchphrase!
|-> There are three things I love in life:
    |-> Kicking @ss
    |-> TBD
    |-> insert 3rd thing here
```

## License
The MIT License (MIT)
Copyright (c) 2021 owwlo
Licensed under the MIT License. See the top-level file LICENSE.