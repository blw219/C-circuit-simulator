#include "tensor.hpp"

/*
    Given dimensions [d1,d2], then this constructs matrices
    from lists as follows:

            --d2-->
        {             |
            {?,?,?},  d1
            {?,?,?},  |
            {?,?,?}   v
        }
*/
Tensor *create_matrix2d(const std::vector<vector<float>> &data)
{
    vector<size_t> dims={data.size(), data[0].size()};
    Tensor *res=create_tensor(dims);

    for(size_t i=0; i<dims[0]; i++){
        assert(data[i].size()==dims[1]);
        for(size_t j=0; j<dims[1]; j++){
            res->write({i,j},data[i][j]);
        }
    }
    return res;
}

/*
    Given dimensions [d1,d2,d3], then this constructs matrices
    from lists as follows:

  |    {
  |       -d1->   |
  |       {0,1},  d2
  d3      {2,3}   v
  |    },{
  |       -d1->   |
  |       {4,5},  d2
  v       {6,7}   v
       }

        ----d1-->

       4----5    |
      /|   /|   d2
     / 6--/-7    v
    0----1 /     /
    |/   |/    d3
    2----3    /

    If we consider matrices as flat 2D xy maps, then this takes a list of
    2D matrices and stacks them together in the z dimension.
*/
Tensor *create_matrix3d(const std::vector<vector<vector<float>>> &data)
{
    // Note permutation due to stacking
    vector<size_t> dims={data[0].size(), data[0][0].size(), data.size()};

    Tensor *res=create_tensor(dims);

    // Reading in order of the input data, which is not the same as tensor
    for(size_t i2=0; i2<dims[2]; i2++){
        assert(data[i2].size()==dims[0]);

        for(size_t i0=0; i0<dims[0]; i0++){
            assert(data[i2][i0].size()==dims[1]);

            for(size_t i1=0; i1<dims[1]; i1++){
                res->write({i0,i1,i2},data[i2][i0][i1]); // Note permutation
            }
        }
    }
    return res;
}

void compare_matrix(const Tensor *a, const Tensor *b)
{
    vector<size_t> dims=a->size();
    assert(a->size() == b->size());

    vector<size_t> index;

    first_index(dims, index);
    while(1){
        float ga=a->read(index);
        float gb=b->read(index);
        assert( abs(ga-gb) < 1e-6 );
        if(!next_index(dims,index)){
            break;
        }
    }
}

void add_and_compare(Tensor *a, Tensor *b, Tensor *ref)
{
    Tensor *got=add(a,b);

    compare_matrix(ref, got);

    delete a;
    delete b;
    delete got;
    delete ref;
}

void mult_and_compare(Tensor *a, Tensor *b, Tensor *ref)
{
    Tensor *got=multiply(a,b);

    compare_matrix(ref, got);

    delete a;
    delete b;
    delete got;
    delete ref;
}

int main()
{

    cerr<<"Mini test of [2,2] broadcast ";

    /*
        [1 2] + [1]  =  [2 3]
                [3]     [4 5]
    */
    add_and_compare(
        create_matrix2d({
            {1, 2}
        }),
        create_matrix2d({
            {1},
            {3}
        }),
        create_matrix2d({
            {2, 3},
            {4, 5}
        })
    );

    cerr<<"Ok"<<endl;

    cerr<<"Mini test of [2,2] inner multiply ";

    /*
        [1 2] * [1]  =  [7]
                [3]
    */
    mult_and_compare(
        create_matrix2d({
            {1, 2}
        }),
        create_matrix2d({
            {1},
            {3}
        }),
        create_matrix2d({
            {7}
        })
    );

    cerr<<"Ok"<<endl;


    ///////////////////////////////
    // Order-3 * Order-2

    /*
        Using pipe to indicate concatenation of matrices in 3rd dimension.
    */

    cerr<<"Mini test of [2,2,2] * [2,2] "<<endl;

    /*
        [1 2]|[5 6] * [1 2] = [ [1 5].[1 3] [2 6].[1 3] ]|[ [1 5].[2 4] [2 6].[2 4] ]] = [ 16 20 ]|[ 22 28 ]
        [3 4]|[7 8]   [3 4]   [ [3 7].[1 3] [4 8].[1 3] ]|[ [3 7].[2 4] [4 8].[2 4] ]]   [ 24 28 ]|[ 34 40 ]
    */
    mult_and_compare(
        create_matrix3d({
            {
                {1,2},
                {3,4}
            },{
                {5,6},
                {7,8}
            }
        }),
        create_matrix2d({
            {1, 2},
            {3, 4}
        }),
        create_matrix3d({
            {
                {16,20},
                {24,28}
            },{
                {22,28},
                {34,40}
            }
        })
    );

    cerr<<"Ok"<<endl;


    cerr<<"Mini test of [1,1,2] * [2,1] -> [1,1,1] ";

    /*
        [1]|[2] * [3] = [11]
                  [4]
    */
    mult_and_compare(
        create_matrix3d({
            {
                {{1}}
            },{
                {{2}}
            }
        }),
        create_matrix2d({
            {3},
            {4}
        }),
        create_matrix3d({
            {
                {11}
            }
        })
    );

    cerr<<"Ok"<<endl;


}
