# save_plots.py
import os
import pandas as pd
import matplotlib.pyplot as plt

# Путь к CSV (предполагается файл results.csv в той же папке)
CSV_PATH = "results.csv"
OUT_DIR = "plots"

os.makedirs(OUT_DIR, exist_ok=True)

df = pd.read_csv(CSV_PATH)

# столбцы: n,rand_std,rand_hyb,rev_std,rev_hyb,alm_std,alm_hyb
arrays = [
    ("rand_std", "rand_hyb", "Random array"),
    ("rev_std", "rev_hyb", "Reverse array"),
    ("alm_std", "alm_hyb", "Almost sorted array")
]

for std_col, hyb_col, title in arrays:
    plt.figure(figsize=(10,5))
    plt.plot(df["n"], df[std_col], label="Merge Sort (standard)")
    plt.plot(df["n"], df[hyb_col], label="Hybrid Merge+Insertion")
    plt.xlabel("Array size (n)")
    plt.ylabel("Time (ms)")
    plt.title(title)
    plt.legend()
    plt.grid(True)

    # безопасное имя файла
    safe_title = title.lower().replace(" ", "_").replace("+", "plus")
    out_path = os.path.join(OUT_DIR, f"{safe_title}.png")
    plt.savefig(out_path, bbox_inches='tight', dpi=200)
    plt.close()
    print(f"Saved plot: {out_path}")

print("All plots saved to folder:", OUT_DIR)
