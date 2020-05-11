#include "tensor.hpp"
#include <cmath>

int main()
{
	size_t dimension;

	for (dimension=2; dimension <6; dimension ++){
		vector<size_t> size;
		for (int i=0; i< dimension; i++){
			size.push_back(2);
		}

		Tensor* t = create_tensor(size);
		vector<size_t> index(size.size());

		t->write(index,0);
		int i=1;
		for (; next_index(size,index);i++){
			t->write(index,i);

			if (t->read(index) != i) break;
		}

		if(i == pow(2,dimension)){
			cout << "Pass " << dimension << endl;
		}

	}


}