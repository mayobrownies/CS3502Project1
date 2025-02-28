# CS3502 Project 1: Threading and IPC

## Repository Structure
- `src/` - Source code files
  - `ProjectA.cpp` - Threading implementation
  - `ProjectB.cpp` - IPC implementation
- `docs/` - Documentation
  - `main.tex` - LaTeX source for the project report
  - `references.bib` - Bibliography references

## Overview
This project aims to explore and implement fundamental operating systems concepts such as multi-threading, deadlock, synchronization, interprocess communication, linux, etc.
1. **ProjectA**: A multi-threaded banking system demonstrating thread synchronization and deadlock handling
2. **ProjectB**: An inter-process communication system for ranking files by size using pipes

## Part A: Threading Implementation

### Features
- Thread creation and management
- Mutex-based synchronization
- Deadlock demonstration and prevention
- Race condition avoidance

### Components
- `BankAccount` class with thread-safe operations
- Multiple test scenarios
- Timeout-based deadlock prevention

### Test Scenarios
1. **Program()**: Basic transaction processing with multiple accounts
2. **ConcurrencyTest()**: Tests thread safety during concurrent withdrawals
3. **SynchronizationTest()**: Potential deadlock and its prevention
4. **StressTest()**: Performance testing under heavy concurrent load

## Part B: IPC Implementation

### Features
- Process creation using fork()
- Pipe-based communication between processes
- Error handling for system calls
- Data processing across processes

### Components
- Parent process: Executes `ls -l` command and sends output through pipe
- Child process: Receives directory data, sorts files by size, and displays results
- Detailed logging of inter-process communication

## Dependencies

- For Ubuntu terminal, enable WSL:
```bash
wsl --install
```
then install Ubuntu from Microsoft Store

- Install GCC compiler and other dependencies:
```bash
sudo apt update
sudo apt install build-essential
g++ --version
```

- headers for project A:
```bash
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <functional>
#include <sstream>
```

- headers for project B:
```bash
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <chrono>
#include <sstream>
#include <iomanip>
```

headers provide necessary functions and classes for projects including:
- input/output stream
- thread management
- synchronization
- POSIX calls
- timing
- formatting
- etc.


## Compilation and Execution

### Compiling the Projects

This project is compiled using GCC c++ compiler in Ubuntu terminal.

```bash
g++ -o projA ProjectA.cpp -pthread
./projA
```

```bash
g++ -o projB ProjectB.cpp
./projB
```

Sample output of test scenarios including concurrency test, synchronization test, and stress test:

```bash
Concurrency Test
Thread 139892631201600 - Account 1: Created with $1000 (Balance: $1000)
Thread 139892067636928 - Account 1: Withdrawn $100 (Balance: $900)
Thread 139892092815040 - Account 1: Withdrawn $100 (Balance: $800)
Thread 139892076029632 - Account 1: Withdrawn $100 (Balance: $700)
Thread 139892631197376 - Account 1: Withdrawn $100 (Balance: $600)
Thread 139892606019264 - Account 1: Withdrawn $100 (Balance: $500)
Thread 139892614411968 - Account 1: Withdrawn $100 (Balance: $400)
Thread 139892622804672 - Account 1: Withdrawn $100 (Balance: $300)
Thread 139892084422336 - Account 1: Withdrawn $100 (Balance: $200)
Thread 139892597626560 - Account 1: Withdrawn $100 (Balance: $100)
Thread 139892520646336 - Account 1: Withdrawn $100 (Balance: $0)
Thread 139892067636928 - Account 1: Insufficient funds for withdrawal of $100 (Balance: $0)
Thread 139892092815040 - Account 1: Insufficient funds for withdrawal of $100 (Balance: $0)
Thread 139892076029632 - Account 1: Insufficient funds for withdrawal of $100 (Balance: $0)
Thread 139892631197376 - Account 1: Insufficient funds for withdrawal of $100 (Balance: $0)
Thread 139892606019264 - Account 1: Insufficient funds for withdrawal of $100 (Balance: $0)
Thread 139892622804672 - Account 1: Insufficient funds for withdrawal of $100 (Balance: $0)
Thread 139892084422336 - Account 1: Insufficient funds for withdrawal of $100 (Balance: $0)
Thread 139892597626560 - Account 1: Insufficient funds for withdrawal of $100 (Balance: $0)
Thread 139892614411968 - Account 1: Insufficient funds for withdrawal of $100 (Balance: $0)
Thread 139892520646336 - Account 1: Insufficient funds for withdrawal of $100 (Balance: $0)
Thread 139892067636928 - Account 1: Insufficient funds for withdrawal of $100 (Balance: $0)
Thread 139892092815040 - Account 1: Insufficient funds for withdrawal of $100 (Balance: $0)
Thread 139892631197376 - Account 1: Insufficient funds for withdrawal of $100 (Balance: $0)
Thread 139892076029632 - Account 1: Insufficient funds for withdrawal of $100 (Balance: $0)
Thread 139892606019264 - Account 1: Insufficient funds for withdrawal of $100 (Balance: $0)
Thread 139892622804672 - Account 1: Insufficient funds for withdrawal of $100 (Balance: $0)
Thread 139892084422336 - Account 1: Insufficient funds for withdrawal of $100 (Balance: $0)
Thread 139892597626560 - Account 1: Insufficient funds for withdrawal of $100 (Balance: $0)
Thread 139892520646336 - Account 1: Insufficient funds for withdrawal of $100 (Balance: $0)
Thread 139892614411968 - Account 1: Insufficient funds for withdrawal of $100 (Balance: $0)
Final balance after concurrent withdrawals: $0
```

