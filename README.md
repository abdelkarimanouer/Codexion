*This project has been created as part of the 42 curriculum by aanouer.*

---

# ⚡ Codexion — Master the race for resources before the deadline masters you

---

## 📖 Description

**Codexion** is a concurrency simulation inspired by the classic **Dining Philosophers Problem** — reimagined in a modern coding environment.

Imagine **N coders** sitting in a circle around a shared **Quantum Compiler**. On the table between each pair of coders lies a **USB dongle**. To compile their quantum code, each coder must grab **two dongles simultaneously** — one from the left, one from the right.

The challenge:
- Every coder must compile regularly or they **burn out**
- Dongles are **shared and limited** — only one coder can use each dongle at a time
- After being released, each dongle has a **cooldown period** before it can be used again
- When multiple coders want the same dongle, a **scheduler** decides who gets it first — either **FIFO** (first come, first served) or **EDF** (most urgent deadline first)
- The simulation stops when **all coders compile enough times** or when **one coder burns out**

The goal of this project is to master **POSIX threads**, **mutexes**, **condition variables**, and **concurrent resource management** in C — without deadlocks, starvation, or data races.

---

## 🛠️ Instructions

### Compilation

```bash
make
```

This compiles the project with `-Wall -Wextra -Werror -pthread` and produces the `codexion` binary.

### Cleaning

```bash
make clean    # remove object files
make fclean   # remove object files and binary
make re       # full recompile
```

### Execution

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

### Arguments

| Argument | Description |
|----------|-------------|
| `number_of_coders` | Number of coders and dongles (must be > 0) |
| `time_to_burnout` | Max ms a coder can go without starting a new compile (>= 0) |
| `time_to_compile` | Time in ms to compile (>= 0) |
| `time_to_debug` | Time in ms to debug (>= 0) |
| `time_to_refactor` | Time in ms to refactor (>= 0) |
| `number_of_compiles_required` | How many compiles each coder must complete (> 0) |
| `dongle_cooldown` | Ms a dongle must rest after being released (>= 0) |
| `scheduler` | Arbitration policy: `fifo` or `edf` |

### Examples

```bash
# 5 coders, normal run — all finish 3 compiles
./codexion 5 800 200 100 100 3 0 fifo

# 2 coders with EDF scheduler and cooldown
./codexion 2 1000 200 100 100 5 50 edf

# Burnout case — compile time exceeds burnout time
./codexion 2 100 200 0 0 5 0 fifo

# Single coder — burns out immediately (needs 2 dongles, only 1 exists)
./codexion 1 800 200 100 100 3 0 fifo
```

### Expected Output Format

```
timestamp_in_ms X has taken a dongle
timestamp_in_ms X is compiling
timestamp_in_ms X is debugging
timestamp_in_ms X is refactoring
timestamp_in_ms X burned out
```

---

## 🚧 Blocking Cases Handled

### 1 — Deadlock Prevention (Coffman's Conditions)

The classic deadlock scenario: every coder grabs their left dongle and waits forever for their right dongle — which is held by their neighbor.

**How we prevent it:** Each coder always picks up their two dongles in a **fixed global order** (lower index first, higher index second). This breaks the **circular wait** condition — one of Coffman's four necessary conditions for deadlock. Since no circular dependency can form, deadlock is impossible.

```c
if (coder->left_dongle < coder->right_dongle)
    { first = left; second = right; }
else
    { first = right; second = left; }
```

### 2 — Starvation Prevention

With FIFO scheduling, starvation can occur for odd numbers of coders under tight timing — this is mathematically expected and not a bug (the subject only guarantees liveness under EDF with feasible parameters).

With **EDF scheduling**, the coder closest to burning out always gets priority. This ensures that under feasible parameters, no coder ever starves.

### 3 — Cooldown Handling

After a dongle is released, it cannot be taken again until `dongle_cooldown` milliseconds have elapsed. Each dongle tracks its `cooldown_end` timestamp. Any coder attempting to take a dongle before the cooldown expires will wait via `pthread_cond_wait` and be re-evaluated when woken by `pthread_cond_broadcast`.

### 4 — Precise Burnout Detection

