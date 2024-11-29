# Multithreaded Job Scheduler

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://github.com/AliQ-AFK/multithreaded_job_scheduler/actions)
[![License](https://img.shields.io/badge/license-MIT-blue)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS-orange)](#)

A highly efficient, multithreaded job processing system demonstrating the power of concurrency using multiple synchronization mechanisms, including mutexes and semaphores.

💡 **If you like this project, please [⭐ star the repository](https://github.com/AliQ-AFK/multithreaded_job_scheduler/stargazers) to show your support!**

---

## 📺 Demo

![Job Processing Demo](https://private-user-images.githubusercontent.com/173552557/391160958-7dcb909c-7f7c-4dba-b39b-718576ff490b.gif?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3MzI5MDI5MzAsIm5iZiI6MTczMjkwMjYzMCwicGF0aCI6Ii8xNzM1NTI1NTcvMzkxMTYwOTU4LTdkY2I5MDljLTdmN2MtNGRiYS1iMzliLTcxODU3NmZmNDkwYi5naWY_WC1BbXotQWxnb3JpdGhtPUFXUzQtSE1BQy1TSEEyNTYmWC1BbXotQ3JlZGVudGlhbD1BS0lBVkNPRFlMU0E1M1BRSzRaQSUyRjIwMjQxMTI5JTJGdXMtZWFzdC0xJTJGczMlMkZhd3M0X3JlcXVlc3QmWC1BbXotRGF0ZT0yMDI0MTEyOVQxNzUwMzBaJlgtQW16LUV4cGlyZXM9MzAwJlgtQW16LVNpZ25hdHVyZT00ZThjZmQyYjUyYjkzOWU2OWM0YzU1OGZjODQxZGVmOTVlYTE4MzU0YjEzOWY0N2YyY2EzNmQ3ZTIwZTAzZmI3JlgtQW16LVNpZ25lZEhlYWRlcnM9aG9zdCJ9._fmO10RamCfZWP2YB0ZL5yvzy_dyEc_NO9eoCn6dqSA)

### Steps Shown in the Demo:
1. Dynamic job generation and arrival.
2. Concurrent execution using mutex and semaphore synchronization.
3. Color-coded log output for easy monitoring.
4. Performance statistics displayed after execution.

---

## 🚀 Features

### Core Functionality
- **Multiple Synchronization Methods**:
  - Mutex-based execution for strict mutual exclusion.
  - Semaphore-based execution for resource counting.
  - Unsynchronized execution for performance comparison.
- **Job Processing**:
  - Print and scan job simulation.
  - Dynamic job generation and arrival time-based scheduling.
  - Page-based job execution.

### Development Features
- **Comprehensive Sanitizer Integration**:
  - Address Sanitizer for memory errors.
  - Undefined Behavior and Leak Sanitizers.
- **Robust Warning Flags**:
  - Includes `-Wall`, `-Wextra`, `-Werror`, and `-Wformat-security`.
- **Testing Infrastructure**:
  - Unit tests for all components.
  - Memory leak detection.
  - Thread safety validation and stress testing.
  - Performance benchmarking.

---

## 📥 Installation

### Prerequisites
#### Required Packages (Ubuntu/Debian):
```bash
sudo apt update
sudo apt install build-essential libcunit1-dev
```

#### Optional (Docker):
```bash
sudo apt install docker.io
```

### Installation Steps
1. Clone the repository:
   ```bash
   git clone https://github.com/AliQ-AFK/multithreaded_job_scheduler.git
   cd multithreaded_job_scheduler
   ```

2. Build the project:
   ```bash
   make all
   ```

---

## 🚦 Usage

### Run the Program
```bash
make run
```

### Testing Commands
| Command                    | Description                                     |
|----------------------------|-------------------------------------------------|
| `make test-jobs`           | Run job generation tests                        |
| `make test-execution`      | Test execution correctness with all methods     |
| `make test-scheduler`      | Validate scheduler behavior                     |
| `make test-memory`         | Run memory sanitizers for leaks and errors      |
| `make test-threads`        | Test thread safety and concurrency              |
| `make test-threads-stress` | Perform 100 iterations of thread stress testing |
| `make test-threads-valgrind` | Test with Valgrind for detailed thread errors |

---

## ⚠️ Platform Compatibility

### Supported Platforms:
- ✅ **Linux** (Primary platform):
  - Ubuntu (recommended).
  - Debian.
  - Other Linux distributions.
- ⚠️ **macOS** (Not tested, may require modifications).
- ❌ **Windows** (Not supported natively).

#### Windows Users:
1. **Recommended**: Use WSL (Windows Subsystem for Linux):
   ```bash
   wsl --install
   wsl --install -d Ubuntu
   ```
2. **Alternative**: Use a Linux virtual machine.

---

## 🛠️ Build Configurations

### Development Build (Default)
```bash
make all
```
Includes:
- All sanitizers.
- Debug symbols.
- Maximum warning levels.
- Thread safety checks.

### Production Build
```bash
make prod
```
Includes:
- Optimization flags (`-O2`).
- Minimal runtime overhead.
- Production-ready performance.

---

## 📊 Project Structure
```
.
├── src/                    # Source files
│   ├── execution.c         # Core execution logic
│   ├── job_generation.c    # Job creation and management
│   ├── mutex_execution.c   # Mutex-based synchronization
│   ├── semaphore_execution.c # Semaphore-based synchronization
│   └── unsync_execution.c  # Unsynchronized execution
├── include/               # Header files
│   ├── execution.h        # Execution definitions
│   ├── job.h              # Job structure definitions
│   └── utils.h            # Utility functions
├── test/                  # Test suite
│   ├── test_execution.c
│   ├── test_job_generation.c
│   └── test_schedule.c
├── log/                   # Log directory
└── makefile               # Build system
```

---

## 🧹 Cleanup
```bash
# Clean build files
make clean

# Clean log files
make clean-logs

# Clean everything
make clean-all
```

---

## 🔍 Logging System

The project includes a comprehensive logging system with:
- Dedicated log directory for all execution types.
- Color-coded log levels for better readability.
- Thread-specific logging for debugging.
- Automatic cleanup functionality.
- Detailed execution tracking and timing.

---

## 🔒 Thread Safety

Three synchronization mechanisms are implemented:
1. **Mutex-based Execution**:
   - Traditional mutual exclusion.
   - Strict resource access control.
   - Deadlock prevention.

2. **Semaphore-based Execution**:
   - Resource counting mechanism.
   - Multiple access control.
   - Fair scheduling.

3. **Unsynchronized Execution**:
   - No synchronization mechanisms.
   - Used for performance comparison.
   - Demonstrates race conditions.

---

## 📊 Performance Monitoring

Each execution method provides:
- Total jobs processed.
- Print/scan job completion rates.
- Execution time statistics.
- Thread-specific performance data.
- Resource utilization metrics.

---

## 🤝 Contributing

1. Fork the repository.
2. Create your feature branch (`git checkout -b feature/AmazingFeature`).
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`).
4. Push to the branch (`git push origin feature/AmazingFeature`).
5. Open a Pull Request.

---

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 🎯 Future Improvements

### Planned Features:
- **Cross-platform Support**: Extend compatibility to Windows and macOS natively.
- **Dynamic Thread Pooling**: Implement a thread pool to optimize resource utilization for job processing.
