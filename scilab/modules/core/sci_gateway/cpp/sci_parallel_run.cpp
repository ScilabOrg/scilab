/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
extern "C" {
#include <stdio.h>

#include "stack-c.h"
#include "gw_core.h"

#include "dynamic_link.h"
#include "api_common.h"
#include "api_double.h"

#include "MALLOC.h"

#include "stack1.h"
#include "api_double.h"
#include "api_int.h"
#include "api_list.h"
#include "api_string.h"
#include "api_common.h"
#include "stack3.h"
#include "stack2.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
#include "sci_mem_alloc.h"

#include "stack-def.h" /* #define nlgh nsiz*4   */
#include "stack-c.h"  /* #define Nbvars C2F(intersci).nbvars, Top & cie */

}

#include <cstdlib>
#include <cstring>
#include "parallel_run.hxx"
#include "parameters.hpp"
#include "parameters.cpp" /* directly include this file for now, waiting for proper implementation from module/parameters */

#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>

/*
 *

 We can handle k Rhs  et m Lhs. Currently, only real (double) matrix are implemented, but the framework is here for any Scilab datatype.
 (see scilab_var and scilab_allocated_var class hierarchy).
 [R1, ... , Rm] = parallel_run(A1, ... , Ak, f [,Types] [,Dims])

 If Args are of différent sizes, the smallest are recycled.

 Types : matrix of <=m strings of the names (as in typeof()) of the  m lhs fo 'f'. default to "constant"
 Rows : matrix of doubles 1 x <=m or 2 x <=m giving the nb of rows (or rows and columns) of the m lhs of f. default to 1

 /!\ due to matrix data layout in Scilab, a matrix [a11,a12,a13;a21,a22,a23] contains 3 (ncols) arguments of 2 (nrows) elements;


*/

extern "C" {
  int  C2F(sci_parallel_run)(char *fname,unsigned long fname_len);
}

namespace {
  int current_top= 0;/* used to keep track of where the current Top should/will be */
  char* current_fname= 0; /* name of the current Scilab function, to be used un error messages */
  
  
  typedef void (*function_to_call_t)(void const*const*, void*const*);
  typedef void (*wrapper_function_t)(double const*, double*);
  typedef void (*loaded_function_t)();
  typedef void (*simple_function_t)(int );

  /* Classes representing scilab types*/
  struct scilab_var {

    /*  explicit scilab_var(int t):type(t){} we cannot construct in initializer list because getVarType does not return the type */
    int type;

    static scilab_var* create(char const* type_name, int nrows, int ncols);

    virtual int get_type() const{ return type; }
    virtual void set_type( int t) { type= t; }
    virtual void set_type( char const* type_name) { 
      /*    std::cerr<<"set_type("<<type_name<<")\n"; */
      type= (strcmp(type_name, "constant")==0) 
	? sci_matrix
	: -1; 
    }

    virtual void debug()const{ std::cerr<<" default debug;\n";}

    virtual  std::size_t element_size() const=0;
    virtual std::size_t sizeof_data()const{ std::cerr<<"default sizeof_data\n"; return 0;}
    virtual  ~scilab_var(){}
  };


  struct scilab_c_function : virtual scilab_var {
    virtual  std::size_t element_size() const {return sizeof(int);}
    int nb_lhs;
    int nb_rhs;
  };

  struct scilab_matrix : virtual scilab_var {
    virtual void debug()const{ std::cerr<<" matrix type:"<<get_type()<<" nrows:"<<nrows<<"ncols:"<<ncols<<std::endl ;}

    int nrows, ncols;
    void set_from_model(scilab_matrix const& m, std::size_t const n){
      type= sci_matrix;
      switch(n) {
      case 0 :{
	nrows= m.nrows;
	ncols= 1;
	break;
      }
      case 1 :{
	nrows= m.nrows;
	ncols= m.ncols;
	break;
      }
      default: {
	nrows= m.nrows * m.ncols;
	ncols= n;
      }
      }
    }
      
    std::size_t rows_size() const { 
      //    std::cerr<<"nrows of scilab_matrix: "<<nrows<<std::endl;
      return nrows * element_size(); } // scilab matrices are stored column wise (as in Fortran)
 
    virtual std::size_t sizeof_data()const{return  ncols*nrows * element_size();}

    virtual  ~scilab_matrix(){}
  };


