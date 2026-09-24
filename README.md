# Aether Engine 🌠

**Modern C++17 • Embedded Systems • State Machines • Concurrency • Testing**

Aether Engine is a learning and portfolio project focused on modern C++17, deterministic state machines, concurrency, testing, and embedded-oriented software architecture.

## 🚀 About the Project

Aether Engine is a modular C++17 game-control engine inspired by the architecture of embedded gaming machines and hardware-controlled systems.

The project focuses on the software engineering challenges behind this type of system:

- Deterministic state management
- Event-driven architecture
- Thread-safe communication
- Cryptographic hashing and verification
- Hardware abstraction
- Automated testing
- Memory and concurrency analysis
- Cross-platform CMake builds

> _The initial version is intentionally software-focused. Hardware communication and embedded targets are planned as future stages of the project._

## 🎯 Main Goal

The goal of Aether Engine is not to build a production gambling platform. It is to use a realistic systems-oriented project to practice and demonstrate:

```text
Modern C++
      │
      ├── State Machines
      ├── Concurrency
      ├── Cryptography
      ├── Unit Testing
      ├── CMake
      ├── CI/CD
      └── Embedded Architecture
```

## 🧩 Current Status

### 🚧 Early Development

Aether Engine is currently in its initial development stage. The architecture and roadmap are defined, while the implementation is being built incrementally.

**Current Focus:**

- [x] Project skeleton
- [x] CMake configuration
- [ ] Basic FSM implementation
- [ ] GoogleTest integration
- [ ] GitHub Actions CI
- [ ] SHA-256 implementation
- [ ] Deterministic round generation
- [ ] Thread-safe event system
- [ ] Sanitizer configuration

_(The README describes the target architecture, while unchecked roadmap items represent planned work.)_

## 🏗️ Architecture

The project is designed around several independent components.

```text
                     ┌──────────────────────┐
                     │      Aether Engine   │
                     └──────────┬───────────┘
                                │
              ┌─────────────────┼─────────────────┐
              │                 │                 │
              ▼                 ▼                 ▼
       ┌────────────┐    ┌─────────────┐   ┌────────────┐
       │   Game FSM │    │Crypto Engine│   │   Events   │
       └──────┬─────┘    └──────┬──────┘   └─────┬──────┘
              │                 │                 │
              └─────────────────┼─────────────────┘
                                │
                                ▼
                     ┌─────────────────────┐
                     │ Hardware Abstraction│
                     │        Layer        │
                     └─────────┬───────────┘
                               │
                 ┌─────────────┼─────────────┐
                 ▼             ▼             ▼
               UART           I2C           SPI
```

The architecture is deliberately modular so that hardware-specific code can be introduced without coupling it directly to the game logic.

## 🎮 Finite State Machine

The core engine is based on a deterministic finite state machine.

```text
                   INSERT_CREDIT
                         │
                         ▼
                    ┌─────────┐
                    │  IDLE   │
                    └────┬────┘
                         │
                         ▼
                    ┌─────────┐
                    │  ARMED  │
                    └────┬────┘
                         │
                   SPIN_REQUESTED
                         │
                         ▼
                  ┌─────────────┐
                  │  SPINNING   │
                  └──────┬──────┘
                         │
                    SPIN_COMPLETE
                         │
                         ▼
                 ┌──────────────┐
                 │  EVALUATING  │
                 └──────┬───────┘
                        │
                 EVALUATION_COMPLETE
                        │
                        ▼
                   ┌─────────┐
                   │ PAYOUT  │
                   └────┬────┘
                        │
                  PAYOUT_COMPLETE
                        │
                        ▼
                     ┌──────┐
                     │ IDLE │
                     └──────┘
```

### States

| State          | Purpose                                   |
| -------------- | ----------------------------------------- |
| **IDLE**       | Waiting for input or credit               |
| **ARMED**      | Game is ready to start                    |
| **SPINNING**   | Processing the current round              |
| **EVALUATING** | Calculating the round result              |
| **PAYOUT**     | Processing the resulting payout           |
| **ERROR**      | Handling invalid or unexpected conditions |

_The FSM will reject invalid events rather than allowing arbitrary state changes._

