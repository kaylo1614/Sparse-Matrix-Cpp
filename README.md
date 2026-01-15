# C++ Implementation of Sparse Matrix Operations (基於鏈結串列之稀疏矩陣運算實作)

## 📖 Project Overview (專案概述)

This project presents a robust implementation of Sparse Matrix operations, specifically focusing on **matrix addition** and **multiplication**.

本專案實作了一個穩健的稀疏矩陣運算系統，特別著重於 **矩陣乘法** 的功能。

### Why Linked Lists? (為何選擇鏈結串列？)

In traditional matrix implementations using 2D arrays or `std::vector`, memory is allocated continuously for all $N \times M$ elements. For Sparse Matrices (where most elements are zero), this approach is highly inefficient:

1. **Memory Waste:** Allocating space for millions of zeros consumes unnecessary RAM.

2. **Computational Overhead:** Standard algorithms iterate through every element, wasting CPU cycles on $0 \times x$ or $0 + x$ operations.

**Solution:** This project solves these issues by using **C++ Raw Pointers** and **Linked Lists**. We only store and process non-zero values, significantly reducing the space complexity from $O(N \times M)$ to $O(K)$ (where $K$ is the number of non-zero elements).

在傳統的二維陣列實作中，系統必須為所有 $N \times M$ 個元素配置連續記憶體。對於稀疏矩陣（絕大多數數值為 0）而言，這種方式極無效率：

1. **記憶體浪費**：為大量的 0 配置空間浪費了記憶體資源。

2. **無效運算**：傳統演算法會遍歷所有元素，導致 CPU 浪費時間在處理 0 的運算上。

**解決方案**：本專案透過 **C++ 指標 (Raw Pointers)** 與 **鏈結串列 (Linked List)** 解決此問題。我們僅儲存非零數值，將空間複雜度從 $O(N \times M)$ 大幅降低至 $O(K)$（$K$ 為非零元素個數）。

## 🛠 Skills Demonstrated (技能展示)

This project demonstrates the following technical competencies:
本專案展示了以下核心技術能力：

* **Pointers & Memory Management (指標與記憶體管理)**: Manual allocation/deallocation using `new`/`delete` to construct complex data structures.

* **Data Structures (資料結構實作)**: Custom implementation of Singly Linked Lists specifically designed for matrix logic.

* **Algorithm Optimization (演算法優化)**: Skipping zero-elements during addition/multiplication to enhance performance.

* **Debugging & Refactoring (除錯與重構)**: Identifying and fixing memory allocation bugs in legacy code.

## ⚠️ Author's Note (重要聲明)

> **This codebase was authored entirely by the undersigned two years prior, without the aid of Generative AI technologies.**
>
> The core logic reflects my independent work from two years ago. While the implementation utilizes traditional C++ paradigms—such as raw pointers—it stands as an authentic testament to my capability in algorithmic logic and complex data structure manipulation.

> **本程式碼之核心邏輯為本人於兩年前獨立撰寫，開發過程完全未依賴生成式 AI 技術輔助。**
>
> 儘管程式碼風格反映了當時的實作慣例（例如使用原始指標），但其忠實呈現了本人在演算法邏輯與複雜資料結構上的獨立實作能力。

## 📂 Repository Structure (檔案結構說明)

To provide a comprehensive view of the development capability, this repository includes two versions of the source code:
為完整呈現開發歷程，本儲存庫包含兩個版本的原始碼：

### 1. `SparseMatrix_Original.cpp` (Legacy Version)

* The original code written 2 years ago.

* Contains the raw algorithmic logic without UX enhancements.

* 兩年前撰寫的原始版本，保留了最原汁原味的演算法邏輯。

### 2. `SparseMatrix_Refined.cpp` (Refined Version)

* **UX Improvements**: Added detailed input prompts, error handling, and matrix visualization.

* **Bug Fix (Critical)**: Fixed a memory allocation bug in the `multi()` function.

> **Technical Detail on Bug Fix:**
>
> * **Issue**: In the original code, the row pointer `newnode` was declared outside the loop, causing all rows to point to the same memory address (Shallow Copy issue).
>
> * **Fix**: Moved memory allocation inside the loop to ensure each row has its own independent linked list nodes.

* **修飾版特點**：增加了詳細的輸入引導與視覺化輸出。

* **關鍵修正**：修復了 `multi()` 函式中的記憶體配置錯誤。

  * **問題**：原版程式碼中，節點宣告於迴圈外，導致不同列共用同一塊記憶體位址。

  * **修正**：將記憶體配置移至迴圈內，確保每一列都擁有獨立的鏈結串列節點。

## 🚀 How to Run (如何執行)

### Input Protocol (輸入格式規範)

Users are requested to adhere to the following specifications:
請遵循以下輸入規範：

1. **Matrix Dimensions**: Input Rows and Cols (e.g., `3 3`).

2. **Element Entry**:

   * Format: `[Column Index] [Value]`

   * Constraint: Inputs must be **strictly sorted by Column Index in ascending order (由小到大)**.

3. **Termination**: Input `0` to signal the end of the current row.

## 🧪 Test Cases & Expected Results (測試範例與結果說明)

You can copy and paste the following inputs to verify the program logic.
您可以直接複製以下內容進行測試。

### Case 1: Basic Multiplication (基礎乘法 2x3 * 3x2)

**Input (請複製此區塊):**

```text
2 3
1 1 3 2 0
2 3 0
3 2
2 4 0
1 5 0
2 6 0
```

運算結果 (Output):

```text
  |    0   16 |
  |   15    0 |

  [Linked List Structure]:
  Row ->[2:16] -> NULL
  Row ->[1:15] -> NULL
```
### Case 2: 10x10 Sparse Matrix (稀疏特性展示)

This case demonstrates the bug fix. We multiply two 10x10 matrices where only the corners have values. The middle rows should be completely zero.
此範例展示了 Bug 修正後的正確性。兩個 10x10 矩陣相乘，只有角落有數值。中間的列（Row 2-9）應該完全為 0。

**Input (請複製此區塊):**

```text
10 10
1 99 0
0
0
0
0
0
0
0
0
10 99 0
10 10
1 1 0
0
0
0
0
0
0
0
0
10 1 0
```

運算結果 (Output):

```test
  |   99    0    0    0    0    0    0    0    0    0 |
  |    0    0    0    0    0    0    0    0    0    0 |
  |    0    0    0    0    0    0    0    0    0    0 |
  |    0    0    0    0    0    0    0    0    0    0 |
  |    0    0    0    0    0    0    0    0    0    0 |
  |    0    0    0    0    0    0    0    0    0    0 |
  |    0    0    0    0    0    0    0    0    0    0 |
  |    0    0    0    0    0    0    0    0    0    0 |
  |    0    0    0    0    0    0    0    0    0    0 |
  |    0    0    0    0    0    0    0    0    0   99 |

  [Linked List Structure]:
  Row ->[1:99] -> NULL
  Row -> NULL
  Row -> NULL
  Row -> NULL
  Row -> NULL
  Row -> NULL
  Row -> NULL
  Row -> NULL
  Row -> NULL
  Row ->[10:99] -> NULL
```