  bool is_function_or_string(scilab_var const&  var){ 
    bool res;
    //    std::cerr<<"in is_function_or_string type :"<<var.get_type()<<std::endl;
    switch(var.get_type()){
    case sci_c_function : { res= true; break;}
    case sci_strings : { 
      scilab_matrix const& as_matrix(dynamic_cast<scilab_matrix const&> (var));
      res = (as_matrix.nrows == 1) && (as_matrix.ncols == 1);
      break;
    }
    default: { res= false; }
    }
    return res;
  }


  struct scilab_real_matrix : scilab_matrix {
    explicit scilab_real_matrix(int nr=1, int nc=1){type=sci_matrix; nrows=nr; ncols=nc;}
    virtual  std::size_t element_size() const { return sizeof(double) ;}
    virtual  ~scilab_real_matrix(){}
  };

  struct scilab_int8_matrix : scilab_matrix {

    virtual  std::size_t element_size() const { return 1 ;}
    virtual  ~scilab_int8_matrix(){}
  };
  struct scilab_complex_matrix : scilab_matrix {

    virtual  std::size_t element_size() const { return 2*sizeof(double) ;}
    virtual  ~scilab_complex_matrix(){}
  };

  struct scilab_string_matrix : scilab_matrix {

    virtual  std::size_t element_size() const { std::cerr<<"unimplemented string matrix element size"; return 0;}
    virtual  ~scilab_string_matrix(){}
  };

  typedef int* sci_addr_t;
  struct scilab_allocated_var : virtual scilab_var{

    static scilab_allocated_var* get(int i);
    static scilab_allocated_var* create(scilab_var const& model, std::size_t n);
  
    virtual void const* data()const=0;
    virtual void* data()=0;
    virtual void const* data_chunk(std::size_t) const=0;
    virtual void* data_chunk(std::size_t)=0;
    virtual std::size_t data_chunk_size()const {return static_cast<char const*>(data_chunk(1))- static_cast<char const*>(data_chunk(0));}
    virtual ~scilab_allocated_var(){}
    virtual bool function_name_or_ptr()const=0;
  };
  scilab_var* scilab_var::create(char const* type_name, int nrows, int ncols){
    scilab_matrix* res;
    res= new scilab_real_matrix();
    res->set_type(type_name);
    //  std::cerr<<"create var of type:"<<type_name<<'\n';
    res->nrows= nrows;
    res->ncols= ncols;
    return res;
  }
  /* templates representing Scilab variables allocated in the stack */
  template< typename ScilabVar>
  struct scilab_allocated_concrete_var : virtual scilab_allocated_var,  ScilabVar {

    explicit scilab_allocated_concrete_var<ScilabVar>(sci_addr_t a):addr(a){
      err= getVarType( pvApiCtx, addr, &(this->type) );
      this->read_from_stack();
    }
    explicit scilab_allocated_concrete_var<ScilabVar>(int pos){
      err= getVarAddressFromPosition(pvApiCtx, pos, &this->addr);
      err= getVarType( pvApiCtx, addr, &(this->type) );
      this->read_from_stack();
    }

    /* if n>0 : alloc n times the model, if n=0, alloc 1/n times 
       model rowsM x colsM -> n>1 -> alloc rows= rowsM x colsM cols= n n==0 alloc rows=rowsM cols=1 */
    explicit scilab_allocated_concrete_var<ScilabVar>( ScilabVar const& model, std::size_t const n=1);

    virtual void const* data()const {return data_ptr.opaque; }
    virtual void* data(){    return data_ptr.opaque; }
    void const* data_chunk(std::size_t i) const { return static_cast<void const*>(data_ptr.as_char+ ScilabVar::rows_size()*i);}
    void* data_chunk(std::size_t i) { return static_cast<void*>(data_ptr.as_char+ ScilabVar::rows_size()*i);}
  
    void read_from_stack();

    SciErr err;
    sci_addr_t addr;

    union {
      double* as_double;
      int* as_int;
      char* as_char;
      char** as_char_ptr_ptr;

      void* opaque;
      int as_sci_c_function;
    }data_ptr;

    virtual ~scilab_allocated_concrete_var<ScilabVar>(){}
    virtual bool function_name_or_ptr()const { return false;}

  };

  template<> void scilab_allocated_concrete_var<scilab_real_matrix>::read_from_stack(){
  
    err= getMatrixOfDouble(pvApiCtx, addr, &nrows, &ncols, &data_ptr.as_double);
  }

  template<> void scilab_allocated_concrete_var<scilab_int8_matrix>::read_from_stack(){
    err= getMatrixOfInteger8(pvApiCtx, addr, &nrows, &ncols, &data_ptr.as_char);
  }
  template<> void scilab_allocated_concrete_var<scilab_string_matrix>::read_from_stack(){
    getAllocatedMatrixOfString(pvApiCtx, addr, &nrows, &ncols, &data_ptr.as_char_ptr_ptr);
  }

