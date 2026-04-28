import matplotlib.pyplot as plt
import pandas as pd

from cyclops import Cyclops, ParamSweep

BATCH_RUNS = 50
WARMUP_RUNS = 5

#AGGREGATE = "MIN"
#AGGREGATE = "MAX"
AGGREGATE = "MEDIAN"

def run_L1D_experiment():

    param_sweep = ParamSweep(
        key="array-elements",
        low=100,
        high=10000,
        step=10,
    )

    cyclops = Cyclops(
        workload="STRIDED_ARRAY",
        metric_grp="L1D_READS",
        warmup_runs=WARMUP_RUNS,
        batch_runs=BATCH_RUNS,
        param_sweep=param_sweep,
    )
    cyclops.exec()

    df = pd.read_csv(
        "L1D_READ_MISS_RATE.csv",
        comment="#",
        index_col=param_sweep.key
    )

    return 64 * df.index.values, df[AGGREGATE].values

def run_LLC_experiment():

    param_sweep = ParamSweep(
        key="array-elements",
        low=10000,
        high=1000000,
        step=10000,
    )

    cyclops = Cyclops(
        workload="STRIDED_ARRAY",
        metric_grp="LLC_READS",
        warmup_runs=WARMUP_RUNS,
        batch_runs=BATCH_RUNS,
        param_sweep=param_sweep,
    )
    cyclops.exec()

    df = pd.read_csv(
        "LLC_READ_MISS_RATE.csv",
        comment="#",
        index_col=param_sweep.key
    )

    return 64 * df.index.values, df[AGGREGATE].values

if __name__ == "__main__":

    x_L1D, y_L1D = run_L1D_experiment()
    x_LLC, y_LLC = run_LLC_experiment()

    plt.figure()
    plt.plot(x_L1D, y_L1D, marker="", label="L1D")
    plt.plot(x_LLC, y_LLC, marker="", label="LLC")
    plt.xscale("log")
    #plt.yscale("log")
    plt.xlabel("Array size (Bytes)")
    plt.ylabel("Miss rate")
    plt.title("")
    plt.grid(True)
    plt.legend()
    plt.savefig(f"cache_miss_rates.png")
    plt.close()
