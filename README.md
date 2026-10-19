# Aether Engine 🌠

**Modern C++17 • Embedded Systems • State Machines • Testing • Cryptography**

Aether Engine is a learning and portfolio project demonstrating modern C++17 practices, deterministic state machines, unit testing, and embedded systems architecture.

## 🚀 About the Project

Aether Engine is a modular C++17 game-control engine designed to explore the software engineering challenges of embedded gaming systems:

- Deterministic finite state machine (FSM) for game lifecycle
- Event-driven architecture
- Cryptographic hashing and verification (SHA-256)
- Thread-safe concurrent architecture
- Comprehensive unit testing with GoogleTest
- Memory and concurrency analysis with sanitizers
- Cross-platform CMake builds

> **Purpose**: This is a learning and portfolio project, not a production gambling platform. It's designed to practice and demonstrate modern C++ in a realistic systems context.

## 📊 Current Status

### ✅ Completed

- [x] Project structure and CMake configuration
- [x] Finite State Machine (FSM) implementation
  - 6 states: IDLE, ARMED, SPINNING, EVALUATING, PAYOUT, ERROR
  - Full transition validation
  - State change callbacks
- [x] GoogleTest integration
- [x] Unit tests (8 tests, 100% passing)
  - State transitions
  - Invalid event rejection
  - Error handling
  - Callback verification
- [x] Compiler warnings configuration (-Wall -Wextra -Wpedantic)
- [x] Git repository with clean history

### 🚧 In Development

- [ ] Stage 3: Quality tools (Sanitizers, RAII, reliable tests)
- [ ] SHA-256 cryptographic engine
- [ ] Provably Fair protocol implementation
- [ ] Thread-safe event system
- [ ] Hardware abstraction layer

### ⏳ Planned

- [ ] GitHub Actions CI/CD pipeline
- [ ] Performance benchmarking
- [ ] Code coverage analysis
- [ ] UART/I2C/SPI abstraction
- [ ] Embedded hardware targets

## 🎮 Finite State Machine

The core component is a deterministic FSM controlling the game lifecycle.

### State Diagram

```
                   START_BET
                       │
                       ▼
                  ┌─────────┐
                  │  IDLE   │
                  └────┬────┘
                       │
                       ▼
                  ┌─────────┐
    ┌────────────→│ BETTING │←────────────┐
    │             └────┬────┘             │
    │                  │                  │
    │           START_SPIN                │
    │                  │                  │
    │                  ▼                  │
    │             ┌─────────┐             │
    │             │SPINNING │             │
    │             └────┬────┘             │
    │                  │                  │
    │            SHOW_RESULT              │
    │                  │                  │
    │                  ▼                  │
    │             ┌─────────┐             │
    │             │ RESULT  │             │
    │             └────┬────┘             │
    │                  │                  │
    │            PAY_PLAYER               │
    │                  │                  │
    │                  ▼                  │
    │             ┌─────────┐             │
    └─────────────│ PAYOUT  │             │
                  └─────────┘             │
                                          │
              TRIGGER_ERROR (any state)   │
                       ▼                  │
                  ┌─────────┐             │
                  │ ERROR   │             │
                  └────┬────┘             │
                       │                  │
                    RESET                 │
                       │                  │
                       └──────────────────┘
```

### States

| State      | Purpose                      |
|------------|------------------------------|
| IDLE       | Waiting for bet placement    |
| BETTING    | Credit accepted, ready       |
| SPINNING   | Round in progress            |
| RESULT     | Calculating outcome          |
| PAYOUT     | Distributing winnings        |
| ERROR      | Fault state (needs RESET)    |

### Key Features

- ✅ Single-threaded by design (no mutex)
- ✅ Invalid events are rejected (return false)
- ✅ TRIGGER_ERROR works from any state
- ✅ ERROR state requires explicit RESET
- ✅ State callbacks for event notification

## 🏗️ Architecture

```
                 ┌─────────────────┐
                 │ Aether Engine   │
                 └────────┬────────┘
                          │
          ┌───────────────┼───────────────┐
          │               │               │
          ▼               ▼               ▼
    ┌──────────┐   ┌────────────┐   ┌─────────┐
    │ Game FSM │   │  Crypto    │   │ Events  │
    └──────────┘   │  Engine    │   └─────────┘
                   └────────────┘
          (Future: Hardware Abstraction Layer)
```

## 🧪 Testing

### Test Suite

8 passing tests covering:

```
✅ Initial state (IDLE)
✅ Complete valid cycle (all 5 transitions)
✅ Invalid event rejection
✅ TRIGGER_ERROR from any state
✅ RESET from ERROR → IDLE
✅ RESET outside ERROR (rejected)
✅ Callback execution on valid transition
✅ Callback not called on invalid transition
```

### Running Tests

After building:

```bash
ctest --test-dir build --output-on-failure
```

Expected output:
```
100% tests passed, 0 tests failed out of 8
```

## 💻 Building & Running

### Requirements

