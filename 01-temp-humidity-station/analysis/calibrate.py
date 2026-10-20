import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

d = np.genfromtxt("data.csv", delimiter=",", names=True)
raw, ref = d["raw"], d["ref"]

gain, offset = np.polyfit(raw, ref, 1)
cal = gain * raw + offset

def stats(x, name):
    e = x - ref
    print(f"{name:>7}: mean err {e.mean():+.3f} | RMSE {np.sqrt((e**2).mean()):.3f} "
          f"| max |err| {np.abs(e).max():.3f} | %FS {100*np.abs(e).max()/(ref.max()-ref.min()):.2f}%")

print(f"CAL_T_GAIN = {gain:.5f}f   CAL_T_OFFSET = {offset:.4f}f")
stats(raw, "Before")
stats(cal, "After")

# Validation points (not used in the fit), from the MQTT/serial output
v_ref = np.array([10.0, 30.0]); v_raw = np.array([8.40, 29.20]); v_cal = np.array([10.00, 30.00])

fig, ax = plt.subplots(1, 2, figsize=(11, 4.2))
ax[0].plot(ref, ref, "k--", label="Ideal")
ax[0].plot(ref, raw, "o-", label="Raw")
ax[0].plot(ref, cal, "s-", label="Calibrated")
ax[0].plot(v_ref, v_cal, "g^", markersize=9, label="Validation (new points)")
ax[0].set_xlabel("Reference (°C)"); ax[0].set_ylabel("Sensor (°C)")
ax[0].legend(); ax[0].set_title("Sensor vs reference"); ax[0].grid(alpha=.3)

allref = np.concatenate([ref, v_ref]); allraw = np.concatenate([raw, v_raw]); allcal = np.concatenate([cal, v_cal])
order = np.argsort(allref); allref, allraw, allcal = allref[order], allraw[order], allcal[order]
x = np.arange(len(allref))
ax[1].bar(x - 0.2, allraw - allref, 0.4, label="Raw error")
ax[1].bar(x + 0.2, allcal - allref, 0.4, label="Calibrated error")
ax[1].set_xticks(x); ax[1].set_xticklabels([f"{r:g}" for r in allref])
ax[1].set_xlabel("Reference (°C)"); ax[1].set_ylabel("Error (°C)")
ax[1].legend(); ax[1].set_title("Error before / after calibration"); ax[1].grid(alpha=.3, axis="y")
plt.tight_layout(); plt.savefig("p1_result.png", dpi=150)
