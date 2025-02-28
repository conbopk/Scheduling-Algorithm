#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Define Process struct
typedef struct {
    char name[20];
    int arrival_time;
    int burst_time;
    int waiting_time;
    int response_time;
    int turnaround_time;
    int completion_time;
    bool started;
    bool completed;
} Process;

int main() {
    int n, i, j;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    //define processes
    Process *processes = (Process *)malloc(n * sizeof(Process));

    //input
    printf("\nEnter information for each process:\n");
    for (i = 0; i < n; i++) {
        printf("Enter name of process %d: ", i + 1);
        scanf("%s", processes[i].name);
        printf("Enter arrival time of process %s: ", processes[i].name);
        scanf("%d", &processes[i].arrival_time);
        printf("Enter burst time of process %s: ", processes[i].name);
        scanf("%d", &processes[i].burst_time);

        //Init default values
        processes[i].waiting_time = 0;
        processes[i].response_time = 0;
        processes[i].turnaround_time = 0;
        processes[i].completion_time = 0;
        processes[i].started = false;
        processes[i].completed = false;
    }

    // Sắp xếp các process theo thời gian đến (arrival time)
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (processes[j].arrival_time > processes[j + 1].arrival_time ) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    int current_time = 0;
    int completed = 0;
    float total_waiting_time = 0;
    float total_turnaround_time = 0;

    //SJF algorithm
    while (completed < n) {
        int shortest_job_index = -1;
        int shortest_burst_time = __INT_MAX__;

        // Tìm process có burst time ngắn nhất trong số các process đã đến và chưa hoàn thành
        for (i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && !processes[i].completed) {
                if (processes[i].burst_time < shortest_burst_time) {
                    shortest_burst_time = processes[i].burst_time;
                    shortest_job_index = i;
                }
            }
        }

        if (shortest_job_index == -1) {
            // Không có process nào sẵn sàng, tăng thời gian lên 1
            current_time++;
            continue;
        }

        // Nếu process chưa bắt đầu, ghi nhận response time
        if (!processes[shortest_job_index].started) {
            processes[shortest_job_index].response_time = current_time - processes[shortest_job_index].arrival_time;
            processes[shortest_job_index].started = true;
        }

        // Cập nhật thời gian hoàn thành
        current_time += processes[shortest_job_index].burst_time;
        processes[shortest_job_index].completion_time = current_time;

        // Tính turnaround time và waiting time
        processes[shortest_job_index].turnaround_time = processes[shortest_job_index].completion_time - processes[shortest_job_index].arrival_time;
        processes[shortest_job_index].waiting_time = processes[shortest_job_index].turnaround_time - processes[shortest_job_index].burst_time;

        // Cập nhật tổng waiting time và turnaround time
        total_waiting_time += (float)processes[shortest_job_index].waiting_time;
        total_turnaround_time += (float)processes[shortest_job_index].turnaround_time;

        processes[shortest_job_index].completed = true;
        completed++;
    }

    // Tính trung bình waiting time và turnaround time
    float avg_waiting_time = total_waiting_time / (float)n;
    float avg_turnaround_time = total_turnaround_time / (float)n;

    //output
    printf("\n%-10s %-15s %-15s %-15s\n", "Proces", "Response Time", "Waiting Time", "Turnaround Time");
    printf("--------------------------------------------------------\n");

    for (i = 0; i < n; i++) {
        printf("%-10s %-15d %-15d %-15d\n",
            processes[i].name, processes[i].response_time, processes[i].waiting_time, processes[i].turnaround_time);
    }

    printf("\nAverage waiting time: %.2f\n", avg_waiting_time);
    printf("Average turnaround time: %.2f\n", avg_turnaround_time);

    free(processes);
  return 0;
}