- C++17 compatible compiler (GCC, Clang, MSVC)
- CMake 3.20+
- Git

### Clone

```bash
git clone https://github.com/javtl/aether-engine.git
cd aether-engine
```

### Configure

```bash
cmake -S . -B build
```

### Build

```bash
cmake --build build
```

### Run Demo

```bash
./build/aether_engine
```

Expected output:
```
IDLE -> BETTING
BETTING -> SPINNING
SPINNING -> RESULT
RESULT -> PAYOUT
PAYOUT -> IDLE
```

### Run Tests

```bash
ctest --test-dir build --output-on-failure
```

## 🛠️ Technology Stack

| Technology     | Version | Purpose            |
|----------------|---------|-------------------|
| C++            | 17      | Core language      |
| CMake          | 3.20+   | Build system       |
| GoogleTest     | 1.14.0  | Unit testing       |
| CTest          | 3.20+   | Test execution     |
| GCC/Clang      | Latest  | Compiler           |
| Linux/macOS    | -       | Primary platforms  |

## 📁 Project Structure

```
aether-engine/
├── CMakeLists.txt           # Build configuration
├── README.md                # This file
├── .gitignore
│
├── include/
│   ├── Fsm.hpp              # FSM interface
│   └── Types.hpp            # Shared type definitions
│
├── src/
│   ├── Fsm.cpp              # FSM implementation
│   ├── main.cpp             # Demo executable
│   └── (future: CryptoEngine.cpp)
│
├── tests/
│   └── FsmTests.cpp         # Unit tests
│
└── .git/                    # Git repository
```

## 🎓 Learning Goals

This project demonstrates:

```
Modern C++17
    ├── RAII & smart pointers
    ├── STL containers & algorithms
    ├── Const-correctness
    ├── constexpr & compile-time evaluation
    ├── enum class & scoped enums
    └── std::function & callbacks

Software Engineering
    ├── State machine design
    ├── Unit testing
    ├── CMake build systems
    ├── Version control (Git)
    ├── Code quality (warnings)
    └── Debugging with sanitizers

Embedded Systems (Future)
    ├── Hardware abstraction
    ├── Event-driven I/O
    ├── Concurrency & synchronization
    └── Resource constraints
```

## 🗺️ Development Roadmap

### Stage 1 ✅
- [x] Repository setup
- [x] CMake configuration
- [x] FSM implementation
- [x] Basic tests

### Stage 2 ✅
- [x] Comprehensive test suite
- [x] State validation
- [x] Callback system
- [x] Error handling

### Stage 3 🚧
- [ ] AddressSanitizer integration
- [ ] UndefinedBehaviorSanitizer
- [ ] RAII best practices
- [ ] Reliable test patterns
- [ ] Debugging exercises

### Stage 4 ⏳
- [ ] SHA-256 implementation
- [ ] Cryptographic round generation
- [ ] Deterministic outcome mapping
- [ ] Verification protocol

### Stage 5 ⏳
- [ ] Thread-safe event queue
- [ ] Hardware simulation
- [ ] Concurrent state access
- [ ] ThreadSanitizer integration

### Stage 6 ⏳
- [ ] Hardware abstraction layer
- [ ] UART communication
- [ ] GPIO abstraction
- [ ] Embedded targets

## 🧰 Development Tools

### Compiler Warnings

All builds use strict warning flags:
```bash
-Wall -Wextra -Wpedantic
```

This ensures code quality from the ground up.

### Sanitizers (Stage 3+)

Planned integration:
- **AddressSanitizer**: Memory error detection
- **UndefinedBehaviorSanitizer**: Undefined behavior detection
- **ThreadSanitizer**: Data race detection (Stage 5)

### Code Analysis

Future tools:
- `clang-tidy`: Static analysis
- `cppcheck`: Code quality
- `valgrind`: Memory profiling
- `gcov`/`llvm-cov`: Code coverage

## 📚 Documentation

- `README.md` — Project overview (this file)
- `.github/workflows/` — CI/CD pipeline (planned)
- Stage-specific guides in project docs

## 🤝 Contributing

This is a personal learning project, but technical suggestions are welcome.

For changes:
1. Create a feature branch
2. Make your changes
3. Run tests: `ctest --test-dir build --output-on-failure`
4. Ensure no compiler warnings
5. Open a pull request

## 📄 License

MIT License. See LICENSE file for details.

## 👨‍💻 Author

**Javier L.** — Junior C++ / Embedded Systems Developer

- GitHub: [@javtl](https://github.com/javtl)
- LinkedIn: [profile](https://www.linkedin.com/in/javierlsw)

## 🎯 Why This Project?

Aether Engine bridges theory and practice by building a realistic embedded system in pure C++. It's designed to answer the question:

> **Can I write production-grade C++ code that's safe, tested, and maintainable?**

The answer lives in the code.

---

**Last Updated**: October 2026  
**Status**: Early Development (Stage 2 Complete, Stage 3 In Progress)
