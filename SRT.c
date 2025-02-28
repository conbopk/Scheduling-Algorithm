#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


// Define Process structure
typedef struct {
    char name[20];
    int arrival_time;
    int burst_time;
    int remaining_time;
    int response_time;
    int waiting_time;
    int turnaround_time;
    int completed_time;
    bool started;
    bool completed;
} Process;


int main() {
    int n, i, j;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Process *processes = (Process*)malloc(n * sizeof(Process));

    //Input
    printf("\nEnter information of the processes:\n");
    for (i = 0; i < n; i++) {
        printf("\nEnter name of process %d: ", i + 1);
        scanf("%s", processes[i].name);
        printf("\nEnter arrival time of process %s: ", processes[i].name);
        scanf("%d", &processes[i].arrival_time);
        printf("\nEnter burst time of process %s: ", processes[i].name);
        scanf("%d", &processes[i].burst_time);

        //Init default values
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].waiting_time = 0;
        processes[i].response_time = -1;            //process chua bat dau
        processes[i].turnaround_time = 0;
        processes[i].completed_time = 0;
        processes[i].started = false;
        processes[i].completed = false;
    }

    int current_time = 0;
    int completed = 0;
    int prev_process = -1;

    //Time line
    int timeline[10000][2];
    int timeline_len = 0;

    // SRT
    while (completed < n) {
        int shortest_remaining_time_index = -1;
        int shortest_remaining_time = __INT_MAX__;

        // Tìm process có remaining time ngắn nhất trong số các process đã đến và chưa hoàn thành
        for(i = 0; i < n; i++) {
            if(processes[i].arrival_time <= current_time && !processes[i].completed) {
                if(processes[i].remaining_time < shortest_remaining_time) {
                  shortest_remaining_time = processes[i].remaining_time;
                  shortest_remaining_time_index = i;
                }
            }
        }

        if(shortest_remaining_time_index == -1) {
            // Không có process nào sẵn sàng, tăng thời gian lên 1
            current_time++;

            // Thêm thời điểm đang rảnh vào timeline
            if(timeline_len == 0 || timeline[timeline_len-1][1] != -1) {
              timeline[timeline_len][0] = current_time - 1;
              timeline[timeline_len][1] = -1;        // -1 đại diện cho CPU đang rảnh
              timeline_len++;
            }

            continue;
        }

        // Nếu process chưa bắt đầu, ghi nhận response time
        if(!processes[shortest_remaining_time_index].started) {
          processes[shortest_remaining_time_index].response_time = current_time - processes[shortest_remaining_time_index].arrival_time;
          processes[shortest_remaining_time_index].started = true;
        }

        // Thêm thời điểm bắt đầu thực thi process vào timeline
        if (prev_process != shortest_remaining_time_index) {
            timeline[timeline_len][0] = current_time;
            timeline[timeline_len][1] = shortest_remaining_time_index;
            timeline_len++;
            prev_process = shortest_remaining_time_index;
        }

        // Thực thi process trong 1 đơn vị thời gian
        processes[shortest_remaining_time_index].remaining_time--;
        current_time++;

        // Kiểm tra nếu process đã hoàn thành
        if(processes[shortest_remaining_time_index].remaining_time == 0) {
          processes[shortest_remaining_time_index].completed = true;
          completed++;

          // Cập nhật completion time
          processes[shortest_remaining_time_index].completed_time = current_time;

          // Tính turnaround time và waiting time
          processes[shortest_remaining_time_index].turnaround_time = processes[shortest_remaining_time_index].completed_time - processes[shortest_remaining_time_index].arrival_time;
          processes[shortest_remaining_time_index].waiting_time = processes[shortest_remaining_time_index].turnaround_time - processes[shortest_remaining_time_index].burst_time;
        }
    }

    // Tính trung bình waiting time và turnaround time
    float total_turnaround_time = 0;
    float total_waiting_time = 0;

    for(i = 0; i < n; i++) {
      total_turnaround_time += processes[i].turnaround_time;
      total_waiting_time += processes[i].waiting_time;
    }

    float avg_turnaround_time = total_turnaround_time / n;
    float avg_waiting_time = total_waiting_time / n;

    //Output
    printf("\n%-10s %-15s %-15s %-15s\n", "Processes", "Response Time", "Waiting Time", "Turnaround Time");
    printf("----------------------------------------------------\n");

    for(i = 0; i < n; i++) {
         printf("%-10s %-15d %-15d %-15d\n",
             processes[i].name, processes[i].response_time, processes[i].waiting_time, processes[i].turnaround_time);
    }

    printf("\nAverage Turnaround Time: %.2f\n", avg_turnaround_time);
    printf("Average Waiting Time: %.2f\n", avg_waiting_time);

    //Timeline
    printf("\nTimeline:\n");
    for(i = 0; i < timeline_len; i++) {
      if(timeline[i][1] == -1) {
        printf("Time %d: CPU Idle\n", timeline[i][0]);
      } else {
        printf("Time %d: Process %s\n", timeline[i][0], processes[timeline[i][1]].name);
      }
    }

    free(processes);

    return 0;
}