A dedicated **monitor thread** checks every coder's `last_compile_start` every 1ms. If the time since last compile exceeds `time_to_burnout`, it:
1. Prints the burnout message immediately (before setting stop flag, to ensure the message is never suppressed)
2. Sets `stop = 1`
3. Broadcasts to all dongle condition variables to wake all waiting coders

This guarantees burnout is detected and printed **within 10ms** of the actual burnout time as required by the subject.

### 5 — Log Serialization

All print operations are protected by a single `print_mutex`. Before printing, the thread checks the `stop` flag while holding the mutex. This ensures:
- No two messages ever interleave on a single line
- No message is printed after the simulation has stopped
- The burnout message is printed before `stop` is set, so it is never blocked

---

## 🔒 Thread Synchronization Mechanisms

### pthread_mutex_t — Mutual Exclusion

Several mutexes protect shared resources:

| Mutex | Protects |
|-------|----------|
| `lock_dongle` | Each dongle's state (`is_available`, `cooldown_end`, queue) |
| `print_mutex` | stdout — prevents interleaved log lines |
| `stop_mutex` | The global `stop` flag |
| `ticket_count_mutex` | The global ticket counter for FIFO ordering |
| `lock_l_c_s` | Each coder's `last_compile_start` timestamp |

**Example — race condition prevented on stop flag:**
```c
// Writer (monitor thread)
pthread_mutex_lock(&sim->stop_mutex);
sim->stop = 1;
pthread_mutex_unlock(&sim->stop_mutex);

// Reader (any coder thread)
pthread_mutex_lock(&sim->stop_mutex);
status = sim->stop;
pthread_mutex_unlock(&sim->stop_mutex);
```

Without the mutex, a coder could read a partial or stale value of `stop` — a classic data race.

### pthread_cond_t — Condition Variables

Each dongle has a `pthread_cond_t condition`. Instead of busy-waiting, coders sleep efficiently:

```c
// Coder waits for dongle to become available
pthread_cond_wait(&dongle->condition, &dongle->lock_dongle);

// Another coder releases dongle and wakes all waiters
pthread_cond_broadcast(&dongle->condition);
```

This replaces a CPU-burning `usleep` loop with deep sleep — coders only wake when something actually changes.

### Priority Queue (Heap) — Fair Arbitration

Each dongle maintains a **min-heap priority queue** of requests. When multiple coders request the same dongle:

- **FIFO**: the coder with the lowest `ticket_number` wins (arrival order)
- **EDF**: the coder with the earliest `deadline` wins (`last_compile_start + time_to_burnout`)

Only the winner of the heap is allowed to take the dongle. All others keep sleeping via `pthread_cond_wait`.

### Monitor Thread — Burnout Detection

A dedicated thread runs independently and checks every coder's `last_compile_start` every 1ms. When burnout is detected:

```c
print_action(sim, coder->id, "burned out"); // print FIRST
pthread_mutex_lock(&sim->stop_mutex);
sim->stop = 1;                              // then set stop
pthread_mutex_unlock(&sim->stop_mutex);
broadcast_all_dongles(sim);                 // wake all sleeping coders
```

The order matters — printing before setting `stop` ensures the burnout message is never suppressed by the stop check inside `print_action`.

### Thread-Safe Communication Between Coders and Monitor

Coders and the monitor never share data directly. All communication goes through protected shared state:

- Coders update `last_compile_start` under `lock_l_c_s`
- Monitor reads `last_compile_start` under `lock_l_c_s`
- Monitor sets `stop` under `stop_mutex`
- Coders read `stop` under `stop_mutex`
- Monitor wakes coders via `broadcast_all_dongles`

---

## 📚 Resources

### Classic References

- [PlayList to Learn Threads](https://www.youtube.com/watch?v=d9s_d28yJq0&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2)
- [AI Claude](https://claude.ai/new)
- [Dining Philosophers Problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [POSIX Threads Programming — Lawrence Livermore National Laboratory](https://hpc-tutorials.llnl.gov/posix/)

### AI Usage

**Claude** was used throughout this project for:

- **Understanding concepts** — explaining threads, mutexes, condition variables, deadlock, starvation, and EDF/FIFO scheduling in simple terms.
- **Optimization & Debugging** — AI is used as a supporting tool to assist with identifying complex logic issues and suggesting code optimizations.

All AI-generated content was reviewed, tested, and fully understood before being included in the project.