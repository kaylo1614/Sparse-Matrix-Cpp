#include<iostream>
#include<cstdlib>
#include<iomanip> // [新增] 用於設定輸出寬度 setw，讓矩陣對齊
using namespace std;

// ==========================================
// 專案說明：稀疏矩陣 (Sparse Matrix) 實作 (基於鏈結串列)
// 作者備註：核心邏輯為兩年前實作 (No AI)。
//           此版本優化了排版對齊與使用者體驗 (UX)。
// 
// [快速測試範例 Quick Usage Example]
// 測試情境：建立兩個 2x2 矩陣相乘
// 
// 輸入 (可以直接複製貼上到 Console):
// 2 2        (A 大小)
// 1 5 0      (A Row 1: Col 1 放 5)
// 2 8 0      (A Row 2: Col 2 放 8)
// 2 2        (C 大小)
// 1 1 0      (C Row 1: Col 1 放 1)
// 2 1 0      (C Row 2: Col 2 放 1)
//
// 預期結果: A * C 應為原矩陣 A (因為 C 是單位矩陣)
// ==========================================

// 定義稀疏矩陣的節點結構
class linknode {
	friend class linkMatrix;

public:
	// 建構子：初始化節點的值、右鏈接和下鏈接
	linknode(int element = 0, linknode* next = 0, linknode* dnext = 0) {
		value = element;
		rightlink = next;
		downlink = dnext;
		colPlace = 0;
	}
	// 乘法運算的輔助函式：建立新節點
	linknode* multiNode(int n, linknode* A) {
		linknode* X = new linknode;
		linknode* Y = X;
		while (A->value != 0) {
			X->value = n * A->value;
			X->colPlace = A->colPlace;
			A = A->rightlink;
			X->rightlink = new linknode;
			X = X->rightlink;
		}
		X->colPlace = A->colPlace;
		return(Y);
	}
	// 加法運算的輔助函式
	linknode* addNode(int n, linknode* A, linknode* B);

private:
	int value;              // 節點數值
	int colPlace;           // 行 (Column) 位置
	linknode* rightlink;    // 指向同一列的下一個非零元素
	linknode* downlink;     // 指向下一列的開頭
};

// 定義稀疏矩陣類別
class linkMatrix {
public:
	linkMatrix() { this->row = 0; this->col = 0; this->first = new linknode; }
	linkMatrix(int m, int n) { row = m; col = n; first = new linknode; }
	linkMatrix(int m, int n, linknode* k) { row = m; col = n; first = k; }
	~linkMatrix();          // 解構子
	void createMatrix();    // 建立矩陣 (包含輸入邏輯)
	void printM();          // 印出矩陣內容
	void Add(linkMatrix A, linkMatrix B);   // 矩陣相加
	void multi(linkMatrix A, linkMatrix B); // 矩陣相乘

	linknode* getNode() { return first; }
	int getRow() { return this->row; }
	int getCol() { return this->col; }
private:
	int row, col;           // 矩陣的列與行數
	linknode* first;        // 頭節點指標
};

// [新增] 跨平台暫停函式，解決線上編譯器無法執行 system("PAUSE") 的問題
void portablePause() {
	cout << "\n[程式執行完畢] 請按 Enter 鍵離開...";
	// 清除輸入緩衝區，避免前面的 Enter 被誤讀
	cin.ignore(1024, '\n'); 
	cin.get(); 
}

