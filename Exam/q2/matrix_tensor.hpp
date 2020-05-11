#ifndef matrix_tensor_hpp
#define matrix_tensor_hpp

#include "tensor.hpp"

/* Implements an order-2 tensor. */
class MatrixTensor
    : public Tensor
{
private:
    vector<size_t> m_size;
    vector<vector<float>> m_values;
public:
    MatrixTensor(const vector<size_t> &_size)
    {
        resize(_size);
    }

    const vector<size_t> &size() const override
    { return m_size; }

    void resize(const vector<size_t> &dims) override
    {
        assert(dims.size()==2);
        assert(dims[0]>0);
        assert(dims[1]>0);

        m_values.resize(dims[0], vector<float>());
        for(int i=0; i<dims[0]; i++){
            m_values[i].resize(dims[1], 0.0f);
        }
        m_size=dims;
    }

    virtual float read(const vector<size_t> &i) const override
    {
        int i0=0, i1=0;
        if(m_size[0]>0 && m_size[0]!=1 ){
            assert(i[0]<m_size[0]);
            i0=i[0];
        }
        if(m_size[1]>1 && m_size[1]!=1){
            assert(i[1]<m_size[1]);
            i1=i[1];
        }
        assert(i0<m_size[0]);
        assert(i1<m_size[1]);
        return m_values[i0][i1];
    }

    // return 0 if out of range
    virtual float read0(const vector<size_t> &i) const override
    {
        for (int j=2; j<i.size(); j++){
            if (i[j]) return 0;
        }
        int i0=0, i1=0;
        if(m_size[0]>0 && m_size[0]!=1 ){
            if(i[0]>=m_size[0]) return 0;
            i0=i[0];
        }
        if(m_size[1]>1 && m_size[1]!=1){
            if(i[1]>=m_size[1]) return 0;
            i1=i[1];
        }
        assert(i0<m_size[0]);
        assert(i1<m_size[1]);
        return m_values[i0][i1];
    }   

    virtual void write(const vector<size_t> &i, float value) override
    {
        size_t i0=0, i1=0;
        if(i.size() > 0){
            i0=i[0];
        }
        if(i.size() > 1){
            i1=i[1];
        }
        assert(i0<m_size[0]);
        assert(i1<m_size[1]);
        m_values[i0][i1]=value;
    }
};

#endif
