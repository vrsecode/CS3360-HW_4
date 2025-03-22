# Discrete-Time Event Simulation for M/M/1 Queuing System  

## 1. Introduction  
This report presents the results of a discrete-time event simulation for an M/M/1 queuing system representing CPU scheduling under First-Come-First-Served (FCFS) and Shortest Job First (SJF) scheduling policies. The simulator models a system where processes arrive following a Poisson process with varying arrival rates (\(\lambda\)) and are serviced with an exponentially distributed service time with an average of **0.04 seconds**. The objective of this study is to analyze the system's performance across different arrival rates.  

## 2. Simulation Setup  
The simulation was executed with arrival rates ranging from **10 to 30 processes per second** in increments of **2**. Each simulation run continued until **10,000 processes** were completed. The key performance metrics evaluated were:  

- **Average Turnaround Time**: The time a process spends in the system from arrival to completion.  
- **Throughput**: The number of completed processes per second.  
- **CPU Utilization**: The percentage of time the CPU is busy.  
- **Average Number of Processes in the Ready Queue**: A measure of system congestion.  

The simulator was compiled and executed using the following commands:  

```g++ main.cpp
```./a.out

## 3. Results and Analysis  

### **Average Turnaround Time vs. Arrival Rate**  
As expected, the average turnaround time increased as \(\lambda\) increased, with FCFS experiencing a steeper rise compared to SJF. This is because SJF prioritizes shorter jobs, reducing waiting times.  

### **Throughput vs. Arrival Rate**  
Throughput increased with arrival rate but plateaued as the system approached saturation. Both FCFS and SJF achieved similar maximum throughput values, but SJF slightly outperformed FCFS at higher loads due to better job selection.  

### **CPU Utilization vs. Arrival Rate**  
CPU utilization steadily increased with \(\lambda\) and approached **100%** as arrival rates neared system capacity. There was no significant difference between FCFS and SJF in this regard.  

### **Average Number of Processes in the Ready Queue vs. Arrival Rate**  
The ready queue size increased with \(\lambda\), with FCFS exhibiting a more dramatic growth. This confirms that SJF reduces process backlog by prioritizing shorter jobs.  

## 4. Conclusion  
The results align with theoretical expectations of M/M/1 queuing behavior. The SJF scheduler consistently outperformed FCFS in reducing turnaround time and queue size, while throughput and CPU utilization remained similar for both. These findings highlight the benefits of SJF in high-load scenarios.  

## 5. Graphical Representation  
The other file included in this package is the Excel file that contains all the graphs and data from **10 runs** of FCFS and SJF each.  

These plots clearly demonstrate the efficiency of SJF in managing system performance under increasing loads.  