## 🔐 Cryptographic Engine

Aether Engine includes a planned deterministic cryptographic subsystem based around SHA-256. The intended model is:

```text
Server Seed
     +
Client Seed
     +
   Nonce
     │
     ▼
┌─────────────┐
│   SHA-256   │
└──────┬──────┘
       │
       ▼
  Round Hash
       │
       ├──────────────► Deterministic Outcome
       │
       └──────────────► Verification
```

The purpose of this component is to explore:

- SHA-256 implementation
- Deterministic hashing
- Reproducible results
- Input/output verification
- Cryptographic testing

> _Note: This project is educational and is not intended to provide a certified or production-ready gambling RNG._

## 🧵 Concurrency

Hardware events will eventually be simulated from a separate thread.

```text
┌───────────────────────────┐
│        Main Thread        │
│                           │
│   Game FSM                │
│   Game Logic              │
│   Crypto                  │
└─────────────┬─────────────┘
              │
              │ Thread-Safe Event Queue
              │
┌─────────────▼─────────────┐
│      Hardware Thread      │
│                           │
│   Credit Events           │
│   Button Events           │
│   Serial Events           │
└───────────────────────────┘
```

Planned synchronization primitives include:

- `std::thread`
- `std::mutex`
- `std::lock_guard`
- `std::condition_variable`

The objective is to understand and demonstrate safe communication between concurrent components.

## 🧪 Testing

Testing is an important part of the project rather than an afterthought. The test suite will use GoogleTest and CTest.

### FSM

Planned tests include:

- Initial state validation
- Valid transitions
- Invalid transitions
- State callback behavior
- Error handling

### Crypto

Planned tests include:

- SHA-256 known test vectors
- Deterministic output
- Round hash consistency
- Outcome reproducibility

### Concurrency

Planned tests include:

- Thread-safe event processing
- Concurrent state access
- Race detection with ThreadSanitizer

## 🤖 Continuous Integration

Every push and pull request targeting the main branch will be checked by GitHub Actions. The CI pipeline will:

```text
Push / Pull Request
        │
        ▼
   Checkout Code
        │
        ▼
 Install Dependencies
        │
        ▼
      CMake
        │
        ▼
      Build
        │
        ▼
    GoogleTest
        │
        ▼
      CTest
        │
        ▼
  Sanitizer Checks
```

This ensures that changes are compiled and tested automatically.
**Workflow:** `.github/workflows/ci.yml`

## 🛠️ Technology Stack

| Technology         | Purpose                             |
| ------------------ | ----------------------------------- |
| **C++17**          | Core language                       |
| **CMake**          | Build system                        |
| **GoogleTest**     | Unit testing                        |
| **CTest**          | Test execution                      |
| **GitHub Actions** | Continuous Integration              |
| **SHA-256**        | Cryptographic hashing               |
| **ASan**           | Memory error detection              |
| **TSan**           | Data race detection                 |
| **Valgrind**       | Memory analysis                     |
| **STL**            | Containers, threading and utilities |

## 💻 Building Locally

### Requirements

- C++17 compatible compiler
- CMake 3.14+
- Git
- GoogleTest

### Supported Toolchains

- GCC
- Clang
- MSVC

### Clone

```bash
git clone https://github.com/javtl/aether-engine.git
cd aether-engine
```

### Configure

```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
```

### Build

```bash
cmake --build build --parallel
```

### Run

```bash
./build/aether_engine
```

## 🧪 Run Tests

After building:

```bash
ctest --test-dir build --output-on-failure
```

Or:

```bash
cd build
ctest --output-on-failure
```

**Example output:**

```text
Test project /aether-engine/build

    Start 1: FsmTest
1/2 Test #1: FsmTest .................... Passed

    Start 2: CryptoTest
2/2 Test #2: CryptoTest ................. Passed

100% tests passed
```

## 🧰 Sanitizers

Aether Engine is intended to support compiler sanitizers during development.

### AddressSanitizer

```bash
cmake -B build \
      -S . \
      -DCMAKE_BUILD_TYPE=Debug \
      -DENABLE_ASAN=ON

cmake --build build

ctest --test-dir build --output-on-failure
```

