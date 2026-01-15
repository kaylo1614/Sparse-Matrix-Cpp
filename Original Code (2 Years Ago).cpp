#include<iostream>
using namespace std;

// 定義稀疏矩陣的節點結構
class linknode {
	friend class linkMatrix;

public:
	// 建構子，用於初始化節點的值、右鏈接和下鏈接
	linknode(int element = 0, linknode* next = 0, linknode* dnext = 0) {
		value = element;
		rightlink = next;
		downlink = dnext;
		colPlace = 0;
	}
	linknode* multiNode(int n, linknode* A) {
		linknode* X =new linknode;
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
	linknode* addNode(int n,linknode* A, linknode* B);

private:
	int value;				// 節點的值
	int colPlace;			// colum 資訊
	linknode* rightlink;	// 右鏈接
	linknode* downlink;		// 下鏈接
};

// 定義稀疏矩陣類別
class linkMatrix {
public:
	linkMatrix() { this->row = 0; this->col = 0; this->first = new linknode; }
	linkMatrix(int m, int n) { row = m; col = n; first = new linknode;}
	linkMatrix(int m, int n, linknode* k) { row = m; col = n; first = k; }
	~linkMatrix();				// 解構子，用於釋放動態分配的記憶體(把數值為0的位置刪除)
	void createMatrix();		// 創建稀疏矩陣
	void printM();				// 印出稀疏矩陣
	void Add(linkMatrix A, linkMatrix B);	// 矩陣相加
	void multi(linkMatrix A, linkMatrix B);	// 矩陣相乘

	linknode* getNode() { return first; }
	int getRow() { return this->row; }
	int getCol() { return this->col; }
private:
	int row, col;		// 矩陣的行和列
	linknode* first;	// 稀疏矩陣的第一個節點
};

int main()
{
	int Am = 0, An = 0;
	int Bm = 0, Bn = 0;
	int Cm = 0, Cn = 0;

	// 創建稀疏矩陣 A、B 和 C，並輸入其值
	cout << "矩陣A大小:" << endl;
	cin >> Am >> An;
	linkMatrix A(Am, An);
	A.createMatrix();
	cout << "matrix A: " << endl;
	A.printM();

	cout << endl;
	/*
	cout << "矩陣B大小:" << endl;
	cin >> Bm >> Bn;
	linkMatrix B(Bm, Bn);
	B.createMatrix();
	cout << "matrix B: " << endl;
	B.printM();

	cout << endl;

	// 檢查矩陣 A 和 B 的大小是否允許相加，然後進行相加運算
	if ((A.getRow() == B.getRow()) && (A.getCol() == B.getCol())) {

		linkMatrix X(A.getRow(), A.getCol());
		X.Add(A, B);
		cout << endl << "A + B:" << endl;
		X.printM();
	}
	else {

		cout << endl << "A + B:" << endl;
		cout << "A cannot add to B!\n";
	}*/


	cout << "矩陣C大小:" << endl;
	cin >> Cm >> Cn;
	linkMatrix C(Cm, Cn);
	C.createMatrix();
	cout << "matrix C: " << endl;
	C.printM();

	//檢查矩陣 A 和 C 的大小是否允許相乘，然後進行相乘運算
	if ((A.getCol() == C.getRow())) {

		linkMatrix Y(A.getRow(), C.getCol());
		Y.multi(A, C);

		cout << endl << "A X C:" << endl;
		Y.printM();
	}
	else {

		cout << endl << "A X C:" << endl;
		cout << "A cannot multiply to B!\n";
	}
	

	system("PAUSE");
	return 0;
}



linkMatrix::~linkMatrix() {
	delete first;
}

void linkMatrix::createMatrix() {

	//給定第一個row
	linknode* rowTemp = this->first;
	linknode* colTemp = this->first;
	linknode* newnode = new linknode;

	for (int ii = 0; ii < this->row; ii++) {
		rowTemp->downlink = newnode;
		rowTemp = rowTemp->downlink;
		newnode = new linknode;
	}

	int tempC, val, tempR = 0;
	cout << "輸入矩陣:" << endl;

	rowTemp = this->first;
	while (tempR != row){
		cin >> tempC;
		if (tempC == 0) {
			colTemp->colPlace = col + 1;//表示之後沒有值
			colTemp->rightlink = newnode;
			newnode = new linknode;
			tempR++;
			rowTemp = rowTemp->downlink;
			colTemp = rowTemp;
		}
		else {
			cin >> val;
			if(val){
				colTemp->value = val;
				colTemp->colPlace = tempC;//用來儲存列的位置資訊
				colTemp->rightlink = newnode;
				newnode = new linknode;
				colTemp = colTemp->rightlink;
			}
		}
	}
	delete newnode;
}

linknode* linknode::addNode(int n, linknode* A, linknode* B) {
	linknode* X=new linknode;
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

		temp->downlink =X.addNode(this->col, tempA, tempB);//卡在把指標轉成物件
		tempA = tempA->downlink;
		tempB = tempB->downlink;
		temp = temp->downlink;
	}
	temp->downlink= new linknode;
	first = first->downlink;
}


//乘法
void linkMatrix::multi(linkMatrix A, linkMatrix B) {
	linknode* tempA = A.getNode();
	linknode* tempB = B.getNode();
	linknode* tempRowA = A.getNode();
	linknode* tempRowB = B.getNode();

	int multiNum = A.getCol();
	linkMatrix Y(A.getRow(), B.getCol());
	linknode* tempY = Y.getNode();
	tempY->downlink = new linknode;

	this->first = new linknode;
	linknode* rowTemp = this->first;
	linknode* colTemp = this->first;
	linknode* newnode = new linknode;

	for(int jj=1;jj<= A.getRow();jj++){
		tempA = tempRowA;
		tempRowB = B.getNode();	

		tempY->downlink->colPlace = B.getCol() + 1;

		for (int ii = 1; ii <= multiNum; ii++) {
			if (ii == tempA->colPlace) {
				tempB = tempRowB;
				linknode t;
				tempY->downlink= t.addNode(B.getCol(), tempY->downlink, t.multiNode(tempA->value, tempB));

				tempA = tempA->rightlink;
			}
			tempRowB = tempRowB->downlink;
		}
		tempRowA = tempRowA->downlink;

		tempY = tempY->downlink;
		tempY->downlink = newnode;
	}
	first = Y.getNode()->downlink;
}


void linkMatrix::printM() {
	linknode* rowTemp = this->first;
	linknode* colTemp = rowTemp;

	for(int ii=1;ii<=this->row;ii++){
		for (int jj = 1; jj <= this->col; jj++) {

			if (jj < colTemp->colPlace) {
				cout << "0 ";
			}
			else {
				cout << colTemp->value<<" ";
				colTemp = colTemp->rightlink;
			}
		}
		cout << endl;
		rowTemp = rowTemp->downlink;
		colTemp = rowTemp;
	}

	//輸出儲存結構

	cout << "struct type of Matrix=" << endl;
	for (linknode* tempR = this->first; tempR->downlink != nullptr; tempR = tempR->downlink) {
		cout << "row";
		for (linknode* tempC = tempR; tempC->rightlink != nullptr; tempC = tempC->rightlink) {
			cout << tempC->value << "->";
		}
		cout <<endl;
		
	}

}
