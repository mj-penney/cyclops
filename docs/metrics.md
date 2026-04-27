# Metric Groups

See the default metric groups (declared in `/core/metric/frontend.c`) or with
the output of `cyclops -h`.

You can only benchmark with one metric group, so select one that contains the
metrics you want (or create a custom group).

## Benchmarking Methodology

The following methods are used to maximise benchmark accuracy and minimise
measurement noise:

- Pin thread to a single core
- Warmup runs to train branch predictors and warm caches (set from the cli)
- Barriers and serialization to ensure the compiler or CPU don't reorder
  workload instructions outside the measurement window
- Detect kernel multiplexing physical PMU counters with `time_running` and
  `time_enabled`, and scale results if necessary (for `perf_event_open()`)

See the benchmarking code in the metric backends in `core/metric/`.
