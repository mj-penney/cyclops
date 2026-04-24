import subprocess
from dataclasses import dataclass, field

@dataclass
class BatchRunner:
    workload: str
    metric_grp: str
    params: dict = field(default_factory=dict)

    def build_output_file_name(self) -> str:
        file_name = f"{self.workload}"
        file_name += f"__{self.metric_grp}"

        for key, val in self.params.items():
            file_name += f"__{key}_{val}"

        file_name += ".csv"
        return file_name

    def exec(self):
        arg_list = [
            "../cyclops",
            "-w", self.workload,
            "-m", self.metric_grp,
            "-r", "10",
            "-o", self.build_output_file_name(),
        ]

        for key, val in self.params.items():
            arg_list.append("-p")
            arg_list.append(f"{key}={val}")

        result = subprocess.run(arg_list)
        print(result.stdout)

if __name__ == "__main__":
    workload_params = { "array-elements": 100 }
    batch = BatchRunner(
        workload="STRIDED_ARRAY",
        metric_grp="IPC",
        params=workload_params,
    )
    batch.exec()