  template<>  scilab_allocated_concrete_var<scilab_c_function>::scilab_allocated_concrete_var(int pos){
    C2F(getrhsvar)(&pos, EXTERNAL_DATATYPE, &nb_lhs, &nb_rhs, &data_ptr.as_sci_c_function,strlen(EXTERNAL_DATATYPE));  
  }


  template<> void scilab_allocated_concrete_var<scilab_c_function>::read_from_stack(){
    double* unused;
    /* c'est n'imp : macro qui fait return 0 et position nécessaire :(
       -> autre constructeur avec position */

  }


  template<> bool scilab_allocated_concrete_var<scilab_string_matrix>::function_name_or_ptr()const{
    return ncols == 1 && nrows == 1 ;
  }
  template<> bool scilab_allocated_concrete_var<scilab_c_function>::function_name_or_ptr()const{
    return true;
  }
  template<>  void const* scilab_allocated_concrete_var<scilab_c_function>::data_chunk(std::size_t i) const {  return 0; }
  template<>  void * scilab_allocated_concrete_var<scilab_c_function>::data_chunk(std::size_t i) {  return 0; }


  template<> void scilab_allocated_concrete_var<scilab_complex_matrix>::read_from_stack(){
    double* unused;
    err= getComplexMatrixOfDouble(pvApiCtx, addr, &nrows, &ncols, &data_ptr.as_double, &unused);/* for now, assume imaginary part is stored after real part.  */
  }
  template<> scilab_allocated_concrete_var<scilab_real_matrix>::scilab_allocated_concrete_var(scilab_real_matrix const& model, std::size_t const n ){
    this->set_from_model(model,n);
    //  std::cerr<<" allocMatrixOfDouble(pvApiCtx, ++"<<current_top<<", nrows, ncols, &data_ptr.as_double);\n";
    err= allocMatrixOfDouble(pvApiCtx, ++current_top, nrows, ncols, &data_ptr.as_double);
  }
  template<>  scilab_allocated_concrete_var<scilab_int8_matrix>::scilab_allocated_concrete_var(scilab_int8_matrix const& model, std::size_t const n ){
    std::cerr<<"unimplemented int8 returned value\n";
  }
  template<>  scilab_allocated_concrete_var<scilab_string_matrix>::scilab_allocated_concrete_var(scilab_string_matrix const& model, std::size_t const n ){
    std::cerr<<"unimplemented string returned value\n";
  }

  template<>  scilab_allocated_concrete_var<scilab_complex_matrix>::scilab_allocated_concrete_var(scilab_complex_matrix const& model, std::size_t const n ){
    std::cerr<<"unimplemented complex returned value\n";
  }




  scilab_allocated_var* scilab_allocated_var::create(scilab_var const& m, std::size_t n){
    scilab_allocated_var* res;
    switch(m.get_type()){
    case sci_matrix : {
      //    std::cerr<<" scilab_allocated_var::create sci_matrix nrows:"<<(dynamic_cast<scilab_real_matrix const&>(m).nrows)<<" ncols:"<<(dynamic_cast<scilab_real_matrix const&>(m).ncols)<<" n:"<<n<<std::endl;
      res= new scilab_allocated_concrete_var<scilab_real_matrix>(dynamic_cast<scilab_real_matrix const&>(m), n);
      break;
    }
    default : {
      std::cerr<<"unimplemented create()\n";
      res=0;
    }
    }
    return res;
  }

  scilab_allocated_var* scilab_allocated_var::get(int pos){
    scilab_allocated_var* res;
    sci_addr_t addr;
    SciErr err= getVarAddressFromPosition(pvApiCtx, pos, &addr);
    int type;
    err= getVarType(pvApiCtx, addr, &type);
    //    std::cerr<<"type "<<type<<"@"<<pos<<std::endl;
    switch(type) {
    case sci_matrix : { 
      if( isVarComplex(pvApiCtx, addr) ) {
	res= new scilab_allocated_concrete_var<scilab_complex_matrix>(addr);
      } else {
	res= new scilab_allocated_concrete_var<scilab_real_matrix>(addr);
      }
      break;
    }
    case sci_ints : { /* TODO int subtypes */
      res= new scilab_allocated_concrete_var<scilab_int8_matrix>(addr);
      break;
    }
    case sci_strings : {
      res = new scilab_allocated_concrete_var<scilab_string_matrix>(addr);
      break;
    }
    case sci_c_function : {/* must use position with GetRhsVar for sci_c_function :( :( :( */
      res = new scilab_allocated_concrete_var<scilab_c_function>(pos);
      break;
    }
    default : { std::cerr<<"unimplemented type in scilab_allocated_var fatory !!!\n";}
    }
    return res;
  }

