"""
CPU Scheduling Algorithms Simulator - GUI
Using Flet Framework
"""

import flet as ft
from typing import List

# ============================================
# Constants
# ============================================
ALGORITHMS = {
    1: "FCFS - First Come First Serve",
    2: "SJF - Shortest Job First",
    3: "SRTF - Shortest Remaining Time",
    4: "Priority (Non-Preemptive)",
    5: "Priority (Preemptive)",
    6: "Round Robin",
}

COLORS = [
    ft.Colors.BLUE_400,
    ft.Colors.GREEN_400,
    ft.Colors.ORANGE_400,
    ft.Colors.PURPLE_400,
    ft.Colors.RED_400,
    ft.Colors.CYAN_400,
    ft.Colors.PINK_400,
    ft.Colors.YELLOW_400,
    ft.Colors.TEAL_400,
    ft.Colors.INDIGO_400,
]


# ============================================
# Process Class
# ============================================
class Process:
    def __init__(self, pid: int, arrival: int, burst: int, priority: int):
        self.id = pid
        self.name = f"P{pid}"
        self.arrival = arrival
        self.burst = burst
        self.priority = priority
        self.color = COLORS[(pid - 1) % len(COLORS)]


# ============================================
# Scheduler Algorithms (Built-in Python)
# ============================================
class Scheduler:
    @staticmethod
    def fcfs(processes: List[Process]) -> dict:
        procs = sorted([{
            "id": p.id, "name": p.name, "arrival": p.arrival,
            "burst": p.burst, "priority": p.priority,
            "remaining": p.burst, "completion": 0, "waiting": 0,
            "turnaround": 0, "response": -1
        } for p in processes], key=lambda x: x["arrival"])
        
        current_time = 0
        execution = []
        
        for p in procs:
            if current_time < p["arrival"]:
                execution.append({"id": -1, "duration": p["arrival"] - current_time})
                current_time = p["arrival"]
            
            p["response"] = current_time - p["arrival"]
            execution.append({"id": p["id"], "duration": p["burst"]})
            current_time += p["burst"]
            p["completion"] = current_time
            p["turnaround"] = p["completion"] - p["arrival"]
            p["waiting"] = p["turnaround"] - p["burst"]
        
        return Scheduler._format(procs, execution, "FCFS")
    
    @staticmethod
    def sjf(processes: List[Process]) -> dict:
        procs = sorted([{
            "id": p.id, "name": p.name, "arrival": p.arrival,
            "burst": p.burst, "priority": p.priority,
            "remaining": p.burst, "completion": 0, "waiting": 0,
            "turnaround": 0, "response": -1
        } for p in processes], key=lambda x: x["arrival"])
        
        n = len(procs)
        current_time = 0
        completed = 0
        done = [False] * n
        execution = []
        
        while completed < n:
            shortest = -1
            min_burst = float('inf')
            
            for i, p in enumerate(procs):
                if not done[i] and p["arrival"] <= current_time and p["burst"] < min_burst:
                    min_burst = p["burst"]
                    shortest = i
            
            if shortest == -1:
                next_arr = min(p["arrival"] for i, p in enumerate(procs) if not done[i])
                execution.append({"id": -1, "duration": next_arr - current_time})
                current_time = next_arr
                continue
            
            p = procs[shortest]
            p["response"] = current_time - p["arrival"]
            execution.append({"id": p["id"], "duration": p["burst"]})
            current_time += p["burst"]
            p["completion"] = current_time
            p["turnaround"] = p["completion"] - p["arrival"]
            p["waiting"] = p["turnaround"] - p["burst"]
            done[shortest] = True
            completed += 1
        
        return Scheduler._format(procs, execution, "SJF")
    
    @staticmethod
    def srtf(processes: List[Process]) -> dict:
        procs = sorted([{
            "id": p.id, "name": p.name, "arrival": p.arrival,
            "burst": p.burst, "priority": p.priority,
            "remaining": p.burst, "completion": 0, "waiting": 0,
            "turnaround": 0, "response": -1
        } for p in processes], key=lambda x: x["arrival"])
        
        n = len(procs)
        current_time = 0
        completed = 0
        timeline = []  # per-unit timeline
        
        while completed < n:
            shortest = -1
            min_rem = float('inf')
            
            for i, p in enumerate(procs):
                if p["arrival"] <= current_time and p["remaining"] > 0 and p["remaining"] < min_rem:
                    min_rem = p["remaining"]
                    shortest = i
            
            if shortest == -1:
                arrivals = [p["arrival"] for p in procs if p["remaining"] > 0]
                if arrivals:
                    next_arr = min(arrivals)
                    for t in range(current_time, next_arr):
                        timeline.append(-1)
                    current_time = next_arr
                continue
            
            p = procs[shortest]
            if p["response"] == -1:
                p["response"] = current_time - p["arrival"]
            
            timeline.append(p["id"])
            p["remaining"] -= 1
            current_time += 1
            
            if p["remaining"] == 0:
                p["completion"] = current_time
                p["turnaround"] = p["completion"] - p["arrival"]
                p["waiting"] = p["turnaround"] - p["burst"]
                completed += 1
        
        # Each time unit as a separate block (like RR style)
        execution = [{"id": pid, "duration": 1} for pid in timeline]
        
        return Scheduler._format(procs, execution, "SRTF")
    
    @staticmethod
    def priority_np(processes: List[Process]) -> dict:
        procs = sorted([{
            "id": p.id, "name": p.name, "arrival": p.arrival,
            "burst": p.burst, "priority": p.priority,
            "remaining": p.burst, "completion": 0, "waiting": 0,
            "turnaround": 0, "response": -1
        } for p in processes], key=lambda x: x["arrival"])
        
        n = len(procs)
        current_time = 0
        completed = 0
        done = [False] * n
        execution = []
        
        while completed < n:
            highest = -1
            min_pri = float('inf')
            
            for i, p in enumerate(procs):
                if not done[i] and p["arrival"] <= current_time and p["priority"] < min_pri:
                    min_pri = p["priority"]
                    highest = i
            
            if highest == -1:
                next_arr = min(p["arrival"] for i, p in enumerate(procs) if not done[i])
                execution.append({"id": -1, "duration": next_arr - current_time})
                current_time = next_arr
                continue
            
            p = procs[highest]
            p["response"] = current_time - p["arrival"]
            execution.append({"id": p["id"], "duration": p["burst"]})
            current_time += p["burst"]
            p["completion"] = current_time
            p["turnaround"] = p["completion"] - p["arrival"]
            p["waiting"] = p["turnaround"] - p["burst"]
            done[highest] = True
            completed += 1
        
        return Scheduler._format(procs, execution, "Priority (NP)")
    
    @staticmethod
    def priority_p(processes: List[Process]) -> dict:
        procs = sorted([{
            "id": p.id, "name": p.name, "arrival": p.arrival,
            "burst": p.burst, "priority": p.priority,
            "remaining": p.burst, "completion": 0, "waiting": 0,
            "turnaround": 0, "response": -1
        } for p in processes], key=lambda x: x["arrival"])
        
        n = len(procs)
        current_time = 0
        completed = 0
        timeline = []  # per-unit timeline
        
        while completed < n:
            highest = -1
            min_pri = float('inf')
            
            for i, p in enumerate(procs):
                if p["arrival"] <= current_time and p["remaining"] > 0 and p["priority"] < min_pri:
                    min_pri = p["priority"]
                    highest = i
            
            if highest == -1:
                arrivals = [p["arrival"] for p in procs if p["remaining"] > 0]
                if arrivals:
                    next_arr = min(arrivals)
                    for t in range(current_time, next_arr):
                        timeline.append(-1)
                    current_time = next_arr
                continue
            
            p = procs[highest]
            if p["response"] == -1:
                p["response"] = current_time - p["arrival"]
            
            timeline.append(p["id"])
            p["remaining"] -= 1
            current_time += 1
            
            if p["remaining"] == 0:
                p["completion"] = current_time
                p["turnaround"] = p["completion"] - p["arrival"]
                p["waiting"] = p["turnaround"] - p["burst"]
                completed += 1
        
        # Each time unit as a separate block (like RR style)
        execution = [{"id": pid, "duration": 1} for pid in timeline]
        
        return Scheduler._format(procs, execution, "Priority (P)")
    
    @staticmethod
    def round_robin(processes: List[Process], quantum: int) -> dict:
        procs = sorted([{
            "id": p.id, "name": p.name, "arrival": p.arrival,
            "burst": p.burst, "priority": p.priority,
            "remaining": p.burst, "completion": 0, "waiting": 0,
            "turnaround": 0, "response": -1
        } for p in processes], key=lambda x: x["arrival"])
        
        n = len(procs)
        current_time = 0
        completed = 0
        queue = []
        idx = 0
        execution = []
        
        while idx < n and procs[idx]["arrival"] <= current_time:
            queue.append(procs[idx])
            idx += 1
        
        while completed < n:
            if not queue:
                if idx < n:
                    execution.append({"id": -1, "duration": procs[idx]["arrival"] - current_time})
                    current_time = procs[idx]["arrival"]
                    while idx < n and procs[idx]["arrival"] <= current_time:
                        queue.append(procs[idx])
                        idx += 1
                continue
            
            p = queue.pop(0)
            if p["response"] == -1:
                p["response"] = current_time - p["arrival"]
            
            exec_time = min(quantum, p["remaining"])
            execution.append({"id": p["id"], "duration": exec_time})
            p["remaining"] -= exec_time
            current_time += exec_time
            
            while idx < n and procs[idx]["arrival"] <= current_time:
                queue.append(procs[idx])
                idx += 1
            
            if p["remaining"] > 0:
                queue.append(p)
            else:
                p["completion"] = current_time
                p["turnaround"] = p["completion"] - p["arrival"]
                p["waiting"] = p["turnaround"] - p["burst"]
                completed += 1
        
        return Scheduler._format(procs, execution, f"Round Robin (Q={quantum})")
    
    @staticmethod
    def _format(procs, execution, name):
        total_burst = sum(p["burst"] for p in procs)
        total_time = sum(e["duration"] for e in execution)
        n = len(procs)
        return {
            "algorithm": name,
            "processes": procs,
            "execution": execution,
            "avg_waiting": sum(p["waiting"] for p in procs) / n,
            "avg_turnaround": sum(p["turnaround"] for p in procs) / n,
            "cpu_util": (total_burst / total_time * 100) if total_time > 0 else 0,
        }
    
    @staticmethod
    def run(processes: List[Process], algo: int, quantum: int = 2):
        if algo == 1: return Scheduler.fcfs(processes)
        elif algo == 2: return Scheduler.sjf(processes)
        elif algo == 3: return Scheduler.srtf(processes)
        elif algo == 4: return Scheduler.priority_np(processes)
        elif algo == 5: return Scheduler.priority_p(processes)
        elif algo == 6: return Scheduler.round_robin(processes, quantum)
        return {}
    
    @staticmethod
    def run_all(processes: List[Process], quantum: int = 2):
        return [Scheduler.run(processes, i, quantum) for i in range(1, 7)]


