# CS3502 Project 1: Threading and IPC

## Overview
Explore and implement fundamental operating systems concepts such as multi-threading, deadlock, synchronization, interprocess communication, linux, etc.
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

- GCC c++ compiler installed with MinGW-w64

- For Ubuntu terminal, enable WSL:
```bash
wsl --install
```
then install Ubuntu from Microsoft Store

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

Sample output:

```bash
Thread 140458598192960 - Account 0: Created with $1000 (Balance: $1000)
Thread 140458598192960 - Account 1: Created with $1000 (Balance: $1000)
Thread 140458598192960 - Account 2: Created with $1000 (Balance: $1000)
Thread 140458598192960 - Account 3: Created with $1000 (Balance: $1000)
Thread 140458598192960 - Account 4: Created with $1000 (Balance: $1000)
Thread 140458598192960 - Account 5: Created with $1000 (Balance: $1000)
Thread 140458598192960 - Account 6: Created with $1000 (Balance: $1000)
Thread 140458598192960 - Account 7: Created with $1000 (Balance: $1000)
Thread 140458598192960 - Account 8: Created with $1000 (Balance: $1000)
Thread 140458598192960 - Account 9: Created with $1000 (Balance: $1000)
Thread 140458598188736 - Account 0: Deposited $100 (Balance: $1100)
Thread 140458589796032 - Account 1: Deposited $100 (Balance: $1100)
Thread 140458573010624 - Account 3: Withdrawn $50 (Balance: $950)
Thread 140458564617920 - Account 4: Deposited $100 (Balance: $1100)
Thread 140458556225216 - Account 5: Withdrawn $50 (Balance: $950)
...
```

```bash
[Parent Process 656] Starting
[Parent Process 656] Pipe created successfully
[Parent Process 656] Created child process 657
[Parent Process 656] Closed read end of pipe
[Parent Process 656] Running 'ls -l'
[Child Process 657] Started
[Child Process 657] Closed write end of pipe
[Child Process 657] Redirected pipe to stdin

[Child Process 657] Started receiving data from pipe
[Parent Process 656] Sent chunk 1 (401 bytes) to pipe
...
```
