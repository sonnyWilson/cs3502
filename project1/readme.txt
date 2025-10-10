# CS 3502 Operating Systems Project 1 - Multi-Threaded Banking System

This repository contains four C programs that implement a multi-threaded banking system to demonstrate key operating system concepts such as threading, synchronization, race conditions, and deadlock handling. The programs use POSIX threads (pthreads) to simulate banking transactions across multiple accounts managed by teller or transfer threads.

## Overview of Phases

### Phase 1: Basic Threads (`phase1.c`)
Demonstrates basic multi-threading with random deposits and withdrawals on shared accounts without synchronization, highlighting race conditions.


### Phase 2: Mutex Protection (`phase2.c`)
Adds mutex locks to prevent race conditions, ensuring thread-safe account updates.


### Phase 3: Create Deadlock (`phase3.c`)
Intentionally creates a deadlock scenario using two threads transferring funds in opposite directions.

### Phase 4: Resolve Deadlock (`phase4.c`)
Resolves the deadlock issue by enforcing a consistent lock order during transfers.


### Notes
- **Phase 3 Behavior**: The program will freeze due to deadlock.
- **Output**: Results vary due to random transaction amounts; run multiple times for different outcomes.