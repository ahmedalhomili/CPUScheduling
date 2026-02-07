"""
CPU Scheduling Algorithms Simulator - GUI
Using Flet Framework
Connects to C++ backend via subprocess
"""

import warnings
warnings.filterwarnings("ignore", category=DeprecationWarning, module="flet")

import flet as ft
import subprocess
import json
import os
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

# Path to C++ executable (sched2.exe in project root)
CPP_EXE = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "sched2.exe")

# MinGW bin path (needed for DLLs at runtime)
MINGW_BIN = r"F:\Program Files\JetBrains\CLion 2025.2.4\bin\mingw\bin"

def get_cpp_env():
    """Get environment with MinGW in PATH so sched2.exe can find its DLLs"""
    env = os.environ.copy()
    env["PATH"] = MINGW_BIN + ";" + env.get("PATH", "")
    return env


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
# C++ Backend Bridge (subprocess)
# ============================================
def call_cpp_scheduler(processes: List[Process], algo: int, quantum: int = 2) -> dict:
    """Call C++ exe with --json mode and return parsed result"""
    stdin_data = f"{len(processes)}\n"
    for p in processes:
        stdin_data += f"{p.arrival} {p.burst} {p.priority}\n"

    cmd = [CPP_EXE, "--json", "--algo", str(algo), "--quantum", str(quantum)]
    result = subprocess.run(cmd, input=stdin_data, capture_output=True, text=True, timeout=10, env=get_cpp_env())

    if result.returncode != 0:
        raise RuntimeError(f"C++ error: {result.stderr}")

    data = json.loads(result.stdout)
    return convert_cpp_result(data, processes)


def call_cpp_all(processes: List[Process], quantum: int = 2) -> List[dict]:
    """Call C++ exe with --json --all and return all results"""
    stdin_data = f"{len(processes)}\n"
    for p in processes:
        stdin_data += f"{p.arrival} {p.burst} {p.priority}\n"

    cmd = [CPP_EXE, "--json", "--all", "--quantum", str(quantum)]
    result = subprocess.run(cmd, input=stdin_data, capture_output=True, text=True, timeout=10, env=get_cpp_env())

    if result.returncode != 0:
        raise RuntimeError(f"C++ error: {result.stderr}")

    data = json.loads(result.stdout)
    return [convert_cpp_result(r, processes) for r in data]


def convert_cpp_result(data: dict, processes: List[Process]) -> dict:
    """Convert C++ JSON output to GUI display format"""
    procs = []
    for p in data["processes"]:
        procs.append({
            "id": p["id"],
            "name": f"P{p['id']}",
            "arrival": p["arrival"],
            "burst": p["burst"],
            "priority": p["priority"],
            "completion": p["completion"],
            "waiting": p["waiting"],
            "turnaround": p["turnaround"],
        })

    # Build execution list from C++ timeline (start/end pairs)
    execution = []
    for t in data["timeline"]:
        execution.append({
            "id": t["process_id"],
            "start": t["start"],
            "end": t["end"],
            "duration": t["end"] - t["start"],
        })

    return {
        "algorithm": data["algorithm"],
        "processes": procs,
        "execution": execution,
        "avg_waiting": data["avg_waiting_time"],
        "avg_turnaround": data["avg_turnaround_time"],
        "cpu_util": data["cpu_utilization"],
    }


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
        total_time = execution[-1]["end"] if execution else 1

        for e in execution:
            pid, dur = e["id"], e["duration"]
            start = e["start"]
            name = "IDLE" if pid == -1 else f"P{pid}"
            color = get_color(pid)
            width = max(45, (dur / total_time) * 500)

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
            markers.append(ft.Container(ft.Text(str(start), size=9), width=width))

        # Last time marker
        if execution:
            markers.append(ft.Text(str(execution[-1]["end"]), size=9))

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
            border=ft.border.all(1, "#444"),
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
            border=ft.border.all(1, "#3a3a6a"),
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
                    border=ft.border.all(1, "#444"),
                    border_radius=8,
                ),
                ft.Container(height=10),
                ft.Text(f"Best Waiting: {best_wt['algorithm']}", color=ft.Colors.GREEN_400),
                ft.Text(f"Best Turnaround: {best_tat['algorithm']}", color=ft.Colors.GREEN_400),
            ], spacing=10),
            padding=20,
            bgcolor="#1e1e3f",
            border_radius=15,
            border=ft.border.all(1, "#3a3a6a"),
        )

    def run_algorithm(e):
        if not processes:
            show_error("Add processes first!")
            return

        results_view.controls.clear()
        algo = algo_dropdown.value
        quantum = int(quantum_input.value or 2)

        try:
            if algo == "all":
                results = call_cpp_all(processes, quantum)
                results_view.controls.append(create_comparison(results))
                for r in results:
                    results_view.controls.append(create_result_card(r))
            else:
                result = call_cpp_scheduler(processes, int(algo), quantum)
                results_view.controls.append(create_result_card(result))
        except FileNotFoundError:
            show_error(f"C++ exe not found: {CPP_EXE}")
        except json.JSONDecodeError as ex:
            show_error(f"JSON parse error: {ex}")
        except Exception as ex:
            show_error(f"Error: {ex}")

        page.update()

    # Initialize
    update_process_list()

    # Check C++ backend connection
    cpp_exists = os.path.exists(CPP_EXE)
    status_color = ft.Colors.GREEN_400 if cpp_exists else ft.Colors.RED_400
    status_text = "C++ Backend Connected" if cpp_exists else "C++ exe not found!"

    # Build UI
    page.add(
        # Header with connection status
        ft.Container(
            ft.Row([
                ft.Icon(ft.Icons.MEMORY, size=35, color=ft.Colors.BLUE_400),
                ft.Text("CPU Scheduling Simulator", size=26, weight=ft.FontWeight.BOLD),
                ft.Container(expand=True),
                ft.Container(
                    ft.Row([
                        ft.Icon(ft.Icons.CIRCLE, size=10, color=status_color),
                        ft.Text(status_text, size=11, color=status_color),
                    ], spacing=5),
                ),
            ], spacing=15),
            margin=ft.margin.only(bottom=20),
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
