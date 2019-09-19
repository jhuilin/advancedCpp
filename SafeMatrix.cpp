#include<iostream>
#include<cstdlib>
using namespace std;

template <typename T>
class SafeMatrix {
	int low_r, high_r, low_c, high_c;
	int rows, cols;
	T** p;

public:

	SafeMatrix() {
		high_r = -1;
		high_c = -1;
    	low_r = 0;
        low_c = 0;
		rows = 0;
		cols = 0;
		p = NULL;
	}

	SafeMatrix(int lr, int hr, int lc, int hc) {
		if (hr < lr || hc < lc) {
			cout << "4 parameters constructor error in bounds definition" << endl;
			exit(1);
		}
		low_r = lr;
		high_r = hr;
		low_c = lc;
		high_c = hc;
		rows = hr - lr + 1;
		cols = hc - lc + 1;
		p = new T * [rows];
		for (int i = 0; i < rows; ++i) {
			p[i] = new T[cols];
		}
	}

	SafeMatrix(int hr, int hc) {
		if (hr <= 0 || hc <= 0) {
			cout << "2 parameters constructor error in bounds definition" << endl;
			exit(1);
		}
		low_r = 0;
		high_r = hr-1;
		low_c = 0;
		high_c = hc-1;
		rows = hr;
		cols = hc;
		p = new T * [rows];
		for (int i = 0; i < rows; ++i) {
			p[i] = new T[cols];
		}
	}

	SafeMatrix(const SafeMatrix<T>& m) {
		low_r = m.low_r;
		high_r = m.high_r;
		low_c = m.low_c;
		high_c = m.high_c;
		rows = m.rows;
		cols = m.cols;
		p = new T * [rows];
		for (int i = 0; i < rows; ++i) {
			p[i] = new T[cols];
			for (int j = 0; j < cols; ++j) {
				p[i][j] = m.p[i][j];
			}
		}
	}

	~SafeMatrix() {
		for (int i = 0; i < rows; ++i) {
			delete[] p[i];
		}
		delete[] p;
	}

	friend class Brackets;

	class Brackets{
		T* ip;
		SafeMatrix<T> s;
	public:
		Brackets(T* ip1, SafeMatrix<T> sm) : ip(ip1), s(sm) { }

		T& operator[](int j) {
			if (j < s.low_c || j > s.high_c) {
				cout << "index " << j << " out of range" << endl;
				exit(1);
			}
			return ip[j-s.low_c];
		}
	};

	Brackets operator[](int i) {
		if (i < low_r || i > high_r) {
			cout << "index " << i << " out of range" << endl;
			exit(1);
		}
		return Brackets(p[i-low_r], *this);
	}


	SafeMatrix<T>& operator=(const SafeMatrix<T> & s) {
		if (this != &s){
			for (int i = 0; i < rows; ++i) {
				delete[] p[i];
			}
			delete[] p;
			p = new T * [s.rows];
			for (int i = 0; i < s.rows; ++i) {
				p[i] = new T[s.cols];
				for (int j = 0; j < s.cols; ++j) {
					p[i][j] = s.p[i][j];
				}
			}
			low_r = s.low_r;
			high_r = s.high_r;
			low_c = s.low_c;
			high_c = s.high_c;
			rows = s.rows;
			cols = s.cols;
		}
		return *this;
	}

	SafeMatrix<T> operator*(const SafeMatrix<T>& s) {
		if (cols == s.rows) {
            SafeMatrix<T> t(rows, s.cols);
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < s.cols; ++j) {
                    t.p[i][j] = 0;
                    for (int k = 0; k < cols; ++k) {
                        t.p[i][j] += p[i][k] * s.p[k][j];
                    }
                }
            }
            return t;
        }else{
            cout << "multiplication error." << endl;
            exit(1);
        }
	}

	template <class U>
	friend ostream& operator<<(ostream& os, SafeMatrix<U> m);
};

template<typename U>
ostream& operator<<(ostream& os, SafeMatrix<U> m) {
	for (int i = 0; i < m.rows; ++i) {
		for (int j = 0; j < m.cols; ++j) {
			cout << m.p[i][j] << " ";
		}
		cout << endl;
	}
	return os;
};

int main() {
	SafeMatrix<int> a(0,1,0,2);
	a[0][0] = 3;
	a[0][1] = 5;
	a[0][2] = 7;
    a[1][0] = 1;
	a[1][1] = 9;
	a[1][2] = 4;
	cout <<"matrix a "<<endl;
    cout <<a<< endl;
    cout<<"------------------"<<endl;

	SafeMatrix<int> b(3,5);
	b[0][0] = 3;
	b[0][1] = 4;
	b[0][2] = 6;
	b[0][3] = 5;
	b[0][4] = 17;
	b[1][0] = 2;
	b[1][1] = 3;
	b[1][2] = 9;
	b[1][3] = 4;
	b[1][4] = 9;
	b[2][0] = 6;
	b[2][1] = 14;
	b[2][2] = 1;
	b[2][3] = 3;
	b[2][4] = 3;
	cout <<"matrix b "<<endl;
    cout <<b<< endl;
    cout<<"------------------"<<endl;
	SafeMatrix<int> c = a * b;
	cout <<"matrix c = a*b "<<endl;
    cout <<c<< endl;
    cout<<"------------------"<<endl;

    SafeMatrix<int> d(3,3);
    d[0][0] = 11;
	d[0][1] = 9;
	d[0][2] = 7;
    d[1][0] = 12;
	d[1][1] = 6;
	d[1][2] = 7;
	d[2][0] = 18;
	d[2][1] = 9;
	d[2][2] = 7;
	cout <<"matrix d "<<endl;
    cout <<d<< endl;
    cout<<"------------------"<<endl;
	SafeMatrix<int> x = d * b;
	cout <<"matrix x = d*b "<<endl;
    cout <<x<< endl;
    cout<<"------------------"<<endl;
    return 0;
}
