# SmartTV Remote Control

A socket programming exercise for **IDATA2304 – Computer Communication and Network Programming** (Fall 2025).

This project implements a simple Smart TV system with remote controls:
- **Smart TV server** (C++): a TCP server that models the TV logic.
- **Smart Remote client** (Kotlin): a TCP client with a simple CLI/GUI (ImGui-Java optional).
- **Protocol specification**: defined in [`PROTOCOL.md`](./PROTOCOL.md).

The solution evolves in three stages:
1. **Part 1:** Minimal working TV and remote over TCP.
2. **Part 2:** Refactored logic, unit tests, cleaner protocol handling.
3. **Part 3:** Multi-client support with asynchronous channel update notifications.

---

## Getting Started

### Requirements
- **Server:** C++20 compiler, [CMake](https://cmake.org/), [Asio](https://think-async.com/) (or Boost.Asio).
- **Client:** [Kotlin](https://kotlinlang.org/) (JDK 17+ recommended).
- (Optional) [ImGui-Java](https://github.com/SpaiR/imgui-java) for GUI.

### Build & Run
#### Server (C++)
```bash
cd server
cmake -B build
cmake --build build
./build/tv-server [port]
```

*Default Port*: `1238`

#### Client (Kotlin CLI)
```bash
cd client
./gradlew run --args="localhost 1238"
```

---

## Protocol
The application-layer protocol is described in detail in [PROTOCOL.md](PROTOCOL.md).

It defines the commands understood by the TV and the responses returned to clients.

---

## Repository Structure
```bash
.
├── client/       # Kotlin client source
├── server/       # C++ server source
├── tests/        # Unit tests (optional, Part 2)
├── README.md     # Overview & build/run instructions
└── PROTOCOL.md   # Protocol specification
```

---

## License
Mozilla Public License 2.0 (MPL 2.0)
