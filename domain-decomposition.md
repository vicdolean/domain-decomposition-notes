# Domain Decomposition Foundations

## The Schwarz Method: Origin of Domain Decomposition

**Original Schwarz Method (1870):**  
A classical overlapping method to solve $-\Delta u = f$ in $\Omega$ with homogeneous Dirichlet boundary conditions by alternating updates over overlapping subdomains.

### Schwarz Alternating Iteration

Given $u^n_1$, $u^n_2$, compute:
- $-\Delta u^{n+1}_1 = f$ in $\Omega_1$,  
  $u^{n+1}_1 = 0$ on $\partial\Omega_1 \cap \partial\Omega$,  
  $u^{n+1}_1 = u^n_2$ on $\partial\Omega_1 \cap \Omega_2$

- $-\Delta u^{n+1}_2 = f$ in $\Omega_2$,  
  $u^{n+1}_2 = 0$ on $\partial\Omega_2 \cap \partial\Omega$,  
  $u^{n+1}_2 = u^{n+1}_1$ on $\partial\Omega_2 \cap \Omega_1$

**Key Observations:**
- Naturally parallel, but converges slowly.
- Overlap is essential for communication.
- The parallel version is known as the **Jacobi–Schwarz Method (JSM).**

---

## Abstract ASM and RAS: Local-to-Global Strategy

**Definition 1 (Extension Operators):**  
Each $E_i$ extends a function $w_i: \Omega_i \rightarrow \mathbb{R}$ to a global function $E_i(w_i): \Omega \rightarrow \mathbb{R}$, zero outside $\Omega_i$.

**Definition 2 (Partition of Unity):**  
$\chi_i: \Omega_i \rightarrow \mathbb{R}$, with  
$\chi_i(x) \geq 0$, $\chi_i(x) = 0$ on $\partial\Omega_i$, and $\sum_i \chi_i(x) = 1$ on $\Omega$.

**Reconstruction:**  
$w(x) = \sum_i E_i(\chi_i \cdot w|_{\Omega_i})$

### Iteration Schemes

- **Additive Schwarz (ASM):**  
  $u^{n+1} = \sum_i E_i(u^{n+1}_i)$

- **Restricted Additive Schwarz (RAS):**  
  $u^{n+1} = \sum_i E_i(\chi_i u^{n+1}_i)$

---

## 2.3 Connection to Block-Jacobi Iteration

**Jacobi Method:**  
$U^{n+1} = U^n + D^{-1}(F - AU^n)$

### Block Jacobi Setup

Let the natural block splitting of $A$.

**Update:**
- $A_{11} U_1^{n+1} = F_1 - A_{12} U_2^n$
- $A_{22} U_1^{n+1} = F_2 - A_{21} U_1^n$

**Residual Form:**
$U^{n+1} = U^n +(R_1^T A_{11}^{-1} R_1 + R_2^T A_{22}^{-1} R_2)r^n$


This reflects RAS/ASM in global PDE solvers.


[Next Lecture → Convergence Analysis](./convergence-analysis.md)
