import csv
import os
import sys
from collections import defaultdict

import matplotlib.pyplot as plt

CSV_PATH = sys.argv[1] if len(sys.argv) > 1 else "results.csv"
OUT_DIR = sys.argv[2] if len(sys.argv) > 2 else "."

data = defaultdict(lambda: defaultdict(list))

with open(CSV_PATH) as f:
    reader = csv.DictReader(f)
    for row in reader:
        algo = row["algorithm"]
        atype = row["array_type"]
        n = int(row["n"])
        t = float(row["avg_time_us"])
        c = float(row["avg_char_comparisons"])
        data[atype][algo].append((n, t, c))

os.makedirs(OUT_DIR, exist_ok=True)

for atype, algos in data.items():
    for metric_idx, metric_name, ylabel in [
        (1, "time", "avg time"),
        (2, "comparisons", "avg char comparisons"),
    ]:
        plt.figure(figsize=(9, 6))
        for algo, rows in sorted(algos.items()):
            rows.sort()
            ns = [r[0] for r in rows]
            ys = [r[metric_idx] for r in rows]
            plt.plot(ns, ys, marker="o", markersize=3, label=algo)
        plt.xlabel("n")
        plt.ylabel(ylabel)
        plt.title(f"{metric_name} / n ({atype})")
        plt.legend()
        plt.grid(True, alpha=0.3)
        out_path = os.path.join(OUT_DIR, f"{metric_name}_{atype}.png")
        plt.savefig(out_path, dpi=100, bbox_inches="tight")
        plt.close()
        print(f"saved {out_path}")
