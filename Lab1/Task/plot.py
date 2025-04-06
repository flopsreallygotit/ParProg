import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

def plot_solution():
    dataframe = pd.read_csv("results.csv")
    x = dataframe["x"]
    t = dataframe["t"]
    u = dataframe["u"]

    fig = plt.figure()
    ax = fig.add_subplot(111, projection="3d")
    ax.plot_trisurf(x, t, u, cmap="viridis")

    ax.set_xlabel("x")
    ax.set_ylabel("t")
    ax.set_zlabel("u")
    ax.set_title("График функции u(x,t)")

    plt.savefig("solution.png")
    plt.clf()

def plot_value_time(values, times, title):
    value_time_map = dict()

    for map_idx in range(len(values)):
        value = values[map_idx]
        time  = times[map_idx]

        if value not in value_time_map:
            value_time_map[value] = [time]
        else:
            value_time_map[value].append(time)

    for value in value_time_map:
        avg_time = np.mean(value_time_map[value])
        value_time_map[value] = avg_time

    plt.title(f"{title} relation")
    plt.xlabel("Value")
    plt.ylabel("Time")

    plt.bar(value_time_map.keys(), value_time_map.values())
    plt.savefig(f"{title}.png")
    plt.clf()

def plot_statistics():
    dataframe = pd.read_csv("statistics.csv")
    dataframe.columns = ["size", "time", "step"]

    sizes = dataframe["size"]
    times = dataframe["time"]
    steps = dataframe["step"]

    plot_value_time(sizes, times, "size_time")
    plot_value_time(steps, times, "step_time")

if __name__ == "__main__":
    # plot_solution()
    plot_statistics()