  /*
    wrapper on a native c function or a scilab macro called on scilab variables.

    constructed on :
    - scilab variable for the function (external native function name, sci_c_function (buggy) or macro name)scilab matrices of arguments
    - expected lhs

    upon construction, allocate scilab result variables and computes all necessary meta data:
  
    std::vector<std::size_t> args_sizes(function_rhs), args_nb(function_rhs), res_sizes(Lhs);
    std::vector<void const*> args_mem(function_rhs);
    std::vector<void**> res_mem(Lhs);


    for native c function
  */
  struct wrapper {

    typedef std::vector<std::size_t> sizes_container;

    /* wrapper contructor
     * @ param args_begin iterator to the first args of parallel_run
     * @ param function_it iterator to the function argument of parallel_run
     * @ param args_end iterator past the end of parallel_run args
     * @ @param function_lhs number of lhs (of parallel_run and of the function: it is the same)
     */
    template<typename VarsIt>
    wrapper(VarsIt args_begin, VarsIt function_it, VarsIt args_end, std::size_t function_lhs)
      :function_rhs(std::distance(args_begin, function_it)), res_sizes(function_lhs){
      VarsIt first_arg_position(args_begin);
      for(VarsIt it(args_begin); it != function_it; ++it) {
	register_arg(dynamic_cast<scilab_matrix const&> (**it));
      }
      //    std::cerr<<"**function_it with var of type"<<dynamic_cast<scilab_allocated_var const&>(**function_it).get_type()<<std::endl;
      n= *std::max_element(args_nb.begin(), args_nb.end());
      get_function(dynamic_cast<scilab_allocated_var const&>(**function_it));
      alloc_complete_results(args_begin, function_it+1, args_end, function_lhs);
    }

    /* the member function performing the call to the function (foreign function of Scilab macro)
     * @ param args array of ptrs to args data
     * @ param res array of ptrs to res data
     */
    void operator()(void const** args, void ** res)  { 
      // std::cerr<<"wrapped args "<<*(double*)*args<<" wrapped_res "<<*(double*)*res<<std::endl;
      (*this.*(this->f_ptr))( args, res ); 
      // std::cerr<<"after call wrapped args "<<*(double*)*args<<" wrapped_res "<<*(double*)*res<<std::endl;
    }

    /* It is idiomatic to pass functors by value in C++, but our wrapper is heavy, o we provide a lightweight handle */
    struct handle {
      handle(wrapper& r):w(r){}
      /* just forward to the underlying wrapper */
      void operator()(void const** args, void ** res) const {
	w( args, res ); 
      }
      wrapper& w;
    };
    handle get_handle() {
      return handle(*this); 
    }
    /* @return begin iterator to the array of pointers to arguments data */
    void const*const* args_data_begin() const { return &args_data[0];}
    /* @return begin iterator to the array of arguments sizes */
    std::size_t const* args_sizes_begin() const { return &args_sizes[0];}
    /* @return begin iterator to the array of arguments number of elements (they are not requires to have the same nb of elements */ 
    std::size_t const* args_nb_begin() const { return &args_nb[0];}
    /* @return nb of tasks (calls) to perform = max(args_nb_begin(), args_nb_begin()+rhs) */
    std::size_t tasks_nb() const { return n;}
    /* @return begin iterator to the array of pointers to result data */
    void * const* res_data_begin() { return &res_data[0];}
    /* @return begin iterator to the array of results sizes */
    std::size_t const* res_sizes_begin() const { return &res_sizes[0];}
    /* @return nb of rhs vars of the function */
    std::size_t nb_rhs() const{ return function_rhs;}
    /* @return true if the underlying function is a foreign function, false if it is a Scilab macro */
    bool is_foreign_function() const { return function.to_call !=0 ;}

  private:
    /* ptr to foreign function (types used where storing, calling or wrapping) */
    union{
      function_to_call_t to_call;
      loaded_function_t to_load;
      wrapper_function_t wrapper;
    } function;
    /* register a matrix of arguments to be used as rhs) */
    void register_arg(scilab_matrix const& m) { 
      args_data.push_back((dynamic_cast<scilab_allocated_var const&> (m)).data());
      args_sizes.push_back( m.rows_size());
      args_nb.push_back(m.ncols);
      scilab_function_rhs_models.push_back(&m);
    }
    /* ptr to data type */
    typedef union {
      double* double_ptr;
      void* opaque_ptr;
    } union_of_ptrs_t;