```bash
Synchronization Test
Thread 139892631201600 - Account 1: Created with $500 (Balance: $500)
Thread 139892631201600 - Account 2: Created with $500 (Balance: $500)
Thread 139892520646336 attempting transfer: $300 from Account 1 to Account 2
Thread 139892520646336 completed transfer: $300 from Account 1 to Account 2
Thread 139892597626560 attempting transfer: $200 from Account 2 to Account 1
Thread 139892597626560 completed transfer: $200 from Account 2 to Account 1
Account 1 final balance: $400
Account 2 final balance: $600
```

Stress test with deadlock prevention mechanism shown:

```bash
Stress Test
Thread 139892631201600 - Account 0: Created with $1000 (Balance: $1000)
Thread 139892631201600 - Account 1: Created with $1000 (Balance: $1000)
Thread 139892631201600 - Account 2: Created with $1000 (Balance: $1000)
Thread 139892631201600 - Account 3: Created with $1000 (Balance: $1000)
Thread 139892631201600 - Account 4: Created with $1000 (Balance: $1000)
Thread 139892631201600 - Account 5: Created with $1000 (Balance: $1000)
Thread 139892631201600 - Account 6: Created with $1000 (Balance: $1000)
Thread 139892631201600 - Account 7: Created with $1000 (Balance: $1000)
Thread 139892631201600 - Account 8: Created with $1000 (Balance: $1000)
Thread 139892631201600 - Account 9: Created with $1000 (Balance: $1000)
Thread 139892631201600 - Account 10: Created with $1000 (Balance: $1000)
Thread 139892631201600 - Account 11: Created with $1000 (Balance: $1000)
Thread 139892631201600 - Account 12: Created with $1000 (Balance: $1000)
Thread 139892631201600 - Account 13: Created with $1000 (Balance: $1000)
Thread 139892631201600 - Account 14: Created with $1000 (Balance: $1000)
Thread 139892631201600 - Account 15: Created with $1000 (Balance: $1000)
Thread 139892631201600 - Account 16: Created with $1000 (Balance: $1000)
Thread 139892631201600 - Account 17: Created with $1000 (Balance: $1000)
Thread 139892631201600 - Account 18: Created with $1000 (Balance: $1000)
Thread 139892631201600 - Account 19: Created with $1000 (Balance: $1000)
Thread 139892597626560 attempting transfer: $50 from Account 6 to Account 17
Thread 139892597626560 completed transfer: $50 from Account 6 to Account 17
Thread 139892614411968 attempting transfer: $50 from Account 2 to Account 7
...
Thread 139891043923648 transfer timed out: $50 from Account 5 to Account 3 (DEADLOCK PREVENTED)
Thread 139891236955840 transfer timed out: $50 from Account 5 to Account 9 (DEADLOCK PREVENTED)

Stress test completed in 1.06 seconds
Total accounts with non-zero balance: 19
```

Sample output of IPC, the parent process send the directory data to the child process which sorts files based on size:

```bash
[Parent Process 555] Starting
[Parent Process 555] Pipe created successfully
[Parent Process 555] Created child process 556
[Parent Process 555] Closed read end of pipe
[Parent Process 555] Running 'ls -l'
[Child Process 556] Started
[Child Process 556] Closed write end of pipe
[Child Process 556] Redirected pipe to stdin

[Child Process 556] Started receiving data from pipe
[Parent Process 555] Sent chunk 1 (288 bytes) to pipe
[Parent Process 555] Finished sending data. Total: 288 bytes in 1 chunks
[Parent Process 555] Closed write end of pipe
[Child Process 556] Received line 1: total 208
[Parent Process 555] Waiting for child process...
[Child Process 556] Received line 2: drwxrwxrwx 1 krisp krisp   4096 Feb 28 17:49 CS3502Project1
[Child Process 556] Received line 3: -rwxrwxrwx 1 krisp krisp   9410 Feb 28 17:48 ProjectA.cpp
[Child Process 556] Received line 4: -rwxrwxrwx 1 krisp krisp   7910 Feb 28 17:48 ProjectB.cpp
[Child Process 556] Received line 5: -rwxrwxrwx 1 krisp krisp 116224 Feb 28 16:31 projA
[Child Process 556] Received line 6: -rwxrwxrwx 1 krisp krisp  71288 Feb 28 16:31 projB

[Child Process 556] Finished receiving data. Now Processing

[Child Process 556] Results:
Processed 4 files in 8.074 ms
Total size: 204832 bytes

Files sorted by size:
projA (116224 bytes)
projB (71288 bytes)
ProjectA.cpp (9410 bytes)
ProjectB.cpp (7910 bytes)
[Parent Process 555] Child process exited with status 0
```
