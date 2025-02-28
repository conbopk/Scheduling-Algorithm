#include <stdio.h>


// Cấu trúc dữ liệu cho một tiến trình
struct Process
{
    int pid;            // Process ID
    int arrival;        // Thời gian đến
    int burst;          // Thời gian thực thi
    int waiting;        // Thời gian chờ
    int turnaround;     // Thời gian hoàn thành
};

// Hàm sắp xếp tiến trình theo thời gian đến (Arrival Time)
void sortByArrival(struct Process p[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (p[j].arrival > p[j + 1].arrival)
            {
                struct Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
};

// Hàm mô phỏng thuật toán FCFS
#include <stdlib.h>

void fcfs(struct Process p[], int n)
{
    int *completionTime = (int *)malloc(n * sizeof(int));
    int totalWaiting = 0;
    int totalTurnaround = 0;

    // Sắp xếp tiến trình theo thời gian đến
    sortByArrival(p, n);

    // Tính toán Completion Time (CT), Waiting Time (WT), Turnaround Time (TAT)
    completionTime[0] = p[0].arrival + p[0].burst;
    p[0].waiting = 0;
    p[0].turnaround = completionTime[0] - p[0].arrival;
    totalTurnaround += p[0].turnaround;

    for (int i = 1; i < n; i++) {
        // Completion Time (CT) = Max (previous CT, Arrival Time) + Burst Time
        if (completionTime[i - 1] < p[i].arrival) {
        	completionTime[i] = p[i].arrival + p[i].burst;
		}
		else {
			completionTime[i] = completionTime[i - 1] + p[i].burst;
		}


		// Waiting Time (WT) = Completion Time - Arrival Time - Burst Time
		p[i].waiting = completionTime[i] - p[i].arrival - p[i].burst;

		// Turnaround Time (TAT) = Completion Time - Arrival Time
		p[i].turnaround = completionTime[i] - p[i].arrival;

		// Cộng tổng thời gian để tính trung bình
		totalWaiting += p[i].waiting;
		totalTurnaround += p[i].turnaround;
    }


	printf("\nFCFS Scheduling Algorithm\n");
    printf("------------------------------------------------\n");
    printf("PID  Arrival  Burst  Waiting  Turnaround  Completion\n");
    printf("------------------------------------------------\n");

	for (int i = 0; i < n; i++)
	{
		printf("%-4d %-8d %-6d %-8d %-10d %-10d\n",
				p[i].pid, p[i].arrival, p[i].burst,
				p[i].waiting, p[i].turnaround, completionTime[i]);
	}

	// Tính thời gian trung bình
	printf("\nAverage Waiting Time: %.2f\n", (float)totalWaiting / n);
	printf("Average Turnaround Time: %.2f\n", (float)totalTurnaround / n);

    // Remember to free the allocated memory
    free(completionTime);
};


int main() {
	// Danh sách tiến trình
	struct Process processes[] = {
		{0,0,2,0,0},
		{1,1,3,0,0},
		{2,2,4,0,0},
		// {3,3,6,0,0}
	};

	int n = sizeof(processes) / sizeof(processes[0]);

	fcfs(processes, n);

	return 0;
}