int main()
{
	int Am = 0, An = 0;
	int Cm = 0, Cn = 0;

	cout << "=============================================" << endl;
	cout << "   C++ Sparse Matrix Operation (Linked List) " << endl;
	cout << "=============================================" << endl << endl;

	// --- 步驟 1: 設定矩陣 A ---
	cout << "---------------------------------------------" << endl;
	cout << "[步驟 1] 設定矩陣 A" << endl;
	cout << "請輸入矩陣 A 的大小 (Row Col): ";
	cin >> Am >> An;
	linkMatrix A(Am, An);
	A.createMatrix();
	
	cout << "\n-> Matrix A 結果 (Matrix View): " << endl;
	A.printM();
	cout << endl;

	// --- 步驟 2: 設定矩陣 C ---
	cout << "---------------------------------------------" << endl;
	cout << "[步驟 2] 設定矩陣 C (用於與 A 相乘)" << endl;
	cout << "請輸入矩陣 C 的大小 (Row Col): ";
	cin >> Cm >> Cn;
	linkMatrix C(Cm, Cn);
	C.createMatrix();

	cout << "\n-> Matrix C 結果 (Matrix View): " << endl;
	C.printM();

	// --- 步驟 3: 執行乘法 ---
	cout << "---------------------------------------------" << endl;
	cout << "[步驟 3] 計算 A * C" << endl;
	// 檢查維度是否符合乘法規則 (A的行數必須等於C的列數)
	if ((A.getCol() == C.getRow())) {
		linkMatrix Y(A.getRow(), C.getCol());
		Y.multi(A, C);

		cout << "\n-> 運算結果 (Result):" << endl;
		Y.printM();
	}
	else {
		cout << "\n[錯誤] 維度不符！ Matrix A 的行數 (" << A.getCol() << ") 必須等於 Matrix C 的列數 (" << C.getRow() << ")。" << endl;
	}

	portablePause();
	return 0;
}

// [修正後] 正確的解構子：遍歷整個矩陣釋放記憶體
linkMatrix::~linkMatrix() {
    linknode* rowHead = first;
    
    while (rowHead != nullptr) {
        linknode* currentRowNode = rowHead;       // 鎖定這一列的開頭
        linknode* nextRowHead = rowHead->downlink; // 先記住下一列在哪裡，以免刪了找不到路

        // 水平遍歷：刪除這一列的所有節點
        while (currentRowNode != nullptr) {
            linknode* temp = currentRowNode;
            currentRowNode = currentRowNode->rightlink; // 往右移動
            delete temp; // 釋放當前節點
        }

        // 往下移動到下一列
        rowHead = nextRowHead;
    }
    // cout << "[System] Memory safely released." << endl; 
}

void linkMatrix::createMatrix() {
	linknode* rowTemp = this->first;
	linknode* colTemp = this->first;
	linknode* newnode = new linknode;

	for (int ii = 0; ii < this->row; ii++) {
		rowTemp->downlink = newnode;
		rowTemp = rowTemp->downlink;
		newnode = new linknode;
	}

	int tempC, val, tempR = 0;
	
	// 優化後的中文輸入說明，防止使用者輸入錯誤
	cout << " > 輸入格式: [Column] [Value] (輸入 0 換下一列)" << endl;
	cout << " > 注意事項: Column 請務必【由小到大】輸入！" << endl;

	rowTemp = this->first;
	while (tempR != row) {
		cout << "   Row " << setw(2) << tempR + 1 << " 輸入: "; // setw 讓數字對齊
		cin >> tempC;
		
		if (tempC == 0) { // 使用者輸入 0 代表換行
			colTemp->colPlace = col + 1;
			colTemp->rightlink = newnode;
			newnode = new linknode;
			tempR++;
			rowTemp = rowTemp->downlink;
			colTemp = rowTemp;
		}
		else {
			cin >> val;
			if (val) {
				colTemp->value = val;
				colTemp->colPlace = tempC;
				colTemp->rightlink = newnode;
				newnode = new linknode;
				colTemp = colTemp->rightlink;
			}
		}
	}
	delete newnode;
}

