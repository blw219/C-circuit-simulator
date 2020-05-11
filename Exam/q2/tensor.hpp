#ifndef tensor_hpp
#define tensor_hpp

#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>
#include <cstddef>

using namespace std;

class Tensor
{
public:
    virtual ~Tensor()
    {}

    /* Returns a vector giving the dimensions of the tensor.
    */
    virtual const vector<size_t> &size() const =0;

    int order(){
        vector<size_t> tmp = size();
        int acc = 0;
        for (int i=0;i<tmp.size();i++){
            if(tmp[i]>0){
                acc+=1;
            }
        }
        return acc;
    }

    /* Returns the volume of the tensor, based on the current size. */
    size_t volume() const;

    /*  Resize the tensor to the given dimensions. This _cannot_ be used
        to change the order of the tensor.
        Assume:
        - size is the original size;
        - size' is the new size;
        - A is the original tensor
        - A' is the new tensor
        For any index i=i_1,i_2,...,i_n within the new tensor size' rannge this function
        should ensure that:
        - A'[i]=A[i], if i_j < size[j], for 0<=j<n
        - A'[i]=0,    otherwise
        This behaviour is a generalisation of the behaviour of vector<float>::resize.
    */
    virtual void resize(const vector<size_t> &dims) =0;

    /*  Read the value at index.
        Pre: 0<=index[i]<size[i], for all 0<=i size
    */
    virtual float read(const vector<size_t> &index) const =0;

    virtual float read0(const vector<size_t> &index) const =0;

    /*  Modify the value at index i.
        Pre: 0<=index[i]<size[i], for all i
    */
    virtual void write(const vector<size_t> &i, float value) =0;
};

/*  Returns the total number of elements contained in a tensor with the
    given dimensions. For example, if dims=[2,3,3], the volume is 2*3*3=18. */
size_t volume(const vector<size_t> &dims);

// Return true if all dimensions are unitary
bool is_scalar(const Tensor *x);

// Return true if there is at most one non-unitary dimension
bool is_vector(const Tensor *x);

/* Convert an index into a linear offset:
        offset == ((index[0]*size[0] + index[1])*size[1] + index[2]) * size[2] + ... index[n-1]
    If allow_broadcast is true, then non-zero indices into length-1 dimensions are allowed.
*/
size_t index_to_offset(const vector<size_t> &size, const vector<size_t> &index, bool allow_broadcast);

/* Convert a linear offset back into the unique index, given tensor size.
    Returns unique vector `index` such that
        offset == ((index[0]*size[0] + index[1])*size[1] + index[2]) * size[2] + ... index[n-1]
    For example, for a matrix, such that:
        offset == index[0]*size[0] + index[1]
*/
vector<size_t> offset_to_index(const vector<size_t> &size, size_t offset);


/* Create a zero-initialised vector with the given dimensions. */
Tensor *create_tensor(const std::vector<size_t> &size);

/* Multiply together two tensors */
Tensor *multiply(const Tensor *a, const Tensor *b);

/* Add two tensors */
Tensor *add(const Tensor *a, const Tensor *b);


/*  Given the dimensions, this pair of functions can iterate
    over every single index within the space. It works for any
    number of dimensions.

    The general usage is:
    ```
    vector<size_t> index;
    first_index(dims, index);
    do{
        // Do something with index
    }while(next_index(dims, index));
    ```
*/
void first_index(const vector<size_t> &dims, vector<size_t> &index);
bool next_index(const vector<size_t> &dims, vector<size_t> &index);


#endif
