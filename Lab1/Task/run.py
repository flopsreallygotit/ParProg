import os
import subprocess


def run_with_different_sizes(max_processors_num=5):
    for processors_num in range(1, max_processors_num + 1):
        args = ["mpirun", "-np", str(processors_num), "./main"]

        subprocess.run(args)


def run_with_different_steps(iter_step=1, steps_num=10, time_order=0.1):
    for time_step in range(iter_step, iter_step * steps_num, iter_step):
        time_step *= time_order

        os.environ["T_STEP"] = str(time_step)

        run_with_different_sizes()


if __name__ == "__main__":
    run_with_different_steps()

    run_with_different_sizes(max_processors_num=1) # For properly image generation
