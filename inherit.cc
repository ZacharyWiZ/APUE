
#include <iostream>

using namespace std;

class A {
	
public:
	virtual void p() {
		cout << "A" <<endl;
	}
};

class B : public A {

public : 
	virtual void p() {
		cout<<"B"<<endl;
	}
};

class C: public A {

public : 
	virtual void p() {
		cout<<"C"<<endl;
	}
};


int main() {
	A *a = new A();
	a->p();
	
	B *c = new C();
	c->p();

	return 0;
}
