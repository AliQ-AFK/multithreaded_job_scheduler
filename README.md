# concurrent_job_scheduler

A concurrent job processing system implementing multiple synchronization mechanisms (mutex, semaphore, and unsynchronized) with comprehensive testing and sanitization.

## 📺 Demo

![Job Processing Demo](https://asciinema.org/connect/e88203ab-430f-41ef-9925-fe9c6f6fe79d)

Example output showing:
- Job generation
- Concurrent execution with different synchronization methods
- Colored logging output
- Performance statistics

## ⚠️ Platform Compatibility

### Supported Platforms:
- ✅ **Linux** (Primary platform)
  - Ubuntu (recommended)
  - Debian
  - Other Linux distributions
- ⚠️ **macOS** (Not tested, may require modifications)
- ❌ **Windows** (Not supported natively)

### Windows Users:
1. **Recommended**: Use WSL (Windows Subsystem for Linux)
   ```bash
   # Install WSL from PowerShell (Admin):
   wsl --install
   
   # After installation and restart:
   wsl --install -d Ubuntu
   ```
2. **Alternative**: Use a Linux virtual machine

## 🚀 Features

### Core Functionality
- **Multiple Synchronization Methods**
  - Mutex-based execution for strict mutual exclusion
  - Semaphore-based execution for resource counting
  - Unsynchronized execution for performance comparison

- **Job Processing**
  - Print and scan job simulation
  - Dynamic job generation
  - Arrival time-based scheduling
  - Page-based processing

### Development Features
- **Comprehensive Sanitizer Integration**
  - Address Sanitizer for memory errors
  - Undefined Behavior Sanitizer
  - Leak Sanitizer for memory leaks

- **Robust Warning Flags**
  - -Wall -Wextra -Werror
  - -Wformat=2 -Wformat-security
  - Debug symbols with frame pointers

### Testing Infrastructure
- Unit tests for all components
- Memory leak detection
- Thread safety validation
- Performance benchmarking

## 🛠️ Prerequisites

### Required Packages (Ubuntu/Debian)
bash
Update package list
sudo apt update
Install required packages
sudo apt install build-essential # Includes GCC and Make
sudo apt install libcunit1-dev # CUnit testing framework

## 📥 Installation

1. Clone the repository:
```bash
git clone https://github.com/AliQ-AFK/multithreaded_job_scheduler.git
cd multithreaded_job_scheduler
```

2. Build the project:
```bash
make all
```

## 🚦 Usage

### Main Program
```bash
make run
```

### Testing Commands

#### Basic Tests
```bash
# Run job generation tests
make test-jobs

# Run execution tests
make test-execution

# Run scheduler tests
make test-scheduler
```

#### Advanced Testing
```bash
# Memory testing (Address, UB, and Leak Sanitizers)
make test-memory

# Thread safety testing
make test-threads

# Thread stress testing (100 iterations)
make test-threads-stress

# Valgrind thread testing
make test-threads-valgrind
```

## 🔧 Build Configurations

### Development Build (Default)
```bash
make all
```
Includes:
- All sanitizers
- Debug symbols
- Maximum warning levels
- Thread safety checks

### Production Build
```bash
make prod
```
Includes:
- Optimization flags (-O2)
- Minimal runtime overhead
- Production-ready performance

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
│   ├── job.h             # Job structure definitions
│   └── utils.h           # Utility functions
├── test/                 # Test suite
│   ├── test_execution.c
│   ├── test_job_generation.c
│   └── test_schedule.c
├── log/                  # Log directory
└── makefile              # Build system
```

## 🧹 Cleanup
```bash
# Clean build files
make clean

# Clean log files
make clean-logs

# Clean everything
make clean-all
```

## 🔍 Logging System

The project includes a comprehensive logging system with:
- Dedicated log directory for all execution types
- Color-coded log levels for better readability
- Thread-specific logging for debugging
- Automatic cleanup functionality
- Detailed execution tracking and timing

## 🔒 Thread Safety

Three synchronization mechanisms are implemented:
1. **Mutex-based Execution**
   - Traditional mutual exclusion
   - Strict resource access control
   - Deadlock prevention

2. **Semaphore-based Execution**
   - Resource counting mechanism
   - Multiple access control
   - Fair scheduling

3. **Unsynchronized Execution**
   - No synchronization mechanisms
   - Used for performance comparison
   - Demonstrates race conditions

## 📊 Performance Monitoring

Each execution method provides:
- Total jobs processed
- Print/scan job completion rates
- Execution time statistics
- Thread-specific performance data
- Resource utilization metrics

## 🤝 Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🎯 Future Improvements

- [ ] Add cross-platform support
- [ ] Implement dynamic thread pooling
- [ ] Add network job submission
- [ ] Enhance logging rotation
- [ ] Add GUI interface

## 👥 Author

* **AliQ-AFK** - *Initial work* - [GitHub](https://github.com/AliQ-AFK)# multithreaded_job_scheduler
