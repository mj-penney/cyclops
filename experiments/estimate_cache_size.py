import matplotlib.pyplot as plt
import pandas as pd

from cyclops import BatchRunner

def run_L1D_batches(array_sizes: list):

    x_L1D = []
    y_L1D = []

    for size in array_sizes:

        params = { "array-elements": size }
        batch = BatchRunner(
            workload="STRIDED_ARRAY",
            metric_grp="L1D_READS",
            params=params,
        )
        batch.exec()

        df = pd.read_csv(
            batch.build_output_file_name(),
            comment="#",
            index_col="METRIC"
        )
        x_L1D.append(batch.params.get("array-elements", 0) * 64)
        y_L1D.append(df.loc["L1D_READ_MISS_RATE", "MEDIAN"])

    return x_L1D, y_L1D

def run_LLC_batches(array_sizes: list):

    x_LLC = []
    y_LLC = []

    for size in array_sizes:

        params = { "array-elements": size }
        batch = BatchRunner(
            workload="STRIDED_ARRAY",
            metric_grp="LLC_READS",
            params=params,
        )
        batch.exec()

        df = pd.read_csv(
            batch.build_output_file_name(),
            comment="#",
            index_col="METRIC"
        )

        x_LLC.append(batch.params.get("array-elements", 0) * 64)
        y_LLC.append(df.loc["LLC_READ_MISS_RATE", "MEDIAN"])

    return x_LLC, y_LLC

if __name__ == "__main__":
    array_elements = [
        100,
        300,
        500,
        800,

        1000,
        3000,
        3500,
        4000,
        5000,
        5500,
        6000,
        8000,

        10000,
        30000,
        50000,
        80000,

        100000,
        300000,
        500000,
        800000,

        1000000,
        3000000,
        5000000,
        8000000,

        #10000000,
        #50000000,

        #100000000,
    ]

    x_LLC, y_LLC = run_LLC_batches(array_elements)
    x_L1D, y_L1D = run_L1D_batches(array_elements)

    plt.figure()
    plt.plot(x_LLC, y_LLC, marker="o", label="LLC")
    plt.plot(x_L1D, y_L1D, marker="o", label="L1D")
    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel("Array size (Bytes)")
    plt.ylabel("Miss rate")
    plt.title("")
    plt.grid(True)
    plt.legend()
    plt.savefig(f"cache_miss_rates.png")
    plt.close()
