# PROJECT 2 - DIFFIE HELLMAN

## 1. Diffie-Hellman in a Prime Field (5 points)

Recall that an element $g$ in the group $\mathbb{Z}^*_p$, where $p$ is a large prime number, is called a **generator** if it satisfies the following condition:

$$
\forall x \in [1, p - 2], g^x \neq 1 \pmod{p}
$$

Given a large prime number $p$ and a positive integer $g$ that is a generator in the group $\mathbb{Z}^*_p$, Alice and Bob perform the **Diffie-Hellman key exchange** as follows:

1. **Alice** randomly selects a private key $a \in \mathbb{Z}^*_p$ and sends Bob the public value:

$$
A = g^a \mod p
$$

2. **Bob** randomly selects a private key $b \in \mathbb{Z}^*_p$ and sends Alice the public value:

$$
B = g^b \mod p
$$

3. Using her private key $a$ and Bob’s public value $B = g^b \mod p$, **Alice** computes the shared secret key:
   
$$
K = B^a = g^{ab} \mod p
$$

5. Similarly, using his private key $b$ and Alice’s public value $A = g^a \mod p$, **Bob** computes the same shared secret key:

$$
K = A^b = g^{ab} \mod p
$$

---

## 1.1 Problem Requirements

In this section, students are required to design a **C++** program to compute the shared secret key between Alice and Bob after performing the **Diffie-Hellman key exchange**. The program must meet the following criteria:

- The program **must be written in C++**. Using any other programming language will result in a **score of 0**.
- Only **standard libraries from C++17 or earlier** may be used. Using any other libraries will result in a **score of 0**.
- The entire source code must be contained in a **single file** named **`bai1.cpp`**.
- After compiling the source code into an executable **`a.exe`**, the program must be executed using the following command:

  ```
  .\a.exe test.inp test.out
  ```

  - `test.inp`: Input file containing test data.
  - `test.out`: Output file where the computed result is stored.

- The **maximum execution time** for each test case is **60 seconds**. If the program exceeds this time limit, the test case will **not be scored**.

---

## 1.2 Input and Output Files

### Input File: `test.inp`
The input file consists of **three lines**:

1. The **first line** contains two **positive integers**:  
   - A **prime number** $p$  
   - A **generator** $g$ in the group $\mathbb{Z}^*_p$  
   Both values are represented in **uppercase hexadecimal** format in **big-endian** notation.

2. The **second line** contains a **positive integer** representing Alice’s **private key** $a \in \mathbb{Z}^*_p$, also in **uppercase hexadecimal** format.

3. The **third line** contains a **positive integer** representing Bob’s **private key** $b \in \mathbb{Z}^*_p$, also in **uppercase hexadecimal** format.

### Output File: `test.out`
- The output file contains a **single positive integer** representing the **shared secret key**:

$$
K = g^{ab} \mod p
$$

- This result must be stored in **uppercase hexadecimal** format in **big-endian** notation.

---

## 1.3 Example

### Example 1
#### Input (`test.inp`):
```
65 1D
12
21
```

#### Explanation:
- $0x65 = 101$, $0x1D = 29$, $0x12 = 18$, $0x21 = 33$.
- Given $p = 101$, the shared secret key is computed as:

$$
K = 29^{(18 \times 33)} \mod 101 = 85 = 0x55
$$

#### Output (`test.out`):
```
55
```

---

### Example 2
#### Input (`test.inp`):
```
67 2B
1A
41
```

#### Explanation:
- $0x67 = 103$, $0x2B = 43$, $0x1A = 26$, $0x41 = 65$.
- Given $p = 103$, the shared secret key is computed as:

$$
K = 43^{(26 \times 65)} \mod 103 = 19 = 0x13
$$

#### Output (`test.out`):
```
13
```

---

## 1.4 Constraints

- $g, a, b \in \mathbb{Z}^*_p$.
- **40% of test cases**: $|p| \leq 64$ bits (40% of total score).
- **40% of test cases**: $64 < |p| \leq 128$ bits (40% of total score).
- **10% of test cases**: $128 < |p| \leq 256$ bits (10% of total score).
- **10% of test cases**: $256 < |p| \leq 512$ bits (10% of total score).

# 2. Diffie-Hellman on Elliptic Curve (5 points)

Given a large prime number $p$, we define an elliptic curve in the form (EC):

$y^2 = x^3 + ax + b \quad (a, b \in \mathbb{F}_p)$

where $a, b$ are given constants satisfying $4a^3 + 27b^2 \neq 0$. Define $E(a, b, p)$ as:

$E(a, b, p) = \{(x, y) \in \text{(EC)} : y^2 = x^3 + ax + b \in \mathbb{F}_p\}$

which represents the set of points on the elliptic curve with given coefficients $a, b$. The addition operation $+$ on $E(a, b, p)$ is defined as follows:

- $+ : E(a, b, p) \times E(a, b, p) \to E(a, b, p)$, meaning that the sum of two points $P, Q \in E(a, b, p)$ results in another point $R \in E(a, b, p)$.
- We introduce the **point at infinity** $O$ such that $\forall P \in E(a, b, p), we have:

$$ P + O = O + P = P $$

  Thus, $O$ is the identity element in $E(a, b, p)$.
- The **inverse element** of $P$, denoted $-P$, is the reflection of $P$ across the x-axis.

