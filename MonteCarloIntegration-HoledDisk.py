import numpy as np
import matplotlib.pyplot as plt


def inside_holed_disk(x, y):
    """
    Restituisce True se (x,y) appartiene al disco grande
    ma non al foro interno.
    """
    in_big_disk = x**2 + y**2 <= 1.0**2
    in_hole = (x - 0.5)**2 + y**2 <= 0.5**2
    return in_big_disk & (~in_hole)


def monte_carlo_holed_disk(n_samples=100_000, seed=1234):
    """
    Campiona punti uniformi nel quadrato [-1,1]x[-1,1]
    e stima:
      - area della regione
      - integrale di x
      - integrale di y
      - integrale di x^2+y^2
      - centro di massa
      - momento d'inerzia rispetto all'origine
    """
    rng = np.random.default_rng(seed)

    # Campionamento uniforme nel quadrato
    x = rng.uniform(-1.0, 1.0, n_samples)
    y = rng.uniform(-1.0, 1.0, n_samples)

    # Area del quadrato contenitore
    V = 4.0

    # Maschera della regione
    mask = inside_holed_disk(x, y)

    # Funzioni da integrare, prolungate con zero fuori dalla regione
    f_area = mask.astype(float)
    f_x = np.where(mask, x, 0.0)
    f_y = np.where(mask, y, 0.0)
    f_inertia = np.where(mask, x**2 + y**2, 0.0)

    # Stime Monte Carlo degli integrali
    area_est = V * np.mean(f_area)
    int_x_est = V * np.mean(f_x)
    int_y_est = V * np.mean(f_y)
    inertia_est = V * np.mean(f_inertia)

    # Errori statistici (deviazione standard della media)
    area_err = V * np.std(f_area, ddof=1) / np.sqrt(n_samples)
    int_x_err = V * np.std(f_x, ddof=1) / np.sqrt(n_samples)
    int_y_err = V * np.std(f_y, ddof=1) / np.sqrt(n_samples)
    inertia_err = V * np.std(f_inertia, ddof=1) / np.sqrt(n_samples)

    # Centro di massa: x_cm = (∫ x dA)/(Area), y_cm = (∫ y dA)/(Area)
    x_cm_est = int_x_est / area_est
    y_cm_est = int_y_est / area_est

    results = {
        "x": x,
        "y": y,
        "mask": mask,
        "area": area_est,
        "area_err": area_err,
        "int_x": int_x_est,
        "int_x_err": int_x_err,
        "int_y": int_y_est,
        "int_y_err": int_y_err,
        "x_cm": x_cm_est,
        "y_cm": y_cm_est,
        "inertia": inertia_est,
        "inertia_err": inertia_err,
    }

    return results


def plot_sample_points(x, y, mask, max_points=15000):
    """
    Mostra i punti Monte Carlo:
      - blu = punti accettati nella regione
      - grigio chiaro = punti fuori
    """
    n = min(len(x), max_points)

    x_plot = x[:n]
    y_plot = y[:n]
    mask_plot = mask[:n]

    fig, ax = plt.subplots(figsize=(7, 7))

    ax.scatter(x_plot[~mask_plot], y_plot[~mask_plot], s=4, alpha=0.3, label="Fuori regione")
    ax.scatter(x_plot[mask_plot], y_plot[mask_plot], s=4, alpha=0.6, label="Dentro regione")

    # Disegno del bordo del disco esterno
    t = np.linspace(0, 2*np.pi, 500)
    ax.plot(np.cos(t), np.sin(t), linewidth=2)

    # Disegno del bordo del foro
    ax.plot(0.5 + 0.5*np.cos(t), 0.5*np.sin(t), linewidth=2)

    ax.set_aspect("equal")
    ax.set_xlim(-1.05, 1.05)
    ax.set_ylim(-1.05, 1.05)
    ax.set_title("Campionamento Monte Carlo del disco forato")
    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.legend()
    ax.grid(True, alpha=0.3)
    plt.show()


def running_area_estimate(n_samples=50_000, seed=1234):
    """
    Calcola la stima progressiva dell'area per visualizzare la convergenza.
    """
    rng = np.random.default_rng(seed)
    x = rng.uniform(-1.0, 1.0, n_samples)
    y = rng.uniform(-1.0, 1.0, n_samples)

    mask = inside_holed_disk(x, y).astype(float)

    cumulative_mean = np.cumsum(mask) / np.arange(1, n_samples + 1)
    area_running = 4.0 * cumulative_mean

    return area_running


def plot_running_area(area_running):
    """
    Mostra la convergenza della stima dell'area.
    """
    fig, ax = plt.subplots(figsize=(8, 4.5))
    ax.plot(area_running, linewidth=1.2, label="Stima Monte Carlo progressiva")

    # Valore esatto dell'area:
    # area disco grande - area foro = pi - pi/4 = 3pi/4
    exact_area = 3.0 * np.pi / 4.0
    ax.axhline(exact_area, linestyle="--", linewidth=2, label="Valore esatto 3π/4")

    ax.set_title("Convergenza della stima dell'area")
    ax.set_xlabel("Numero di campioni")
    ax.set_ylabel("Area stimata")
    ax.legend()
    ax.grid(True, alpha=0.3)
    plt.show()


if __name__ == "__main__":
    results = monte_carlo_holed_disk(n_samples=200_000, seed=42)

    print("=== Stime Monte Carlo ===")
    print(f"Area         = {results['area']:.6f} ± {results['area_err']:.6f}")
    print(f"∫ x dA       = {results['int_x']:.6f} ± {results['int_x_err']:.6f}")
    print(f"∫ y dA       = {results['int_y']:.6f} ± {results['int_y_err']:.6f}")
    print(f"x_cm         = {results['x_cm']:.6f}")
    print(f"y_cm         = {results['y_cm']:.6f}")
    print(f"I_O          = {results['inertia']:.6f} ± {results['inertia_err']:.6f}")

    plot_sample_points(results["x"], results["y"], results["mask"], max_points=12000)

    area_running = running_area_estimate(n_samples=50_000, seed=42)
    plot_running_area(area_running)