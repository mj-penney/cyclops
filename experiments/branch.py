import matplotlib.pyplot as plt
from matplotlib.ticker import EngFormatter
import pandas as pd
import os

from cyclops import Cyclops, ParamSweep

BATCH_RUNS = 20
WARMUP_RUNS = 20
N_BRANCHES = 250000

AGGREGATE = "MEDIAN"

WORKLOAD = "BRANCH_PATTERN"

METRIC_GRP = "BRANCH"
METRIC = "BRANCH_MISPREDICTIONS"

def sweep_pattern_len(lo: int, hi: int, step: int):

    param_sweep = ParamSweep(
        key="pattern-len",
        low=lo,
        high=hi,
        step=step,
    )

    cyclops = Cyclops(
        workload=WORKLOAD,
        metric_grp=METRIC_GRP,
        warmup_runs=WARMUP_RUNS,
        batch_runs=BATCH_RUNS,
        params={
            "n-branches": N_BRANCHES,
            "bias": 50,
        },
        param_sweep=param_sweep,
    )
    cyclops.exec()

def fig_1_run_experiment():
    sweep_pattern_len(1, 60000, 1000)
    os.rename("param_sweep.csv", "fig_1.csv")

def fig_1_make_fig():

    df = pd.read_csv(
        f"fig_1.csv",
        comment="#",
        index_col="pattern-len"
    )

    x = df.index.values
    y1 = df[f"BRANCH_MISPREDICTIONS:{AGGREGATE}"].values / N_BRANCHES
    y2 = df[f"IPC:{AGGREGATE}"].values

    fig = plt.figure()

    ax1 = fig.add_subplot(2, 1, 1)
    ax2 = fig.add_subplot(2, 1, 2, sharex=ax1)

    ax1.plot(x, 100 * y1, marker="")
    ax2.plot(x, y2, marker="")

    ax1.set_title("% Mispredictions and IPC vs Pattern Length")
    ax1.set_ylabel("% Mispredictions")

    ax2.set_ylabel("IPC")
    ax2.set_xlabel("Pattern Length")

    ax1.tick_params(axis='x', which='both', labelbottom=False)
    ax2.tick_params(axis='x', which='both', labelbottom=True)
    ax2.xaxis.set_major_formatter(EngFormatter(unit='', sep=''))

    ax1.grid(True)
    ax2.grid(True)

    fig.tight_layout()
    plt.savefig("fig_1.png")
    plt.close()

def fig_2_run_experiment():
    sweep_pattern_len(1, 3000, 30)
    os.rename("param_sweep.csv", "fig_2.csv")

def fig_2_make_fig():

    df = pd.read_csv(
        f"fig_2.csv",
        comment="#",
        index_col="pattern-len"
    )

    x = df.index.values
    y1 = df[f"BRANCH_MISPREDICTIONS:{AGGREGATE}"].values / N_BRANCHES

    fig, ax1 = plt.subplots()
    ax1.plot(x, 100 * y1)

    ax1.set_title("% Mispredictions vs Pattern Length")
    ax1.set_ylabel("% Mispredictions")
    ax1.set_xlabel("Pattern length")

    ax1.xaxis.set_major_formatter(EngFormatter(unit='', sep=''))
    ax1.grid(True)

    fig.tight_layout()

    plt.savefig("fig_2.png")
    plt.close(fig)

def fig_3_run_experiment():
    sweep_pattern_len(1, 6000, 30)
    os.rename("param_sweep.csv", "fig_3.csv")

def fig_3_make_fig():

    df = pd.read_csv(
        f"fig_3.csv",
        comment="#",
        index_col="pattern-len"
    )

    x = df.index.values
    y1 = df[f"BRANCH_MISPREDICTIONS:{AGGREGATE}"].values / N_BRANCHES
    y2 = df[f"IPC:{AGGREGATE}"].values

    fig = plt.figure()

    ax1 = fig.add_subplot(2, 1, 1)
    ax2 = fig.add_subplot(2, 1, 2, sharex=ax1)

    ax1.plot(x, 100 * y1, marker="")
    ax2.plot(x, y2, marker="")

    ax1.set_title("% Mispredictions and IPC vs Pattern Length")
    ax1.set_ylabel("% Mispredictions")

    ax2.set_ylabel("IPC")
    ax2.set_xlabel("Pattern Length")

    ax1.tick_params(axis='x', which='both', labelbottom=False)
    ax2.tick_params(axis='x', which='both', labelbottom=True)
    ax2.xaxis.set_major_formatter(EngFormatter(unit='', sep=''))

    ax1.grid(True)
    ax2.grid(True)

    ax1.axvline(x=3820, linestyle="--")
    ax2.axvline(x=3820, linestyle="--")

    fig.tight_layout()
    plt.savefig("fig_3.png")
    plt.close()

if __name__ == "__main__":

    fig_1_run_experiment()
    fig_1_make_fig()

    fig_2_run_experiment()
    fig_2_make_fig()

    fig_3_run_experiment()
    fig_3_make_fig()

