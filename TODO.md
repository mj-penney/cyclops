# TODO

## General

- Implement a logging module
- Improve error handling and propagation

## Documentation

- Thoroughly document the workload plugin system
- Document the metric/group hierarchy and how they relate to metric backends
- Document metric backends & strategies for increasing accuracy and reducing
  noise
- For workloads and metric groups, add a description to the object
    - Descriptions and more details (e.g. metrics within the group and workload
      params & usage) should be displayed if the user does
      `./cyclops -w BRANCH` with no other args
- Document how batches and parameter-sweeps work, and how results are
  aggregated and output to CSV files
- Add troubleshooting section to README

## CLI

- Add option to print info for each workload
- Add advanced options:
    - Pin to specific core (core 0 by default)
    - Include/exclude kernel for perf\_event\_open() metric groups

## Batch / Param-Sweep Orchestration

- Add more aggregations (stddev etc.)

## Metric Groups

- Verify the effectiveness of the barriers and serialization in metric backends
- Add ARM-specific instruction metrics/groups

## Workload

- Add a template workload plugin file for users to get users started
