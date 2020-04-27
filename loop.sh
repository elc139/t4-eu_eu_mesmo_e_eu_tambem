#!/bin/bash

export WSLENV=OUTPUT_ONLY_TIME:OMP_NUM_THREADS:${WSLENV};

export OUTPUT_ONLY_TIME=1;

# shellcheck disable=SC2034
_probs_exec=./cmake-build-mingw/virusim_parallel_probs.exe;
# shellcheck disable=SC2034
_trials_exec=./cmake-build-mingw/virusim_parallel_trials.exe;

# shellcheck disable=SC2034
_probs_name="probabilities";
# shellcheck disable=SC2034
_trials_name="trials";

# shellcheck disable=SC2034
_small_size=30;
# shellcheck disable=SC2034
_small_trials=50;
# shellcheck disable=SC2034
_small_probs=11;
# shellcheck disable=SC2034
_small_sample=100;

# shellcheck disable=SC2034
_medium_size=30;
# shellcheck disable=SC2034
_medium_trials=5000;
# shellcheck disable=SC2034
_medium_probs=101;
# shellcheck disable=SC2034
_medium_sample=10;

# shellcheck disable=SC2034
_big_size=100;
# shellcheck disable=SC2034
_big_trials=5000;
# shellcheck disable=SC2034
_big_probs=101;
# shellcheck disable=SC2034
_big_sample=1;

printf "# Results tables\n\n";

for approach in 'probs' 'trials'; do

  _exec=_${approach}_exec;
  _name=_${approach}_name;

  printf "## Executing with parallelized _%s_ approach\n\n" ${!_name};

  for problem_size in 'small' 'medium' 'big'; do

    _size=_${problem_size}_size;
    _trials=_${problem_size}_trials;
    _probs=_${problem_size}_probs;
    _sample=_${problem_size}_sample;

    printf "### %s problem (x%d samples)\n\n" ${problem_size^} ${!_sample};

    printf "| %*s | %*s | %*s | %*s |\n" \
      -7 "# Threads" -13 "Mean Time" -7 "Speedup" -10 "Efficiency";
    echo "|      ---: |          ---: |    ---: |       ---: |";
    for nthreads in {1..12}; do
      export OMP_NUM_THREADS=${nthreads};

      sum=0;
      for i in $(seq 1 ${!_sample}); do
        printf "Taking sample # %d" "$i" 1>&2;
        sum=$((sum + $(${!_exec} ${!_size} ${!_trials} ${!_probs})));
        printf "%b" "$(seq -s'\b' "$(tput cols)" | tr -d "[:digit:]")" 1>&2;
      done;
      printf "%b" "$(seq -s'\b' "$(tput cols)" | tr -d "[:digit:]")" 1>&2;
      time=$((sum / ${!_sample}));

      if [ "$nthreads" -eq "1" ]; then
        base="$time";
      fi;

      # shellcheck disable=SC2005
      speedup=$(echo "$(printf 'scale=4; %s / %s' "$base" "$time")" | bc);
      # shellcheck disable=SC2005
      efficiency=$(echo "$(printf 'scale=4; %s / %s * 100' "$speedup" "$nthreads")" | bc);
      printf "| %*s | %*s ms | %*s | %*s %% |\n" \
        9 "$nthreads" 10 "$time" 7 "$(printf "%.4f" "$speedup")" 8 "$(printf "%.2f" "$efficiency")";
    done;
    echo "";
  done;
done;