# Security Policy

## Project Overview

This is an **educational CPU Scheduling Simulator** project. It is not a production application and does not handle sensitive data.

## Supported Versions

| Component | Version | Status |
| --------- | ------- | ------ |
| C++ Backend (sched2.exe) | 1.0 | ✅ Current |
| Python GUI (Flet) | 0.80.5 | ✅ Current |

## Architecture & Security Notes

- **C++ Backend**: Compiled with MinGW g++ 13.1.0 (C++17). Uses custom data structures with `malloc/free`. No network access.
- **Python GUI**: Uses Flet 0.80.5. Connects to C++ backend via `subprocess` (local process only, no network).
- **Data Flow**: User input → Python GUI → stdin to sched2.exe → JSON stdout → Python parses and displays.
- **No external dependencies** in C++ (standard headers only: iostream, string, cstring, cstdlib, fstream, iomanip, sstream).

## Memory Management

The C++ code uses `malloc/free` for custom data structures. All allocated memory is freed via `*_destroy()` functions:
- `queue_destroy()`
- `pq_destroy()`
- `list_destroy()`
- `stack_destroy()`

## Reporting Issues

Since this is an educational project, please report any issues via GitHub Issues.