    /* alloc the scilab variables that will hold the complete collection of results 
     * @param first_arg_position only used to compute the args positions for error messages
     * @param res_types_begin, res_types_end iterator range on the args describing result, can be empty
     * @param nb_lhs number of lhs    */
    template<typename VarPtrIt>
    void alloc_complete_results(VarPtrIt first_arg_position, VarPtrIt res_type_begin, VarPtrIt res_type_end, std::size_t nb_lhs){
      SciErr err;
      char const* type_name="constant";
      std::vector<char const*> type_names(Lhs, "constant");
      typedef std::pair<std::size_t, std::size_t> dim_t;
      std::vector<dim_t>dims(Lhs, dim_t(1,1));

      if(res_type_begin != res_type_end){
	if((**res_type_begin).get_type()==sci_strings){
	  //	std::cerr<<"we have a type lhs arg\n";
	  // process it 	scilab_allocated_concrete_var<scilab_string_matrix> const& str(dynamic_cast<scilab_allocated_concrete_var<scilab_string_matrix> const&>(v));
	  ++res_type_begin;
	}
	if(res_type_begin != res_type_end){
	  if((**res_type_begin).get_type()==sci_matrix) {
	    //	  std::cerr<<"we have a dim lhs arg\n";
	    scilab_allocated_concrete_var<scilab_real_matrix> const& m(dynamic_cast<scilab_allocated_concrete_var<scilab_real_matrix> const&>(**res_type_begin));

	    switch(m.ncols) {
	    case 2:{
	      //	    std::cerr<<"we have rows and cols\n";
	      for(std::size_t i(0); i< m.nrows && i< dims.size(); ++i){
		dims[i].first=static_cast<std::size_t>(m.data_ptr.as_double[i]);
		dims[i].second=static_cast<std::size_t>(m.data_ptr.as_double[i+m.nrows]);

	      }

	      break;
	    }
	    case 1: {
	      //	    std::cerr<<"we have rows \n";
	      for(std::size_t i(0); i<m.nrows && i< dims.size(); ++i){
		dims[i].first=static_cast<std::size_t>(m.data_ptr.as_double[i]);
	      }
	      break;
	    }
	    default : {
	      Scierror(999,_("%s: Wrong size of input argument #%d: Number of columns are incompatible "),current_fname
		       , std::distance(first_arg_position, res_type_begin));
	    }
	    }
	  }
	}
      }
      for(std::size_t i(0); i!= Lhs; ++i){
	scilab_function_lhs_models.push_back(scilab_var::create(type_names[i], dims[i].first, dims[i].second));
      }
      res_data.resize(nb_lhs);
      res_sizes.resize(nb_lhs);
    
      /* we want to have to result var at hand before calling the scilab macro so we must create it now before the args */
      for(std::size_t i(0); i!= nb_lhs; ++i) {
	union_of_ptrs_t u;
	//      std::cerr<<"alloc matrix of size "<< 1 <<" "<<n<<std::endl;
	scilab_collections_of_lhs.push_back( scilab_allocated_var::create(*scilab_function_lhs_models[i], n));
	//      scilab_collections_of_lhs[i]->debug();
	res_data[i]= scilab_collections_of_lhs[i]->data();
	res_sizes[i]= scilab_collections_of_lhs[i]->data_chunk_size();
      }

      //    std::cerr<<"allocating"<<nb_lhs<<" res scilab variables with "<<std::distance(res_type_begin, res_type_end)<<" type informations\n";
    
    }

    /* extract the function form the scilab variable (i.e.string) reprensenting it.
     * @param v the variable 
     * @return nothing useful but GetRhsVar() macro wants to be able to return an int :(
     */
    int get_function(scilab_allocated_var const& v) {
      //    std::cerr<<"in get_function with var of type"<<v.type<<std::endl;
      function.to_call=0;
      switch(v.get_type()) {
      case sci_c_function : {
	int m_fun, n_fun;
	GetRhsVar(2, EXTERNAL_DATATYPE, &m_fun, &n_fun, &scilab_function);
	f_ptr = &wrapper::macro<false>;
	break;
      }
      case sci_strings : {
	scilab_allocated_concrete_var<scilab_string_matrix> const& str(dynamic_cast<scilab_allocated_concrete_var<scilab_string_matrix> const&>(v));
	char * funName =str.data_ptr.as_char_ptr_ptr[0];
	int found;
	found=SearchInDynLinks(funName, &function.to_load);
	f_ptr= &wrapper::native_function;
	if(found == -1) 
	  { 
	    /* should check amongst defined macros with getmacroslist (cf dans core/src/c/getvariablesname.c) and check that type is sci_XXX */
	    function.to_call=0;
	    scilab_function_name= funName;
	    scilab_function_name_length= std::strlen(scilab_function_name);
	    f_ptr= &wrapper::macro<true>;
	    
	  }
      }
      }
    }