linknode* linknode::addNode(int n, linknode* A, linknode* B) {
	linknode* X = new linknode;
	linknode* start = X;
	while ((A->colPlace <= n) || (B->colPlace <= n)) {

		if (A->colPlace == B->colPlace) {
			X->value = A->value + B->value;
			X->colPlace = B->colPlace;
			A = A->rightlink;
			B = B->rightlink;
		}
		else if (A->colPlace < B->colPlace) {
			X->value = A->value;
			X->colPlace = A->colPlace;
			A = A->rightlink;
		}
		else {
			X->value = B->value;
			X->colPlace = B->colPlace;
			B = B->rightlink;
		}

		if (X->value != 0) {
			X->rightlink = new linknode;
			X = X->rightlink;
		}
	}
	X->colPlace = n + 1;
	X->rightlink = new linknode;
	return(start);
}

void linkMatrix::Add(linkMatrix A, linkMatrix B) {
	linknode* tempA = A.getNode();
	linknode* tempB = B.getNode();

	linknode* temp = new linknode;
	first = temp;
	linknode X;

	for (int ii = 0; ii < this->row; ii++) {
		temp->downlink = X.addNode(this->col, tempA, tempB);
		tempA = tempA->downlink;
		tempB = tempB->downlink;
		temp = temp->downlink;
	}
	temp->downlink = new linknode;
	first = first->downlink;
}

// 修正後的乘法函式
void linkMatrix::multi(linkMatrix A, linkMatrix B) {
	linknode* tempA = A.getNode();
	linknode* tempB = B.getNode();
	linknode* tempRowA = A.getNode();
	linknode* tempRowB = B.getNode();

	int multiNum = A.getCol();
	
	// Y 是用來暫存運算結果的矩陣
	linkMatrix Y(A.getRow(), B.getCol());
	linknode* tempY = Y.getNode();
	
	// [修正 1] 這裡先幫第一列建立起頭
	tempY->downlink = new linknode;

	this->first = new linknode;
	// linknode* rowTemp = this->first; // 這行其實沒用到，可以拿掉
	// linknode* newnode = new linknode; // [修正 2] 移除這行，不要在迴圈外宣告

	for (int jj = 1; jj <= A.getRow(); jj++) {
		tempA = tempRowA;
		tempRowB = B.getNode();

		tempY->downlink->colPlace = B.getCol() + 1;

		for (int ii = 1; ii <= multiNum; ii++) {
			if (ii == tempA->colPlace) {
				tempB = tempRowB;
				linknode t;
				tempY->downlink = t.addNode(B.getCol(), tempY->downlink, t.multiNode(tempA->value, tempB));
				tempA = tempA->rightlink;
			}
			tempRowB = tempRowB->downlink;
		}
		tempRowA = tempRowA->downlink;

		tempY = tempY->downlink;
		
		// [修正 3] 關鍵修正！每一列結束後，都要 new 一個「全新」的節點給下一列使用
		// 這樣才不會讓不同列共用同一個記憶體
		tempY->downlink = new linknode; 
	}
	first = Y.getNode()->downlink;
}

void linkMatrix::printM() {
	linknode* rowTemp = this->first;
	linknode* colTemp = rowTemp;

	for (int ii = 1; ii <= this->row; ii++) {
		cout << "  |";
		for (int jj = 1; jj <= this->col; jj++) {
			if (jj < colTemp->colPlace) {
				cout << setw(5) << "0";
			}
			else {
				cout << setw(5) << colTemp->value;
				colTemp = colTemp->rightlink;
			}
		}
		cout << " |" << endl;
		rowTemp = rowTemp->downlink;
		colTemp = rowTemp;
	}

	cout << "\n  [Linked List Structure]:" << endl;
	for (linknode* tempR = this->first; tempR != nullptr && tempR->downlink != nullptr; tempR = tempR->downlink) {
		cout << "  Row ->";
		for (linknode* tempC = tempR; tempC != nullptr && tempC->rightlink != nullptr; tempC = tempC->rightlink) {
			if (tempC->value != 0)
				cout << "[" << tempC->colPlace << ":" << tempC->value << "] ->";
		}
		cout << " NULL" << endl;
	}
}