# ============================================
# GUI Application
# ============================================
def main(page: ft.Page):
    page.title = "CPU Scheduling Simulator"
    page.theme_mode = ft.ThemeMode.DARK
    page.bgcolor = "#1a1a2e"
    page.padding = 20
    page.scroll = ft.ScrollMode.AUTO
    
    # State
    processes: List[Process] = []
    
    # Results container
    results_view = ft.Column(spacing=20)
    
    # Process list view
    process_list = ft.Column(spacing=8)
    
    def get_color(pid: int):
        if pid == -1:
            return ft.Colors.GREY_600
        return COLORS[(pid - 1) % len(COLORS)]
    
    def update_process_list():
        process_list.controls.clear()
        if not processes:
            process_list.controls.append(
                ft.Text("No processes added yet", italic=True, color=ft.Colors.GREY_500)
            )
        else:
            for p in processes:
                process_list.controls.append(
                    ft.Container(
                        content=ft.Row([
                            ft.Container(width=12, height=12, bgcolor=p.color, border_radius=6),
                            ft.Text(p.name, weight=ft.FontWeight.BOLD, width=50),
                            ft.Text(f"Arrival: {p.arrival}", width=100),
                            ft.Text(f"Burst: {p.burst}", width=80),
                            ft.Text(f"Priority: {p.priority}", width=100),
                            ft.IconButton(
                                icon=ft.Icons.DELETE,
                                icon_color=ft.Colors.RED_400,
                                icon_size=18,
                                on_click=lambda e, pid=p.id: delete_process(pid),
                            ),
                        ], spacing=10),
                        padding=10,
                        bgcolor="#2a2a4a",
                        border_radius=8,
                    )
                )
        page.update()
    
    def delete_process(pid: int):
        nonlocal processes
        processes = [p for p in processes if p.id != pid]
        for i, p in enumerate(processes):
            p.id = i + 1
            p.name = f"P{i + 1}"
            p.color = COLORS[i % len(COLORS)]
        update_process_list()
    
    # Input fields
    arrival_input = ft.TextField(label="Arrival Time", width=120, value="0")
    burst_input = ft.TextField(label="Burst Time", width=120, value="1")
    priority_input = ft.TextField(label="Priority", width=120, value="1")
    quantum_input = ft.TextField(label="Quantum", width=100, value="2")
    
    algo_dropdown = ft.Dropdown(
        label="Algorithm",
        width=250,
        options=[ft.dropdown.Option(str(k), v) for k, v in ALGORITHMS.items()] +
                [ft.dropdown.Option("all", "Run All Algorithms")],
        value="1",
    )
    
    def add_process(e):
        try:
            arrival = int(arrival_input.value or 0)
            burst = int(burst_input.value or 1)
            priority = int(priority_input.value or 1)
            
            if burst <= 0:
                show_error("Burst time must be > 0")
                return
            
            pid = len(processes) + 1
            processes.append(Process(pid, arrival, burst, priority))
            arrival_input.value = ""
            burst_input.value = ""
            priority_input.value = ""
            update_process_list()
        except ValueError:
            show_error("Enter valid numbers")
    
    def load_sample(e):
        nonlocal processes
        processes = [
            Process(1, 0, 7, 2),
            Process(2, 1, 4, 1),
            Process(3, 2, 9, 3)
        ]
        update_process_list()
    
    def clear_all(e):
        nonlocal processes
        processes = []
        results_view.controls.clear()
        update_process_list()
    
    def show_error(msg):
        page.snack_bar = ft.SnackBar(ft.Text(msg), bgcolor=ft.Colors.RED_700)
        page.snack_bar.open = True
        page.update()
    
    def create_gantt_chart(result: dict):
        execution = result["execution"]
        items = []
        markers = []
        time = 0
        total = sum(e["duration"] for e in execution)
        
        for e in execution:
            pid, dur = e["id"], e["duration"]
            name = "IDLE" if pid == -1 else f"P{pid}"
            color = get_color(pid)
            width = max(45, (dur / total) * 500)
            
            items.append(
                ft.Container(
                    content=ft.Column([
                        ft.Text(name, size=11, weight=ft.FontWeight.BOLD),
                        ft.Text(str(dur), size=9),
                    ], horizontal_alignment=ft.CrossAxisAlignment.CENTER, spacing=2),
                    width=width,
                    height=45,
                    bgcolor=color,
                    border_radius=6,
                    alignment=ft.Alignment(0, 0),
                )
            )
            markers.append(ft.Container(ft.Text(str(time), size=9), width=width))
            time += dur
        
        markers.append(ft.Text(str(time), size=9))
        
        return ft.Container(
            content=ft.Column([
                ft.Text("Gantt Chart", weight=ft.FontWeight.BOLD, size=13),
                ft.Row(items, scroll=ft.ScrollMode.AUTO, spacing=2),
                ft.Row(markers, scroll=ft.ScrollMode.AUTO),
            ], spacing=5),
            padding=15,
            bgcolor="#252545",
            border_radius=10,
        )
    
    def create_result_card(result: dict):
        procs = result["processes"]
        
        # Table rows
        rows = []
        for p in procs:
            rows.append(ft.DataRow(cells=[
                ft.DataCell(ft.Row([
                    ft.Container(width=10, height=10, bgcolor=get_color(p["id"]), border_radius=5),
                    ft.Text(p["name"]),
                ])),
                ft.DataCell(ft.Text(str(p["arrival"]))),
                ft.DataCell(ft.Text(str(p["burst"]))),
                ft.DataCell(ft.Text(str(p["completion"]))),
                ft.DataCell(ft.Text(str(p["waiting"]))),
                ft.DataCell(ft.Text(str(p["turnaround"]))),
            ]))
        
        table = ft.DataTable(
            columns=[
                ft.DataColumn(ft.Text("Process")),
                ft.DataColumn(ft.Text("Arrival")),
                ft.DataColumn(ft.Text("Burst")),
                ft.DataColumn(ft.Text("Completion")),
                ft.DataColumn(ft.Text("Waiting")),
                ft.DataColumn(ft.Text("Turnaround")),
            ],
            rows=rows,
            border=ft.Border.all(1, "#444"),
            border_radius=8,
        )
        
        stats = ft.Row([
            ft.Container(
                ft.Column([
                    ft.Text("Avg Waiting", size=11),
                    ft.Text(f"{result['avg_waiting']:.2f}", size=18, weight=ft.FontWeight.BOLD),
                ], horizontal_alignment=ft.CrossAxisAlignment.CENTER),
                padding=12, bgcolor="#3b5998", border_radius=10, expand=True,
            ),
            ft.Container(
                ft.Column([
                    ft.Text("Avg Turnaround", size=11),
                    ft.Text(f"{result['avg_turnaround']:.2f}", size=18, weight=ft.FontWeight.BOLD),
                ], horizontal_alignment=ft.CrossAxisAlignment.CENTER),
                padding=12, bgcolor="#2e7d32", border_radius=10, expand=True,
            ),
            ft.Container(
                ft.Column([
                    ft.Text("CPU Util", size=11),
                    ft.Text(f"{result['cpu_util']:.1f}%", size=18, weight=ft.FontWeight.BOLD),
                ], horizontal_alignment=ft.CrossAxisAlignment.CENTER),
                padding=12, bgcolor="#f57c00", border_radius=10, expand=True,
            ),
        ], spacing=10)
        
        return ft.Container(
            content=ft.Column([
                ft.Text(result["algorithm"], size=18, weight=ft.FontWeight.BOLD),
                ft.Divider(color="#444"),
                create_gantt_chart(result),
                ft.Container(height=10),
                table,
                ft.Container(height=10),
                stats,
            ], spacing=10),
            padding=20,
            bgcolor="#1e1e3f",
            border_radius=15,
            border=ft.Border.all(1, "#3a3a6a"),
        )
    
    def create_comparison(results: List[dict]):
        rows = []
        best_wt = min(results, key=lambda x: x["avg_waiting"])
        best_tat = min(results, key=lambda x: x["avg_turnaround"])
        
        for r in results:
            wt_style = ft.FontWeight.BOLD if r == best_wt else None
            tat_style = ft.FontWeight.BOLD if r == best_tat else None
            
            rows.append(ft.DataRow(cells=[
                ft.DataCell(ft.Text(r["algorithm"])),
                ft.DataCell(ft.Text(f"{r['avg_waiting']:.2f}", weight=wt_style,
                           color=ft.Colors.GREEN_400 if r == best_wt else None)),
                ft.DataCell(ft.Text(f"{r['avg_turnaround']:.2f}", weight=tat_style,
                           color=ft.Colors.GREEN_400 if r == best_tat else None)),
                ft.DataCell(ft.Text(f"{r['cpu_util']:.1f}%")),
            ]))
        
        return ft.Container(
            content=ft.Column([
                ft.Text("Comparison", size=18, weight=ft.FontWeight.BOLD),
                ft.DataTable(
                    columns=[
                        ft.DataColumn(ft.Text("Algorithm")),
                        ft.DataColumn(ft.Text("Avg WT")),
                        ft.DataColumn(ft.Text("Avg TAT")),
                        ft.DataColumn(ft.Text("CPU %")),
                    ],
                    rows=rows,
                    border=ft.Border.all(1, "#444"),
                    border_radius=8,
                ),
                ft.Container(height=10),
                ft.Text(f"Best Waiting: {best_wt['algorithm']}", color=ft.Colors.GREEN_400),
                ft.Text(f"Best Turnaround: {best_tat['algorithm']}", color=ft.Colors.GREEN_400),
            ], spacing=10),
            padding=20,
            bgcolor="#1e1e3f",
            border_radius=15,
            border=ft.Border.all(1, "#3a3a6a"),
        )
    
    def run_algorithm(e):
        if not processes:
            show_error("Add processes first!")
            return
        
        results_view.controls.clear()
        algo = algo_dropdown.value
        quantum = int(quantum_input.value or 2)
        
        if algo == "all":
            results = Scheduler.run_all(processes, quantum)
            results_view.controls.append(create_comparison(results))
            for r in results:
                results_view.controls.append(create_result_card(r))
        else:
            result = Scheduler.run(processes, int(algo), quantum)
            results_view.controls.append(create_result_card(result))
        
        page.update()
    
    # Initialize
    update_process_list()
    
    # Build UI
    page.add(
        # Header
        ft.Container(
            ft.Row([
                ft.Icon(ft.Icons.MEMORY, size=35, color=ft.Colors.BLUE_400),
                ft.Text("CPU Scheduling Simulator", size=26, weight=ft.FontWeight.BOLD),
            ], spacing=15),
            margin=ft.Margin.only(bottom=20),
        ),
        
        # Add Process Section
        ft.Container(
            ft.Column([
                ft.Text("Add Process", size=16, weight=ft.FontWeight.BOLD),
                ft.Row([
                    arrival_input, burst_input, priority_input,
                    ft.Button("Add", icon=ft.Icons.ADD, on_click=add_process),
                    ft.Button("Sample", icon=ft.Icons.SCIENCE, on_click=load_sample),
                    ft.Button("Clear", icon=ft.Icons.DELETE, on_click=clear_all,
                              bgcolor=ft.Colors.RED_900),
                ], wrap=True, spacing=10),
            ], spacing=10),
            padding=15,
            bgcolor="#252545",
            border_radius=12,
        ),
        
        ft.Container(height=15),
        
        # Process List
        ft.Container(
            ft.Column([
                ft.Text("Process Queue", size=16, weight=ft.FontWeight.BOLD),
                process_list,
            ], spacing=10),
            padding=15,
            bgcolor="#252545",
            border_radius=12,
        ),
        
        ft.Container(height=15),
        
        # Run Section
        ft.Container(
            ft.Column([
                ft.Text("Run Simulation", size=16, weight=ft.FontWeight.BOLD),
                ft.Row([
                    algo_dropdown,
                    quantum_input,
                    ft.Button("Run", icon=ft.Icons.PLAY_ARROW, on_click=run_algorithm,
                              bgcolor=ft.Colors.GREEN_700, height=45),
                ], wrap=True, spacing=10),
            ], spacing=10),
            padding=15,
            bgcolor="#252545",
            border_radius=12,
        ),
        
        ft.Container(height=20),
        
        # Results
        results_view,
    )


if __name__ == "__main__":
    ft.app(main)