    /* performs the Scilab macro call 
     * @param byName bool template parameter tells if the macro is called by name or by ptr (ptr is currently broken).
     * @param args array of ptrs to args data
     * @param res array of ptr to res data
     */
    template<bool byName>
    void macro(void const** args, void ** res)  {
      /* rhs models from  */
      int save_Nbvars= Nbvars, save_current_top= current_top;
      for( std::vector<scilab_var const*>::const_iterator it(scilab_function_rhs_models.begin())
	     ; it != scilab_function_rhs_models.end(); ++it, ++args) {
	
	scilab_allocated_var* scilab_arg= scilab_allocated_var::create(**it,0);
	//            std::cerr<<"creating temp arg for macro of type"<<scilab_arg->get_type()<<std::endl;
	memcpy(scilab_arg->data(), *args,scilab_arg->sizeof_data());
	//            std::cerr<<"value:"<<*((double*)scilab_arg->data())<<std::endl;
      }

      int  sci_rhs = scilab_function_rhs_models.size();
      int  sci_lhs = scilab_function_lhs_models.size();

      std::size_t dummy_vars(0);
      for( ;sci_rhs+dummy_vars < sci_lhs+max_safety_lhs; ++dummy_vars){
        //      std::cerr<<"alloc dummy var";
        scilab_allocated_concrete_var<scilab_real_matrix> dummy(scilab_real_matrix());
      }

      int sci_arg_pos = current_top-sci_rhs+1;
      //  std::cerr<<"sci_rhs :"<<sci_rhs<<" Nbvars: "<<Nbvars<<std::endl;
      Nbvars = Rhs+Lhs+sci_rhs; 
      //  std::cerr<<"sci_rhs :"<<sci_rhs<<" Nbvars: "<<Nbvars<<std::endl;
      if(byName){
	C2F(scistring)(&sci_arg_pos, scilab_function_name, &sci_lhs, &sci_rhs, scilab_function_name_length);
      } else {
	C2F(scifunction)(&sci_arg_pos, &scilab_function, &sci_lhs, &sci_rhs);
      }
      // result r is now on first position on stack
      {
	double* tmp;
	int* addr;
	int rm1, rn1;
	//      std::cerr<<"Nbvars:"<<Nbvars<<"Top:"<<Top<<"curent_top:"<<current_top<<std::endl;
	Nbvars = Rhs+Lhs+sci_rhs+dummy_vars;
	int res_pos=Rhs+Lhs+1; // 

	for( std::vector<scilab_var const*>::iterator it(scilab_function_lhs_models.begin())
	       ; it != scilab_function_lhs_models.end(); ++it, ++res_pos, ++res){

	  //      std::cerr<<"res_pos :"<<res_pos<<" Nbvars: "<<Nbvars<<std::endl;
	  scilab_allocated_var* scilab_res= scilab_allocated_var::get(res_pos);
	  /*	for(int tmpi(0); tmpi <  (scilab_res->sizeof_data()/sizeof(double)); ++tmpi)
	    { std::cerr<<"res:"<< (*(((double*)scilab_res->data())+tmpi))<<std::endl; }*/
	  memcpy(*res, scilab_res->data(), scilab_res->sizeof_data());
	}
	Nbvars= save_Nbvars;
	current_top=save_current_top;
      }

      //    std::cerr<<"calling a macro"<<(byName ? "by name" : "by ref")<<std::endl;
    }
    void native_function(void const** args, void ** res) {  
      // std::cerr<<"before function.to_call "<<*(double*)*args<<" wrapped_res "<<*(double*)*res<<std::endl;
      function.to_call(args, res); 
    }

    /* we prealloc as much scilab var more than requested lhs in case the scilab macro call back returns more thant requested.*/
    static unsigned int const max_safety_lhs = 20;

