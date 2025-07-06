# Lecture 1: Introduction and Motivation — The Need for Domain Decomposition

## Electromagnetic Wave Propagation in Heterogeneous Media

A motivating example comes from electromagnetics, where the inverse problem of reconstructing the electric permittivity $\varepsilon$ from measurements of the electric field $\mathbf{E}$ involves solving Maxwell’s equations:

$$
\nabla \times (\mu^{-1} \nabla \times \mathbf{E}) - \omega^2 \varepsilon \mathbf{E} = \mathbf{J}
$$

Where:
- $\mathbf{E}$: electric field
- $\mu > 0$: magnetic permeability
- $\varepsilon > 0$: electric permittivity
- $\omega$: angular frequency
- $\mathbf{J}$: source current

**Key challenge:** The inverse problem is ill-posed and computationally demanding, especially at high frequencies or in complex domains.

![Wave propagation examples](/figures/em_wave_examples.png)
<!-- Replace with images such as avc1imag_v2.png, coupesagittale_v2.png, resistor.png -->

---

## Why Efficient Linear Solvers Matter

Solving Maxwell’s equations or other PDEs often requires solving large linear systems:

$$
A \mathbf{u} = \mathbf{b}
$$

There are three main solver strategies:

- **Direct methods** (e.g., LU, Cholesky): Robust but memory intensive.
- **Iterative methods** (e.g., CG, GMRES): Scalable but may converge slowly.
- **Hybrid methods** (e.g., DDM, Multigrid): Combine robustness and scalability.

> Domain Decomposition (DDM) is a key hybrid method, enabling parallelism and modularity.

![Solver landscape](/figures/whydd.png)

---

## Why Not Just Use Direct Solvers?

Direct solvers are limited in practice due to **fill-in and memory constraints**, especially in 3D.

### Complexity table:

| Method                     | 1D           | 2D           | 3D           |
|---------------------------|--------------|--------------|--------------|
| Dense matrix              | $\mathcal{O}(n^3)$ | $\mathcal{O}(n^3)$ | $\mathcal{O}(n^3)$ |
| Band structure exploited  | $\mathcal{O}(n)$   | $\mathcal{O}(n^2)$ | $\mathcal{O}(n^{7/3})$ |
| Sparse (e.g. nested dissection) | $\mathcal{O}(n)$ | $\mathcal{O}(n^{3/2})$ | $\mathcal{O}(n^2)$ |

- 2D: up to $10^7$ DoFs can be solved directly
- 3D: fill-in becomes critical around $10^5$ unknowns
- Hybrid methods are essential for scalability

---

## Hardware Trends and Parallelism Opportunities

### Dennard Scaling Breakdown
Modern processors no longer scale by frequency—parallelism is essential.

![Dennard scaling](/figures/dennard_scaling.pdf)

### Supercomputing Trends

![Top500 Performance](/figures/top500-jun-2025-performance.pdf)

### Why Domain Decomposition?

- Matches modern CPU/GPU hardware
- Supports scalable, modular design
- Enables high-performance on laptops to supercomputers

---

## Takeaways

- PDE solvers must scale to very large systems.
- Domain decomposition enables parallelism and modularity.
- Additive Schwarz is a foundational idea for preconditioners and solvers.

[Next Lecture → Domain Decomposition Foundations](./domain-decomposition.md)
