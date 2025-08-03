🚀 CFS Scheduler Simulator - Visualizing Linux's Core Scheduling Algorithm
<div align="center"> <img src="https://img.shields.io/badge/C++-17-blue?logo=cplusplus" alt="C++17"> <img src="https://img.shields.io/badge/Platform-macOS-lightgrey?logo=apple" alt="macOS"> <img src="https://img.shields.io/badge/Linux-Compatible-brightgreen?logo=linux" alt="Linux"> <img src="https://img.shields.io/badge/Visualization-ncurses-yellow" alt="ncurses"> <img src="https://img.shields.io/github/license/qwtoe/cfs-scheduler-simulator" alt="License">

</div>

## 🌟 Project Highlights

**CFS Scheduler Simulator** is an interactive visualization tool for Linux's Completely Fair Scheduler (CFS), designed for OS developers and learners. This project brings to life Linux's core scheduling algorithm through an elegant terminal interface, allowing you to **witness scheduling decisions in real-time!**

```bash
# Experience the magic of scheduling algorithms
make && bin/cfs_simulator
```
## 🧠 Why This Project Stands Out

- **First-of-its-kind** user-space CFS visualization with live red-black tree rendering
- **Cross-platform compatibility** - Developed on macOS, runs seamlessly on Linux
- **Interactive learning** - Experiment with scheduling parameters in real-time
- **Research-grade implementation** - Accurate vruntime calculation and weight tables from Linux kernel
- **Minimalist design** - Under 800 lines of core C++ code with zero external dependencies

## 🧩 Core Features

- 🔍 **Real-time Red-Black Tree Visualization**

- **Dynamic rendering** of the CFS scheduler's red-black tree structure
- **Color-coded nodes** (red/black) with ASCII art connections
- **Animated operations** for task insertion, removal, and tree rotations

## 📊 Task Scheduling Dashboard

- Real-time monitoring for each task:
  - **vruntime** (virtual runtime)
  - **Priority** (nice value)
  - **Progress bar** (completion status)
  - **Task type** (CPU-bound/IO-bound)
  - **Scheduling history** (last 10 scheduling decisions)

## 🎮 Interactive Experimentation

- **Dynamic task injection**: Press + to add randomized tasks
- **Parameter tuning**: Adjust time slice (t), priority (p), and weights (w)
- **Scenario presets**: Load predefined test cases (1-9 keys)
- **Pause/resume**: Spacebar to freeze simulation for detailed inspection

## 🛠️ Technical Architecture
!()[/source/architecture.png]


## 🚀 Getting Started

### Prerequisites

- macOS or Linux system
- C++17 compatible compiler (Clang/GCC)
- NCurses library

### Installation
```bash
# macOS installation
brew install ncurses

# Ubuntu/Debian installation
sudo apt-get install libncurses5-dev libncursesw5-dev

# Clone repository
git clone https://github.com/yourusername/cfs-scheduler-simulator.git
cd cfs-scheduler-simulator

# Build project (auto-creates bin directory)
make

# Launch simulator
bin/cfs_simulator
```

### 🕹️ Interactive Controls

|Key|Action|Description|
|---|---|---|
|`+`|Add random task|Creates new CPU/IO bound task|
|`p`|Toggle priority mode|Swaps between CFS and hybrid scheduling|
|`t`|Adjust time slice|Cycles through 10ms, 25ms, 50ms values|
|`1-9`|Load scenario preset|Predefined test cases|
|SPACE|Pause/resume simulation|Freeze current state for inspection|
|`q`|Quit application|Exit simulator|





## 📚 Learning Resources

### CFS Algorithm Deep Dive

1. **vruntime Calculation**:
    
```
\text{vruntime}_i = \frac{\text{execution time} \times \text{NICE_0_LOAD}}{\text{task weight}}
```
    
2. **Priority to Weight Mapping** (from Linux kernel):
    
```c
static const int prio_to_weight[40] = {
    /* -20 */ 88761, 71755, 56483, 46273, 36291,
    /* -15 */ 29154, 23254, 18705, 14949, 11916,
    /* ... */
};
```
    
3. **Red-Black Tree Operations**:
   - O(log n) time complexity for insertion/deletion        
   - Automatic tree balancing after modifications
   - Left-leaning red-black tree variant for efficiency

### 🔬 Experimental Scenarios

Explore these predefined scenarios (press `1-9` during simulation):

|Key|Scenario|Learning Objective|
|---|---|---|
|1|CPU-bound dominance|Observe vruntime accumulation|
|2|IO-bound burstiness|See how I/O tasks get prioritization|
|3|Priority inversion|Test hybrid scheduler intervention|
|4|Mixed workload|Study fairness in diverse loads|
|5|Real-time task simulation|Experience deadline scheduling|


## 🧪 Advanced Customization

Create custom task scenarios by modifying `src/simulator.cpp`:

```cpp
// Custom task creation
scheduler.add_task(new Task(
    101,                    // Unique ID
    TaskType::CPU_BOUND,     // Task type (CPU_BOUND/IO_BOUND)
    0,                       // Nice value (-20 to 19)
    3000,                    // Total duration (ms)
    1.5                      // CPU intensity multiplier (optional)
));
// Hybrid scheduling experiment
scheduler.set_scheduling_mode(HYBRID_CFS_RT);
```

## 📈 Performance Analysis Toolkit

Generate detailed scheduling reports using our Python analytics module:

```bash
# Run simulator with logging
bin/cfs_simulator --log=detailed > scheduler.log

# enerate interactive report
python3 analysis/report_generator.py scheduler.log
```

**Report includes**:

- Vruntime growth comparison charts
- CPU utilization heatmaps
- Context switch frequency analysis
- Scheduling latency distribution
- Fairness deviation metrics (using standard deviation)

---

Making the invisible visible - one vruntime at a time! ✨
Experience Linux's scheduling magic from the comfort of your terminal.