    std::size_t function_rhs; /* nb of rhs of the function, nb of */
    std::size_t n; /* nb of calls to perform */
    sizes_container args_sizes, args_nb, res_sizes; /* sizes of arguments, nb of elements for each argument, sizes for results */
    std::vector<void const*> args_data; /* ptrs to args data */
    std::vector<void *> res_data; /* ptrs to res data */

    /* the member function to call, dispatches to macro of foreign function */
    void(wrapper::*f_ptr)(void const** args, void ** res);

    int scilab_function; /* the scilab function 'ptr' for scifunction */
    char* scilab_function_name;/* the scilab function name for scistring */
    std::size_t scilab_function_name_length;/* the scilab function name length for scistring */

    /* store models of scilab lhs and rhs variables */
    std::vector<scilab_var const*> scilab_function_lhs_models, scilab_function_rhs_models; 
    std::vector<scilab_allocated_var*>scilab_collections_of_lhs;    /* lhs vars of the parallel_run function : collections of the lhs form the function*/

  };
  /* Checks if the function parallel_run arguments are valid.
   * 1 or more matrices of doubles
   * 1 matrix of 1 string
   * 0 or 1 matrix of strings and/or 1 matrix of doubles with 1 ou 2 columns
   * 0 or 1 configuration plist  
   *
   * @retun true is the args are valid */
  bool check_args(void) {
    {
      if(Rhs<2) { return false; }
      bool before_function(true), at_least_one_arg(false);
      bool ok(true);
      SciErr err;
      for( unsigned int pos(1); pos <= Rhs && ok; ++pos) {
	int* addr;
	err= getVarAddressFromPosition(pvApiCtx, pos, &addr);
	int type;
	err= getVarType( pvApiCtx, addr, &type );
	if (before_function) {
	  switch (type) {
	  case sci_matrix : {
	    /* check not complex "%s: This feature has not been implemented.\n" */
	    ok= !isVarComplex( pvApiCtx, addr);
	    at_least_one_arg= true;
	    break;
	  }
	  case sci_strings : {
	    /* check dim = 1x1 */
	    int rows, cols;
	    err= getMatrixOfString(pvApiCtx, addr, &rows, &cols, 0,0);
	    ok= (rows == 1) && (cols == 1);
	    before_function= false;
	    break;
	  }
	  default : {
	    Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),current_fname, 1);
	    ok= false;
	  }
	  }
	} else {
	  switch (type) {
	  case sci_strings :{
	    break;
	  }
	  case sci_matrix :{
	    /* check not complex and ncol <=2 */
	    ok= !isVarComplex( pvApiCtx, addr);
	    if(ok) {
	      int rows, cols;
	      double* unused;
	      err= getMatrixOfDouble(pvApiCtx, addr, &rows, &cols, &unused);
	      ok= (cols <= 2);
	      break;
	    }
	  }
	  case sci_mlist :{ /* config plist (which is a mlist) */
	    break;
	  }

	  default : {
	    ok= false;
	  }
	  }
	}
	}
      return ok && at_least_one_arg && (!before_function);
    }
  }

  /* Get configuration options form the config parameter
   *
   * in :
   * @param config_arg_pos position where the config arg can be
   *
   * in/out:
   *
   * @param nb_workers int config value for the key "nb_workers"
   * @param shared_memory bool config value for the key "shared_memory"
   * @param dynamic_scheduling bool config value for the key "dynamic_scheduling"
   * @param chunk_size int config value for the key "chunk_size"
   * @param prologue char* config value for the key "prologue"
   * @param prologue char* config value for the key "epilogue"
   * 
   * @return bool true if there was a configuration argument in position config_arg_pos.
   */
  bool get_config_parameters(int config_arg_pos, int& nb_workers, bool& shared_memory, bool& dynamic_scheduling, int& chunk_size, char*& prologue, char*& epilogue){
    int log(0);
    int* addr;
    SciErr err(getVarAddressFromPosition(pvApiCtx, config_arg_pos, &addr));
    bool has_config_arg( check_parameters(addr)==0);
    if(has_config_arg) {
      int found;
      get_int_parameter(addr, "nb_workers", &nb_workers, &found, nb_workers, log,  CHECK_NONE);
      int tmp;
      get_int_parameter(addr, "shared_memory", &tmp, &found, shared_memory ? 1:0, log,CHECK_NONE);
      shared_memory= (tmp!=0);
      get_int_parameter(addr, "dynamic_scheduling", &tmp, &found, dynamic_scheduling ? 1:0, log,CHECK_NONE);
      dynamic_scheduling= (tmp!=0);
      get_int_parameter(addr, "chunk_size", &chunk_size, &found, chunk_size, log,CHECK_NONE);
      get_string_parameter(addr, "prologue", &prologue, &found, prologue, log, CHECK_NONE);
      get_string_parameter(addr, "epilogue", &epilogue, &found, epilogue, log, CHECK_NONE);
    }
    return has_config_arg;
  }

  /*
    A simple wrapper just wraps prologues and epilogues, each taking only a scalar argument (the process number).
   */
  struct simple_wrapper{
    explicit simple_wrapper(char const* name):fun(name) {    }
    void operator()(int i){
      if(*fun){ /* do nothing on empty name */
	union{
	loaded_function_t to_load;
	simple_function_t to_call;
      } function;
      int found=SearchInDynLinks(const_cast<char*>(fun), &function.to_load);
      if(found != -1){
	function.to_call(i);
      }else{
	double* tmp_ptr;
	SciErr err= allocMatrixOfDouble(pvApiCtx, ++current_top, 1, 1, &tmp_ptr);
	*tmp_ptr= static_cast<double>(i);
	++Nbvars;
	int lhs(0), rhs(1);
	C2F(scistring)(&current_top, const_cast<char*>(fun), &lhs, &rhs, strlen(fun));
	--Nbvars;
	--current_top;
      }
      }
    }
    char const* fun;
  };
}

