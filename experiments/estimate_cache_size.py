import matplotlib.pyplot as plt
import pandas as pd
import os

from cyclops import Cyclops, ParamSweep

#BATCH_RUNS = 3
#WARMUP_RUNS = 5
BATCH_RUNS = 3
WARMUP_RUNS = 5

WORKLOAD = "STRIDED_ARRAY"
AGGREGATE = "MEDIAN"

def sweep_array_size_kib(lo: int, hi: int, step: int, repeats: int, metric_grp: str):

    param_sweep = ParamSweep(
        key="array-size-kib",
        low=lo,
        high=hi,
        step=step,
    )

    cyclops = Cyclops(
        workload=WORKLOAD,
        metric_grp=metric_grp,
        warmup_runs=WARMUP_RUNS,
        batch_runs=BATCH_RUNS,
        param_sweep=param_sweep,
        params={
            "repeats": repeats,
        },
    )
    cyclops.exec()

def fig_1_run_experiment():
    sweep_array_size_kib(1, 100, 1, 1000, "L1D_READS")
    os.rename("param_sweep.csv", "fig_1.csv")

def fig_1_make_fig():

    df = pd.read_csv(
        "fig_1.csv",
        comment="#",
        index_col="array-size-kib"
    )

    x = df.index.values
    y = df[f"L1D_READ_MISS_RATE:{AGGREGATE}"].values

    plt.figure()
    plt.plot(x, 100 * y, marker="")
    #plt.xscale("log")
    plt.xlabel("Array size (KiB)")
    plt.ylabel("% L1d cache misses")
    plt.title("% L1d cache misses vs array size")
    plt.grid(True)
    plt.savefig(f"fig_1.png")
    plt.close()

def fig_2_run_experiment(repeats):
    sweep_array_size_kib(1, 100, 1, repeats, "TASK_CLOCK_NS")
    os.rename("param_sweep.csv", "fig_2.csv")

def fig_2_make_fig(repeats):

    df = pd.read_csv(
        "fig_2.csv",
        comment="#",
        index_col="array-size-kib"
    )

    x = df.index.values
    y = df[f"TASK_CLOCK_NS:MEDIAN"].values / (x * 16 * repeats)

    plt.figure()
    plt.plot(x, y, marker="")
    #plt.xscale("log")
    #plt.yscale("log")
    plt.xlabel("Array size (KiB)")
    plt.ylabel("Time (ns)")
    plt.title("Latency jump from L1 to L2 cache")
    plt.grid(True)
    plt.savefig(f"fig_2.png")
    plt.close()

def fig_3_run_experiment(repeats):
    sweep_array_size_kib(10, 3000, 20, repeats, "TASK_CLOCK_NS")
    os.rename("param_sweep.csv", "fig_3.csv")

def fig_3_make_fig(repeats):

    df = pd.read_csv(
        "fig_3.csv",
        comment="#",
        index_col="array-size-kib"
    )

    x = df.index.values
    y = df[f"TASK_CLOCK_NS:MEDIAN"].values / (x * 16 * repeats)

    plt.figure()
    plt.plot(x, y, marker="")
    #plt.xscale("log")
    #plt.yscale("log")
    plt.xlabel("Array size (KiB)")
    plt.ylabel("Time (ns)")
    plt.title("Latency jump from L2 to L3 cache")
    plt.grid(True)
    plt.savefig(f"fig_3.png")
    plt.close()

def fig_4_run_experiment(repeats):
    sweep_array_size_kib(1000, 7000, 100, repeats, "TASK_CLOCK_NS")
    os.rename("param_sweep.csv", "fig_4.csv")

def fig_4_make_fig(repeats):

    df = pd.read_csv(
        "fig_4.csv",
        comment="#",
        index_col="array-size-kib"
    )

    x = df.index.values
    y = df[f"TASK_CLOCK_NS:MEDIAN"].values / (x * 16 * repeats)

    plt.figure()
    plt.plot(x, y, marker="")
    #plt.xscale("log")
    #plt.yscale("log")
    plt.xlabel("Array size (KiB)")
    plt.ylabel("Time (ns)")
    plt.title("Latency jump from L3 cache to DRAM")
    plt.grid(True)
    plt.savefig(f"fig_4.png")
    plt.close()

if __name__ == "__main__":
    #fig_1_run_experiment()
    fig_1_make_fig()

    repeats = 1000
    #fig_2_run_experiment(repeats)
    fig_2_make_fig(repeats)

    repeats = 100
    #fig_3_run_experiment(repeats)
    fig_3_make_fig(repeats)

    repeats = 1
    #fig_4_run_experiment(repeats)
    fig_4_make_fig(repeats)
