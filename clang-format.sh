#!/bin/bash
# run this from top-level norns directory before committing...
find ws-wrapper/src -regex .*[\.][ch] | xargs clang-format -i
find matron/src -regex .*[\.][ch] | xargs clang-format -i
find crone/src -regex .*[\.][ch] | xargs clang-format -i
find watcher/src -regex .*[\.][ch] | xargs clang-format -i