For two points $P, Q \in E(a, b, p)$, the sum $R = P + Q$ is computed as follows:

1. Find the line $d: y = cx + d$ passing through $P$ and $Q.
2. Find the intersection point $Y$ between the elliptic curve (EC) and the line d.
3. Define $R = P + Q = -Y$ as the result of addition on the elliptic curve.

The coefficients $a, b$ satisfy $4a^3 + 27b^2 \neq 0$, ensuring that the line $d$ through two points $P$ and $Q$ will intersect the elliptic curve at a third point $Y$ distinct from $P$ and $Q$. If $d$ is vertical (i.e., $P$ and $Q$ are symmetric across the x-axis), then $d$ intersects the curve at the **point at infinity** $O$, meaning that $P + Q = -O$.

- The definition of $O$ as the identity element is justified since:

$$ P + O = P \quad \text{and} \quad P + (-P) = -O. $$

- When computing $2P = P + P$, the line $d$ is the tangent to the curve at $P$, and the computation follows the same steps.

The elliptic curve group $(E(a, b, p), +)$ forms an **abelian group**. This allows us to define the **elliptic curve discrete logarithm problem (ECDLP)**, analogous to the discrete logarithm problem in $\mathbb{Z}^*_p$:

### Definition 2.1
Given an elliptic curve (EC) over a finite field $\mathbb{F}_p$, a point $Q \in E(a, b, p)$, and a generator $G \in E(a, b, p)$, the elliptic curve discrete logarithm problem requires finding an integer $x \in \mathbb{Z}^*_p$ such that:

$$ Q = xG = G + G + ... + G \quad (x \text{ times}) $$

assuming at least one solution exists.

- This problem is considered hard (infeasible for polynomial-time algorithms) when $p$ is a sufficiently large prime (e.g., $p \geq 2^{256}$).
  ![Figure 1: Some graphical representations of elliptic curves, where the curve $y^2 = x^3$ is not an elliptic curve.](https://scontent.fsgn16-1.fna.fbcdn.net/v/t1.15752-9/482140214_502165489362666_5559469188283386762_n.png?_nc_cat=100&ccb=1-7&_nc_sid=9f807c&_nc_eui2=AeHDNPnFC5BJYTDk2jpPfMKFjNEqOHub2faM0So4e5vZ9m-_li7FK3s8fnVOdTNOeKV6K03jx2xT7MWTX6ZV3yxs&_nc_ohc=p30F5YHaJosQ7kNvgGmTThG&_nc_oc=Adi7s0hpA0g7igBYqDbsIm3rr2RPabehkzdDoLccfDMfvHOOSdP1YB93A3BKzXhJ6prndDa6QFsoglBYFGUxh6wy&_nc_zt=23&_nc_ht=scontent.fsgn16-1.fna&oh=03_Q7cD1gGyK-YKsqzd6PA6wqCMacgYve6hIQXrSEVgQEIzE7OZKg&oe=67EDFAEB)
  Figure 1: Some graphical representations of elliptic curves, where the curve $y^2 = x^3$ is not an elliptic curve.

## 2.1 Task Requirements
In this task, students must design a program to compute $R = P + Q$ for given points $P, Q \in E(a, b, p)$. The program must satisfy the following requirements:

- Implemented in **C++**. Any other programming language will receive a score of **0**.
- Only **standard C++17 libraries** are allowed. Using non-standard libraries results in a score of **0**.
- The entire source code must be in a **single file** named `bai2.cpp`.
- The compiled program must be executed using:

  ```sh
  ./a.exe test.inp test.out
  ```

  where `test.inp` is the input file and `test.out` is the output file.
- The execution time for each test case must not exceed **60 seconds**.

## 2.2 Input and Output Files

### `test.inp`
The input file consists of **three lines**:
1. The first line contains three positive integers: the prime number $p$ and the coefficients $a, b$ defining the elliptic curve.
2. The second line contains two positive integers representing the point $P \in E(a, b, p)$ as $(x_P, y_P)$.
3. The third line contains two positive integers representing the point $Q \in E(a, b, p)$ as $(x_Q, y_Q)$.

All values are given in **uppercase hexadecimal format** in **big-endian** order.

### `test.out`
The output file contains **two positive integers** representing the point $R = P + Q \) as \( (x_R, y_R)$, also stored in **uppercase hexadecimal format** in **big-endian** order.

## 2.3 Example

### Example Input (`test.inp`)
```
7CF 724 68B
440 D0
212 1CF
```

### Example Output (`test.out`)
```
440 D0
```

#### Explanation
- Given $p = 0x7CF = 1999$, the elliptic curve equation is:
  $y^2 = x^3 + 1828x + 1675 \mod 1999$
- The points $P = (0x212, 0x1CF) = (530, 463)$ and $Q = (0x6EB, 0x31) = (1771, 49)$ are on the curve.
- The computed sum is $R = (1088, 208) = (0x440, 0xD0)$.

## 2.5 Constraints
- $a, b \in \mathbb{Z}^*_p$.
- The input and output will **never** include the point at infinity $O$.
- **50% of test cases**: $|p| \leq 64$ bits.
- **30% of test cases**: $64 < |p| \leq 128$ bits.
- **20% of test cases**: $128 < |p| \leq 256$ bits.

---
This problem requires implementing **elliptic curve point addition** while handling arithmetic in a finite field efficiently within the given constraints.
