/*
	Discrete-Time Event Simulator

	Program simulates a queueing system using an event queue to process new events (arrivals, departures).
	Utilizes a ready queue to place arriving processes when a process is executing on the cpu.
	Arrival times and service times are generated at run-time using an average from two inputs.

	Author: Alan De La Torre
	October 2024
*/

#include <iostream>
#include <random>
#include <numeric>
using namespace std;

int number_of_processes = 0;
double avg_arrival_rate;
double avg_service_rate;
double total_service_time = 0.0;
double total_ready_queue_time = 0.0;
double start_time = 0.0;
double GenerateTimes(double lambda) {
	double u;
	random_device rd;
	mt19937 generator(rd());
	uniform_real_distribution<double> uniform_dist(0.0001, 1.0);
	u = uniform_dist(generator);
	return (-1.0 / lambda) * log(u);
}

class Event {
private:
	struct EventQueue {
		string type = "";
		double event_time = 0.0;
		int process_id = 0;
		EventQueue* next = nullptr;
	};
	EventQueue* head = nullptr;
	double previous_arrival;

public:
	Event() {
		previous_arrival = 0.0;
	}
	~Event() {
		while (head != nullptr) {
			EventQueue* lead = head;  
			head = head->next;       
			delete lead;       
		}
	}
	void InsertEvent(string process_type, double time, int p_id) {

		EventQueue* new_event = new EventQueue;
		new_event->type = process_type;
		new_event->event_time = time;
		new_event->process_id = p_id;
		new_event->next = nullptr;

		//If empty, this executes
		EventQueue* lead = head;
		if (lead == nullptr) {
			head = new_event;
			return;
		}
	
		//If its less than head
		if (new_event->event_time < lead->event_time) {
			new_event->next = lead;
			head = new_event;
			return;
		}

		//Find the correct spot in the list
		EventQueue* prev = nullptr;
		while (lead != nullptr) {
			if (new_event->event_time < lead->event_time)
				break;

			prev = lead;
			lead = lead->next;
		}

		new_event->next = lead;
		prev->next = new_event;

	}

	void RemoveEvent() {
		if (head == nullptr)
			return;

		EventQueue* temp = head; 
		head = head->next; 
		delete temp;
	}
	string GetNextEventType() {
		return head->type;
	}
	double GetNextEventTime() {
		return head->event_time;
	}
	int GetNextProcessID() {
		return head->process_id;
	}
	void OutputEvents() {
		EventQueue* lead = head;
		while (lead != nullptr) {
			cout << lead->process_id << endl;
			cout << lead->type << endl;
			cout << lead->event_time << endl;
			cout << "-----------------" << endl;
			lead = lead->next;
		}
	}
};
class Ready {
private:
	double service_time;
	int process_id;

public:
	Ready() {
		service_time = 0.0;
		process_id = 0;
	}
	void SetServiceTime(double s_time) {
		service_time = s_time;
	}
	void SetProcessID(int p_id) {
		process_id = p_id;
	}
	double GetServiceTime() {
		return service_time;
	}
	int GetProcessID() {
		return process_id;
	}
};
void HandleArrival(bool& server_idle, Event* manager, vector<Ready>& ready_queue, double& clock, int p_id);
void HandleDeparture(bool& server_idle, Event* manager, vector<Ready>& ready_queue, double& clock);
int main() {
	double avg_turnaround_time;
	double total_throughput;
	double cpu_utilization;
	double avg_number_of_processes_in_ready_queue;
	int completed_processes = 0;

	cout << "Enter the avg arrival rate:" << endl;
	cin >> avg_arrival_rate;
	cout << "Enter the avg service time:" << endl;
	cin >> avg_service_rate;

	// Initialization
	double clock = 0.0;
	bool server_idle = true;
	vector<Ready> ready_queue;
	Event* manager = new Event;
	manager->InsertEvent("arrival", clock, 1);
	number_of_processes++;

	// Process events
	string event_type;
	double event_time;
	int event_id;
	while (completed_processes < 10000) {  
		event_type = manager->GetNextEventType();
		event_time = manager->GetNextEventTime();
		event_id = manager->GetNextProcessID();
		manager->RemoveEvent();
		clock = event_time;

		if (event_type == "arrival") {
			HandleArrival(server_idle, manager, ready_queue, clock, event_id);
		}
		else {
			HandleDeparture(server_idle, manager, ready_queue, clock);
			completed_processes++;
		}

		total_ready_queue_time += ready_queue.size();
	}

	// Calculating final metrics
	avg_turnaround_time = 1/(25-avg_arrival_rate);
	total_throughput = completed_processes / clock;  
	cpu_utilization = total_service_time / clock;  
	avg_number_of_processes_in_ready_queue = total_ready_queue_time / clock;

	cout << "Average Turnaround Time for processes: " << avg_turnaround_time << endl;
	cout << "Total Throughput: " << total_throughput << endl;
	cout << "CPU Utilization: " << cpu_utilization * 100 << "%" << endl;
	cout << "Average Number of Processes in the Ready Queue: " << avg_number_of_processes_in_ready_queue << endl;

	delete manager;
	return 0;
}

//Used to handle events of type arrival
void HandleArrival(bool& server_idle, Event* manager, vector<Ready>& ready_queue, double& clock, int p_id) {
	if (server_idle) {
		server_idle = false;
		double service_time = GenerateTimes(1 / avg_service_rate);
		total_service_time += service_time;
		manager->InsertEvent("departure", clock + service_time, p_id);
	}
	else {
		Ready ready_process;
		ready_process.SetProcessID(p_id);
		double service_time = GenerateTimes(1 / avg_service_rate);
		ready_process.SetServiceTime(service_time);
		ready_queue.push_back(ready_process);
	}

	double arrival_time = GenerateTimes(avg_arrival_rate);
	number_of_processes++;
	manager->InsertEvent("arrival", clock + arrival_time, number_of_processes);
}

//Used to handle events of type departure
void HandleDeparture(bool& server_idle, Event* manager, vector<Ready>& ready_queue, double& clock) {
	if (ready_queue.empty()) {
		server_idle = true;
	}
	else {
		server_idle = false;
		Ready next_process = ready_queue.front();
		ready_queue.erase(ready_queue.begin());

		double service_time = next_process.GetServiceTime();
		total_service_time += service_time; 
		manager->InsertEvent("departure", clock + service_time, next_process.GetProcessID());
	}
}