import subprocess
from dataclasses import dataclass, field

@dataclass
class ParamSweep:
    key: str
    low: int
    high: int
    step: int

@dataclass
class Cyclops:
    workload: str
    metric_grp: str
    warmup_runs: int = 5
    batch_runs: int = 5
    params: dict = field(default_factory=dict)
    param_sweep: ParamSweep | None = None
    write_per_batch_csv_files: bool = False

    def exec(self):
        arg_list = [
            "../cyclops",
            "-w", self.workload,
            "-m", self.metric_grp,
            "-u", str(self.warmup_runs),
            "-r", str(self.batch_runs),
        ]

        if self.write_per_batch_csv_files:
            arg_list.append("-o")

        for key, val in self.params.items():
            arg_list.append("-p")
            arg_list.append(f"{key}={val}")

        if self.param_sweep:
            arg_list.append("-s")
            arg_list.append(
                f"{self.param_sweep.key}="
                f"{self.param_sweep.low}:"
                f"{self.param_sweep.high}:"
                f"{self.param_sweep.step}"
            )

        result = subprocess.run(arg_list)
        print(result.stdout)

if __name__ == "__main__":
    workload_params = { "array-elements": 100 }
    cyclops = Cyclops(
        workload="STRIDED_ARRAY",
        metric_grp="IPC",
        params=workload_params,
    )
    cyclops.exec()
