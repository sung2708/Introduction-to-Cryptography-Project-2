# PROJECT 2 - DIFFIE-HELLMAN

## 1. Diffie-Hellman in a Prime Field (5 points)

Recall that an element $g$ in the group $\mathbb{Z}^*_p$, where $p$ is a large prime number, is called a **generator** if it satisfies the following condition:

$$
\forall x \in [1, p - 2], g^x \not\equiv 1 \pmod{p}
$$

Given a large prime number $p$ and a positive integer $g$ that is a generator in the group $\mathbb{Z}^*_p$, Alice and Bob perform the **Diffie-Hellman key exchange** as follows:

1.  **Alice** randomly selects a private key $a \in \mathbb{Z}^*_p$ and sends Bob the public value:
    $$A = g^a \pmod{p}$$
2.  **Bob** randomly selects a private key $b \in \mathbb{Z}^*_p$ and sends Alice the public value:
    $$B = g^b \pmod{p}$$
3.  Using her private key $a$ and Bob’s public value $B = g^b \pmod{p}$, **Alice** computes the shared secret key:
    $$K = B^a = (g^b)^a = g^{ba} \pmod{p}$$
4.  Similarly, using his private key $b$ and Alice’s public value $A = g^a \pmod{p}$, **Bob** computes the same shared secret key:
    $$K = A^b = (g^a)^b = g^{ab} \pmod{p}$$

---

### 1.1 Problem Requirements

In this section, students are required to design a **C++** program to compute the shared secret key between Alice and Bob after performing the **Diffie-Hellman key exchange**. The program must meet the following criteria:

* The program **must be written in C++**. Using any other programming language will result in a **score of 0**.
* Only **standard libraries from C++17 or earlier** may be used. Using any other libraries will result in a **score of 0**.
* The entire source code must be contained in a **single file** named **`bai1.cpp`**.
* After compiling the source code into an executable **`a.exe`**, the program must be executed using the following command:
    ```
    .\a.exe test.inp test.out
    ```
    * `test.inp`: Input file containing test data.
    * `test.out`: Output file where the computed result is stored.
* The **maximum execution time** for each test case is **60 seconds**. If the program exceeds this time limit, the test case will **not be scored**.

---

### 1.2 Input and Output Files

#### Input File: `test.inp`
The input file consists of **three lines**:
1.  The **first line** contains two **positive integers**:
    * A **prime number** $p$
    * A **generator** $g$ in the group $\mathbb{Z}^*_p$
    Both values are represented in **uppercase hexadecimal** format in **big-endian** notation.
2.  The **second line** contains a **positive integer** representing Alice’s **private key** $a \in \mathbb{Z}^*_p$, also in **uppercase hexadecimal** format.
3.  The **third line** contains a **positive integer** representing Bob’s **private key** $b \in \mathbb{Z}^*_p$, also in **uppercase hexadecimal** format.

#### Output File: `test.out`
* The output file contains a **single positive integer** representing the **shared secret key**:
    $$K = g^{ab} \pmod{p}$$
* This result must be stored in **uppercase hexadecimal** format in **big-endian** notation.

---

### 1.3 Example

#### Example 1
**Input (`test.inp`):**
```
65 1D
12
21
```
**Explanation:**
* $0x65 = 101$, $0x1D = 29$, $0x12 = 18$, $0x21 = 33$.
* Given $p = 101$, the shared secret key is computed as:
    $$K = 29^{(18 \times 33)} \pmod{101} = 29^{594} \pmod{101} = 85 = 0x55$$
**Output (`test.out`):**
```
55
```

---

### 1.4 Constraints
* $g, a, b \in \mathbb{Z}^*_p$.
* **40% of test cases**: $|p| \leq 64$ bits.
* **40% of test cases**: $64 < |p| \leq 128$ bits.
* **10% of test cases**: $128 < |p| \leq 256$ bits.
* **10% of test cases**: $256 < |p| \leq 512$ bits.

---

## 2. Diffie-Hellman on Elliptic Curve (5 points)

