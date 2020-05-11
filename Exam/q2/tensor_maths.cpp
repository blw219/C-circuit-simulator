
#include "tensor.hpp"
#include "order_n_tensor.hpp"

Tensor *multiply(const Tensor *a, const Tensor *b)
{
    Tensor* C;
    if(a->size().size()==2 && b->size().size()==2){
    assert(a->size()[1]==b->size()[0]);
        size_t row = a->size()[0];
        size_t column = b->size()[1];
        vector<size_t> c = {row,column};
        C = create_tensor(c);
        vector<size_t> indexC(c.size());
        vector<size_t> indexB(b->size().size());
        vector<size_t> indexA(a->size().size());
        float acc=0;
        for(size_t i=0;i<row;i++){
            for(size_t j=0;j<column;j++){
                indexC={i,j};
                acc=0;
                for(size_t k=0; k <= column; k++){
                    indexA={i,k};
                    indexB={k,j};
                    acc+=((a->read(indexA))*(b->read(indexB)));
                }
                C->write(indexC,acc);
            }
        }
        return C;
    }else if(a->size().size()==3 && b->size().size()==2){
        vector<size_t> size = {a->size()[0],a->size()[1],b->size()[1]};
        C = create_tensor(size);

        for (size_t ia1 = 0; ia1<size[0]; ia1++){
            for (size_t ia2=0; ia2<size[1]; ia2++){
                for (size_t ib2=0; ib2<size[2]; ib2++){
                    float tmp =0;
                    for (size_t k=0; k<b->size()[0]; k++){
                        tmp += (a->read({ia1,ia2,k}))*(b->read({k,ib2}));
                    }
                    C->write({ia1,ia2,ib2},tmp);
                }
            }
        }
        
        return C;
    }else if(a->size().size()==3 && b->size().size()==3){
    assert(a->size()[2]==b->size()[0]);
        size_t a_1=a->size()[0];
        size_t a_2=a->size()[1];
        size_t a_3=a->size()[2];
        size_t b_2=a->size()[1];
        size_t b_3=a->size()[2];
        vector<size_t> c = {a_1,a_2,b_2,b_3};
        C = create_tensor(c);
        vector<size_t> indexC(c.size());
        vector<size_t> indexB(b->size().size());
        vector<size_t> indexA(a->size().size());
        float acc=0;
        for(size_t i=0;i<a_1;i++){
            for(size_t j=0;j<a_2;j++){
                for(size_t p=0;p<b_2;p++){
                    for(size_t q=0;q<b_3;q++){
                        indexC={i,j,p,q};
                        acc=0;
                        for(size_t k=0; k <= a_3; k++){
                        indexA={i,j,k};
                        indexB={k,p,q};
                        acc+=((a->read(indexA))*(b->read(indexB)));
                        }
                        C->write(indexC,acc);
                    }
                }    
            }
        }
        return C;
    }
}

Tensor *add(const Tensor *a, const Tensor *b)
{
    if (a->size() == b->size()){
    	Tensor* res = create_tensor(a->size());
    	vector<size_t> index((a->size()).size());

    	float tmp = (a->read(index))+(b->read(index));
    	res->write(index,tmp);   	
    	while(next_index(a->size(),index)){
    		tmp = (a->read(index))+(b->read(index));
    		res->write(index,tmp);
    	}
    	return res;

    }else if (a->size().size()==b->size().size()){
    	vector<size_t> size;
    	for (int i=0; i<a->size().size(); i++){
    		size.push_back(max(a->size()[i], b->size()[i]));
    	}
    	Tensor* res = create_tensor(size);
    	vector<size_t> index(size.size());

    	res->write(index,((a->read0(index))+(b->read0(index))));	
    	while(next_index(size,index)){
    		res->write(index,(a->read0(index)+b->read0(index)));
    	}
    	return res;

    }else if (a->size().size() > b->size().size()){
    	vector<size_t> size;
    	for (int i=0; i<b->size().size(); i++){
    		size.push_back(max(a->size()[i], b->size()[i]));
    	}
    	Tensor *res = new OrderNTensor(size);
    	vector<size_t> index(size.size());

    	res->write(index,(a->read0(index)+b->read0(index)));
    	while(next_index(size,index)){
    		res->write(index,(a->read0(index)+b->read0(index)));
    	}

    	for (int i=b->size().size(); i< a->size().size(); i++){
    		size.push_back(a->size()[i]);
    	}
    	res->resize(size);
    	index.resize(size.size());
    	res->write(index,a->read0(index));
    	while(next_index(size,index)){
    		res->write(index,a->read0(index));
    	}
    	return res;

    }else{
    	return add(b,a);
    }
}