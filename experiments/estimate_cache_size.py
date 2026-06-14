import matplotlib.pyplot as plt
import pandas as pd
import os

from cyclops import Cyclops, ParamSweep

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
    sweep_array_size_kib(10, 2800, 20, repeats, "TASK_CLOCK_NS")
    os.rename("param_sweep.csv", "fig_3.csv")

def fig_3_make_fig(repeats):

    df = pd.read_csv(
        "fig_3.csv",
        comment="#",
        index_col="array-size-kib"
    )

    x = df.index.values
    y = df[f"CPU_CYCLES:MEDIAN"].values / (x * 16 * repeats)

    plt.figure()
    plt.plot(x, y, marker="")
    plt.xscale("log")
    plt.yscale("log")
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

def latency_plot_make_fig(repeats):

    df = pd.read_csv(
        "fig_3.csv",
        comment="#",
        index_col="array-size-kib"
    )

    x = df.index.values
    ns_per_access = df[f"TASK_CLOCK_NS:MEDIAN"].values / (x * 16 * repeats)
    cycles_per_access = df[f"CPU_CYCLES:MEDIAN"].values / (x * 16 * repeats)

    fig = plt.figure()

    ax1 = fig.add_subplot(2, 1, 1)
    ax2 = fig.add_subplot(2, 1, 2, sharex=ax1)

    ax1.plot(x, ns_per_access, marker="")
    ax2.plot(x, cycles_per_access, marker="")

    ax1.set_xscale("log")
    ax2.set_xscale("log")

    ax1.set_title("Latency per Access", pad=25)

    ax1.set_ylabel("Latency (ns)")
    ax2.set_ylabel("Latency (cycles)")

    ax2.set_xlabel("Working set (KiB)")
    ax1.tick_params(axis="x", which="both", labelbottom=False)

    ax1.grid(True)
    ax2.grid(True)

    ax1.axvline(32, linestyle="--")
    ax1.axvline(256, linestyle="--")
    ax1.axvline(3072, linestyle="--")
    ax2.axvline(32, linestyle="--")
    ax2.axvline(256, linestyle="--")
    ax2.axvline(3072, linestyle="--")

    for size, label in [
        (32,   "L1D: 32 KiB"),
        (256,  "L2: 256 KiB"),
        (3072, "L3:\n3 MiB"),
    ]:
        ax1.text(
            size,
            1.03,
            label,
            transform=ax1.get_xaxis_transform(),
            ha="center",
            va="bottom",
            fontsize=9,
        )

    ax1.annotate(
        "2.5ns",
        xy=(17, 2.5),
        xytext=(15, 5),
        arrowprops={"arrowstyle": "->"},
    )

    ax1.annotate(
        "5.3ns",
        xy=(70, 5.3),
        xytext=(50, 7.8),
        arrowprops={"arrowstyle": "->"},
    )

    ax1.annotate(
        "11.0ns",
        xy=(320, 11.0),
        xytext=(350, 8),
        arrowprops={"arrowstyle": "->"},
    )

    ax1.annotate(
        "17.5ns",
        xy=(2000, 17.3),
        xytext=(1100, 13.7),
        arrowprops={"arrowstyle": "->"},
    )

    ax2.annotate(
        "6cy",
        xy=(17, 6),
        xytext=(15, 13),
        arrowprops={"arrowstyle": "->"},
    )

    ax2.annotate(
        "13cy",
        xy=(80, 13),
        xytext=(70, 20),
        arrowprops={"arrowstyle": "->"},
    )

    ax2.annotate(
        "27cy",
        xy=(320, 27),
        xytext=(380, 20),
        arrowprops={"arrowstyle": "->"},
    )

    ax2.annotate(
        "44cy",
        xy=(2000, 43),
        xytext=(1300, 35),
        arrowprops={"arrowstyle": "->"},
    )

    fig.tight_layout()
    plt.savefig("latency_plot.png")
    plt.close()

if __name__ == "__main__":
    #fig_1_run_experiment()
    #fig_1_make_fig()

    repeats = 1000
    #fig_2_run_experiment(repeats)
    #fig_2_make_fig(repeats)

    repeats = 100
    #fig_3_run_experiment(repeats)
    #fig_3_make_fig(repeats)
    latency_plot_make_fig(repeats)

    repeats = 1
    #fig_4_run_experiment(repeats)
    #fig_4_make_fig(repeats)
