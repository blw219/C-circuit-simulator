#ifndef order_n_tensor_hpp
#define order_n_tensor_hpp

#include "tensor.hpp"

class OrderNTensor
	: public Tensor
{
private:
    vector<size_t> m_size;
    //from m_size[0] to size[1] ....
    vector<float> m_values;
public:
    OrderNTensor(const vector<size_t> &_size)
    {
        resize(_size);
    }

    const vector<size_t> &size() const override
    { return m_size; }

    void resize(const vector<size_t> &dims) override
    {
    	assert(dims.size()>0);
    	// auto it = m_values.begin();
    	int pos =0;

    	for (int i=0; i< dims.size(); i++){
    		if (i>=m_size.size()){
    			m_values.insert(m_values.begin()+pos, dims[i], 0.0f);
    			pos += dims[i];
    		}else if(dims[i] > m_size[i]){
    			pos += m_size[i];
    			m_values.insert(m_values.begin()+pos, dims[i]-m_size[i], 0.0f);
    			pos += dims[i]-m_size[i];
    		}else if(dims[i] < m_size[i]){
    			pos += dims[i];
    			for (int n=0; n<(m_size[i]-dims[i]); n++){
    				m_values.erase(m_values.begin()+pos);
    			}
    		}
    	}

  		m_size = dims;
    }    

    virtual float read(const vector<size_t> &i) const override
    {
    	for (int j=0; j<i.size();j++){
    		assert(i[j] < m_size[j]);
    	}
    	size_t pos=index_to_offset(m_size,i,true);

        return m_values[pos];
    }   

    //this returns 0 if i is out of range
    virtual float read0(const vector<size_t> &i) const override
    {
        for (int j=0; j<i.size();j++){
            if (i[j] != 0 && j == m_size.size()) return 0;
            if(i[j] >= m_size[j]) return 0;
        }
        size_t pos=index_to_offset(m_size,i,true);

        return m_values[pos];
    }      

    virtual void write(const vector<size_t> &i, float value) override
    {
    	for (int j=0; j<i.size();j++){
    		assert(i[j]<m_size[j]);
    	}
    	size_t pos = index_to_offset(m_size,i,false);

        if (pos > m_values.size()){m_values.resize(pos);}
        m_values[pos]=value;
    }

};

#endif