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

![Two overlapping subdomains Ω1 and Ω2](images/fig_schwarz_overlap.png)

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
$$
U^{n+1} = U^n + \left( R_1^T A_1^{-1} R_1 + R_2^T A_2^{-1} R_2 \right) r^n
$$

This reflects RAS/ASM in global PDE solvers.

---

### 1D Poisson Example

Discretize $-\Delta u = f$ in $(0,1)$ using centered finite differences with $m$ internal nodes:
$$
A = \frac{1}{h^2} \text{tridiag}(-1, 2, -1)
$$

Split into overlapping subdomains:
- $\Omega_1 = (0, (m_s + 1)h)$
- $\Omega_2 = (m_s h, 1)$

Update in $\Omega_1$:
$$
\frac{-u^{n+1}_{1,j-1} + 2u^{n+1}_{1,j} - u^{n+1}_{1,j+1}}{h^2} = f_j,\quad
u^{n+1}_{1,0} = 0,\quad
u^{n+1}_{1,m_s+1} = u^n_{2,m_s+1}
$$

Similar for $\Omega_2$.

**Matrix View:**
$$
A =
\begin{pmatrix}
A_{11} & A_{12} \\
A_{21} & A_{22}
\end{pmatrix}
\Rightarrow
\begin{pmatrix}
A_{11} & 0 \\
0 & A_{22}
\end{pmatrix}
\begin{pmatrix}
U^{n+1}_1 \\
U^{n+1}_2
\end{pmatrix}
=
\begin{pmatrix}
F_1 - A_{12} U^n_2 \\
F_2 - A_{21} U^n_1
\end{pmatrix}
$$

![1D decomposition with minimal overlap](images/fig_overlap_1d.png)

---

## 2.4 Discrete Setting and Matrix Formulation

**Continuous ↔ Discrete Correspondence**

| Continuous | Discrete |
|------------|----------|
| $\Omega = \cup_i \Omega_i$ | $N = \cup_i N_i$ |
| $u : \Omega \to \mathbb{R}$ | $U \in \mathbb{R}^{\#N}$ |
| Restriction $u_i = u|_{\Omega_i}$ | $R_i \in \{0,1\}^{\#N_i \times \#N}$ |
| Extension $E_i$ | $R_i^T$ |
| Partition: $u = \sum E_i(\chi_i u_i)$ | $\sum R_i^T D_i R_i = I$ |

**Finite Difference Examples:**

- **No overlap:**  
  ![No overlap FD](images/fig_fd_no_overlap.png)

- **Overlap:**  
  ![Overlap FD](images/fig_fd_overlap.png)

**Finite Element Examples:**

- **Overlap:**  
  ![FE overlap](images/fig_fe_overlap.png)

- **Extended Overlap:**  
  ![FE extended overlap](images/fig_fe_extended_overlap.png)

---

## 2.5 Algebraic Partitioning and Overlap

**Graph-Based Partitioning:**
- Nodes: DOFs
- Edges: $(i,j)$ if $A_{ij} \ne 0$
- Use tools like METIS or SCOTCH.

**Overlap Definition:**
$$
N^\delta_i = N_i \cup \{j : A_{ij} \ne 0\}
$$

**Algebraic Partition of Unity:**
Let $D_i$ be diagonal with:
$$
(D_i)_{jj} = \frac{1}{\#\{i : j \in N^\delta_i\}}, \quad \sum_i R_i^T D_i R_i = I
$$

**FE Subdomain Partitioning:**
If $\varphi_k$ are FE basis functions:
- $N_i = \{k : \text{supp}(\varphi_k) \cap \Omega_i \ne \emptyset\}$
- $\mu_k = \#\{j : \text{supp}(\varphi_k) \cap \Omega_j \ne \emptyset\}$
- $(D_i)_{kk} = 1/\mu_k$ for $k \in N_i$

**Additive Schwarz Iteration:**
$$
U^{m+1}_j = U^m_j + A_j^{-1} R_j(F - AU^m), \quad A_j = R_j A R_j^T
$$

![Overlapping neighborhoods in graph partitioning](images/fig_graph_overlap.png)

![Overlapping FE subdomains](images/fig_fe_subdomains.png)

[Next Lecture → Convergence Analysis](./convergence-analysis.md)
