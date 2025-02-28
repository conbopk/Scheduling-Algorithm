#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


//Define process structure
typedef struct {
    char name[20];
    int burst_time;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
} Process;


// Define Gantt chart Entry structure
typedef struct {
    char process_name[20];
    int start_time;
    int stop_time;
} GanttEntry;

int main() {
    int n, quantum, i;

    //Input
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    printf("Enter the quantum time: ");
    scanf("%d", &quantum);

    Process *processes = (Process *) malloc(n * sizeof(Process));

    printf("Enter information for each process:\n");
    for (i = 0; i < n; i++) {
        printf("Enter process name %d:\n", i + 1);
        scanf("%s", processes[i].name);
        printf("Enter process burst time %s:\n", processes[i].name);
        scanf("%d", &processes[i].burst_time);

        //Define default values
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
    }

    // Mảng để lưu Gantt chart
    // Dự trù tối đa số lần context switch là n * (burst_time / quantum + 1)
    int max_entries = 0;
    for (i = 0; i < n; i++) {
        max_entries += (processes[i].burst_time / quantum) + 1;
    }
    GanttEntry *gantt = (GanttEntry *) malloc(max_entries * sizeof(GanttEntry));
    int gantt_size = 0;

    //RR
    int current_time = 0;
    int completed = 0;
    int queue[100]; //ready queue
    int front = 0, rear = n - 1;

    //Khởi tạo hàng đợi ban đầu với tất cả các process (arrival time = 0)
    for (i = 0; i < n; i++) {
        queue[i] = i;
    }

    while (completed < n) {
        // Lấy process từ đầu hàng đợi
        if (front <= rear) {
            int current_process = queue[front++];

            // Nếu process vẫn còn thời gian cần thực thi
            if (processes[current_process].remaining_time > 0) {
                // Thêm vào Gantt chart
                strcpy(gantt[gantt_size].process_name, processes[current_process].name);
                gantt[gantt_size].start_time = current_time;

                // Xác định thời gian thực thi
                int execution_time = (processes[current_process].remaining_time < quantum)
                                         ? processes[current_process].remaining_time
                                         : quantum;

                // Cập nhật remaining time và current time
                processes[current_process].remaining_time -= execution_time;
                current_time += execution_time;

                // Cập nhật thời gian kết thúc trong Gantt chart
                gantt[gantt_size].stop_time = current_time;
                gantt_size++;

                // Cập nhật waiting time cho các process khác đang trong hàng đợi
                for (i = 0; i < n; i++) {
                    if (i != current_process && processes[i].remaining_time > 0) {
                        processes[i].waiting_time += execution_time;
                    }
                }

                // Nếu process chưa hoàn thành, đưa vào cuối hàng đợi
                if (processes[current_process].remaining_time > 0) {
                    queue[++rear] = current_process;
                } else {
                    // Process đã hoàn thành
                    processes[current_process].turnaround_time = current_time;
                    completed++;
                }
            }
        } else {
            // Hàng đợi rỗng, nhưng còn process chưa hoàn thành
            // (Trường hợp này không xảy ra với giả định arrival time = 0)
            current_time++;
        }
    }

    //Tính lại waiting time
    for (i = 0; i < n; i++) {
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
    }

    // Tính trung bình waiting time và turnaround time
    float total_waiting_time = 0;
    float total_turnaround_time = 0;

    for (i = 0; i < n; i++) {
        total_waiting_time += (float)processes[i].waiting_time;
        total_turnaround_time += (float)processes[i].turnaround_time;
    }

    float avg_waiting_time = total_waiting_time / (float)n;
    float avg_turnaround_time = total_turnaround_time / (float)n;

    // Hiển thị Gantt chart
    printf("\nGantt Chart:\n");
    printf("%-10s %-20s %-20s\n", "Process", "Start Time", "Stop Time");
    printf("------------------------------------------------------\n");

    for (i = 0; i < gantt_size; i++) {
        printf("%-10s %-20d %-20d\n",
               gantt[i].process_name,
               gantt[i].start_time,
               gantt[i].stop_time);
    }

    // Visualization Grantt Chart
    // print name process
    printf("|");
    for (i = 0; i < gantt_size; i++) {
        int duration = gantt[i].stop_time - gantt[i].start_time;
        // Tạo chuỗi khoảng trắng để căn giữa tên process
        int padding = (duration * 2 - 1) / 2;

        printf("%*s%s%*s|", padding, "", gantt[i].process_name,
               duration * 2 - 1 - padding - (int) strlen(gantt[i].process_name), "");
    }
    printf("\n");

    // Print Time
    printf("0");
    for (i = 0; i < gantt_size; i++) {
        int duration = gantt[i].stop_time - gantt[i].start_time;
        for (int j = 0; j < duration * 2; j++) {
            printf(" ");
        }
        printf("%d", gantt[i].stop_time);
    }
    printf("\n");

    // Hiển thị average waiting time và average turnaround time
    printf("\nAverage Waiting Time: %.2f\n", avg_waiting_time);
    printf("Average Turnaround Time: %.2f\n", avg_turnaround_time);

    free(processes);
    free(gantt);

    return 0;
}