/* Calling point from Scilab. 
 * checking args and contruction a wrapper around function call of a foreign function or a Scilab macro.
 * this wrapper (in fact, a handle) is then passed to another wrapper that will parallelize the calls.
 * the parallel wrapper is independant of Scilab (thanks to this wrapper) and is implemented in parallel_wrapper.hpp.
 *
 * Calling parallel_run is
 * 1°) checking args
 * 2°) constructing wrapper pre allocating result Scilab vars and abstracting arrays of args and results pointers
 * (in parallel_wrapper )
 * 3°) contructing a parallel_wrapper 
 * 4°) calling the parallel_wrapper according to config options (i.e. nb of workers)
 * 4.1°) for each call to be made, adjusting the args and res ptr
 * 4.2°) calling the wrapper
 *
 */
int  C2F(sci_parallel_run)(char *fname,unsigned long fname_len) 
{
  typedef std::vector<scilab_allocated_var*> vars_container_t;
  current_fname=fname;
  
  if( !check_args()) {
    Scierror(999,_("%s: Wrong number of input argument(s).\n"),fname);/* need a better error message */
    PutLhsVar();
    return 0; 
  }
  
  current_top= Rhs;
  int nb_args_to_handle(Rhs);
  int nb_workers(0), chunk_size(1);
  bool shared_memory(false), dynamic_scheduling(false);
  char* prologue_name("");
  char* epilogue_name("");
  if( get_config_parameters(Rhs, nb_workers, shared_memory, dynamic_scheduling, chunk_size, prologue_name, epilogue_name)){ --nb_args_to_handle;}
  /*
  std::cerr<<"nb_workers:"<<nb_workers<<" shared_memory:"<<shared_memory<<" dynamic_scheduling:"<<dynamic_scheduling<<" chunk_size:"<< chunk_size<< std::endl;
  if(prologue_name)  std::cerr<<" prologue:"<<prologue_name<<std::endl;
  if(epilogue_name) std::cerr<<" epilogue:"<< epilogue_name<<std::endl;
  */
  vars_container_t stack(nb_args_to_handle);

  for(int i(0); i!= nb_args_to_handle; ++i){
    stack[i]= scilab_allocated_var::get(i+1);
  }
  vars_container_t::iterator function_arg= std::find_if(stack.begin(), stack.end(), std::mem_fun(&scilab_allocated_var::function_name_or_ptr));
  wrapper w(stack.begin(), function_arg, stack.end(), Lhs);
  bool const with_threads(w.is_foreign_function() && shared_memory);
  simple_wrapper prologue(prologue_name), epilogue(epilogue_name);
  make_parallel_wrapper(w.args_data_begin(), w.args_sizes_begin(), w.args_nb_begin(), w.nb_rhs(), w.tasks_nb()
			,  w.res_data_begin(), w.res_sizes_begin(), Lhs, w.get_handle(), prologue, epilogue)(with_threads, nb_workers, dynamic_scheduling, chunk_size);

  for(std::size_t i(0); i != Lhs; ++i) {
    LhsVar(i+1)=Rhs+i+1;
  }


  PutLhsVar(); /* to be moved to gateway */

  return 0;
}