Given a large prime number $p$, we define an elliptic curve in the form (EC):
$$y^2 \equiv x^3 + ax + b \pmod{p}$$
where $a, b \in \mathbb{F}_p$ are given constants satisfying $4a^3 + 27b^2 \not\equiv 0 \pmod{p}$. Define $E(a, b, p)$ as:
$$E(a, b, p) = \{(x, y) \in \mathbb{F}_p^2 : y^2 \equiv x^3 + ax + b \pmod{p}\} \cup \{O\}$$
which represents the set of points on the elliptic curve with given coefficients $a, b$. The addition operation $+$ on $E(a, b, p)$ is defined as follows:

* $+: E(a, b, p) \times E(a, b, p) \to E(a, b, p)$, meaning that the sum of two points $P, Q \in E(a, b, p)$ results in another point $R \in E(a, b, p)$.
* The **point at infinity** $O$ serves as the identity element: $\forall P \in E(a, b, p), P + O = O + P = P$.
* The **inverse element** of $P = (x_P, y_P)$, denoted $-P$, is $(x_P, -y_P \pmod{p})$.

For two points $P = (x_P, y_P)$ and $Q = (x_Q, y_Q)$ in $E(a, b, p)$, the sum $R = P + Q = (x_R, y_R)$ is computed as follows:

1.  If $P = O$, then $R = Q$. If $Q = O$, then $R = P$.
2.  If $x_P = x_Q$ and $y_P \equiv -y_Q \pmod{p}$, then $P + Q = O$.
3.  Otherwise, let $\lambda$ be the slope of the line passing through $P$ and $Q$:
    * If $P \neq Q$: $\lambda = (y_Q - y_P)(x_Q - x_P)^{-1} \pmod{p}$
    * If $P = Q$: $\lambda = (3x_P^2 + a)(2y_P)^{-1} \pmod{p}$
4.  The coordinates of $R$ are:
    * $x_R = \lambda^2 - x_P - x_Q \pmod{p}$
    * $y_R = \lambda(x_P - x_R) - y_P \pmod{p}$

The elliptic curve group $(E(a, b, p), +)$ forms an **abelian group**. This allows us to define the **elliptic curve discrete logarithm problem (ECDLP)**:

#### Definition 2.1
Given an elliptic curve over a finite field $\mathbb{F}_p$, a point $Q \in E(a, b, p)$, and a generator $G \in E(a, b, p)$, the ECDLP requires finding an integer $x$ such that:
$$Q = xG = \underbrace{G + G + \dots + G}_{x \text{ times}}$$

---

### 2.1 Task Requirements
Design a program to compute $R = P + Q$ for given points $P, Q \in E(a, b, p)$.

* Implemented in **C++**. Any other language results in **0** points.
* Only **standard C++17 libraries** are allowed.
* The entire source code must be in a **single file** named `bai2.cpp`.
* Execution command: `./a.exe test.inp test.out`
* Time limit: **60 seconds** per test case.

### 2.2 Input and Output Files

#### `test.inp`
The input file consists of **three lines**:
1.  The first line contains three positive integers: $p, a, b$ (uppercase hex).
2.  The second line contains two positive integers: $x_P, y_P$ (uppercase hex).
3.  The third line contains two positive integers: $x_Q, y_Q$ (uppercase hex).

#### `test.out`
* The output file contains **two positive integers** representing $R = P + Q = (x_R, y_R)$ in uppercase hex.

### 2.3 Example

**Input (`test.inp`):**
```
7CF 724 68B
212 1CF
6EB 31
```
**Output (`test.out`):**
```
440 D0
```
**Explanation:**
* $p = 0x7CF = 1999$. Curve: $y^2 \equiv x^3 + 1828x + 1675 \pmod{1999}$.
* $P = (530, 463)$, $Q = (1771, 49)$.
* $R = P + Q = (1088, 208) = (0x440, 0xD0)$.

### 2.5 Constraints
* $a, b \in \mathbb{F}_p$.
* Inputs and outputs will **never** include the point at infinity $O$.
* **50% of test cases**: $|p| \leq 64$ bits.
* **30% of test cases**: $64 < |p| \leq 128$ bits.
* **20% of test cases**: $128 < |p| \leq 256$ bits.