ASan can help detect problems such as:

- Buffer overflows
- Use-after-free
- Use-after-scope
- Invalid memory access
- Memory leaks (when LeakSanitizer is available)

### ThreadSanitizer

```bash
cmake -B build \
      -S . \
      -DCMAKE_BUILD_TYPE=Debug \
      -DENABLE_TSAN=ON

cmake --build build

ctest --test-dir build --output-on-failure
```

TSan is intended to detect data races in multithreaded code.

## 📁 Project Structure

```text
aether-engine/
│
├── .github/
│   └── workflows/
│       └── ci.yml
│
├── include/
│   ├── Fsm.hpp
│   └── CryptoEngine.hpp
│
├── src/
│   ├── Fsm.cpp
│   ├── CryptoEngine.cpp
│   └── main.cpp
│
├── tests/
│   ├── test_fsm.cpp
│   └── test_crypto.cpp
│
├── docs/
│   ├── ARCHITECTURE.md
│   ├── TESTING.md
│   └── HARDWARE.md
│
├── CMakeLists.txt
├── README.md
└── LICENSE
```

## 🗺️ Roadmap

### Phase 1 — Foundation

- [x] Project structure
- [x] CMake configuration
- [ ] Basic executable
- [ ] FSM implementation
- [ ] GoogleTest integration
- [ ] CTest integration
- [ ] GitHub Actions CI

### Phase 2 — Core Engine

- [ ] Event system
- [ ] State callbacks
- [ ] SHA-256 implementation
- [ ] Deterministic round generation
- [ ] Outcome mapping
- [ ] Error handling

### Phase 3 — Concurrency

- [ ] Hardware event simulation
- [ ] Thread-safe event queue
- [ ] Condition variable integration
- [ ] ThreadSanitizer CI job

### Phase 4 — Embedded Architecture

- [ ] Hardware abstraction layer
- [ ] UART support
- [ ] GPIO abstraction
- [ ] I2C support
- [ ] SPI support
- [ ] Raspberry Pi target

### Phase 5 — Tooling & Observability

- [ ] Structured logging
- [ ] Configuration system
- [ ] Performance benchmarks
- [ ] Static analysis
- [ ] Code coverage
- [ ] Documentation improvements

## 📚 Documentation

Technical documentation will be added as the project evolves.

- `ARCHITECTURE.md` — System architecture
- `TESTING.md` — Testing strategy
- `HARDWARE.md` — Hardware abstraction and integration

## 🎓 What I'm Learning

This project is being developed as part of my journey as a Junior C++ / Embedded Developer. The main areas I'm practicing are:

```text
C++17
 │
 ├── RAII & Smart Pointers
 ├── STL
 ├── Templates
 ├── Concurrency
 ├── Error Handling
 │
 ├── CMake
 ├── Unit Testing
 ├── CI/CD
 │
 ├── Linux
 ├── Networking
 └── Embedded Systems
```

The project will evolve incrementally, with the goal of learning how to design, test, debug, and maintain a systems-oriented C++ codebase.

## 🤝 Contributing

This is primarily a personal learning and portfolio project, but technical suggestions and contributions are welcome.

```bash
git checkout -b feature/my-feature
git add .
git commit -m "feat: add my feature"
git push origin feature/my-feature
```

Then open a Pull Request.

## 📄 License

This project is licensed under the MIT License. See `LICENSE` for details.

## 👨‍💻 Author

**Javier L.**
_Junior C++ / Embedded Developer_
📍 Cádiz, Spain

**Interests:** C++ · Embedded Systems · Firmware · Linux · Networking · Systems Programming

## ⭐ Why Aether Engine?

Aether Engine is a practical project for exploring how modern C++ can be used to build software that sits close to hardware. It combines:

```text
        ┌──────────────┐
        │   Modern C++ │
        └──────┬───────┘
               │
     ┌─────────┼─────────┐
     ▼         ▼         ▼
    FSM    Concurrency  Crypto
     │         │         │
     └─────────┼─────────┘
               ▼
          Unit Testing
               │
               ▼
         GitHub Actions
               │
               ▼
       Embedded Architecture
```

_Built to learn. Designed to scale. Tested continuously._
