MergIn
======
Concatenate streams to standard output by lines.

Build
-----
    cmake . && cmake --build .

Usage
-----
    echo B | mergin <(echo A) - <(echo C)
