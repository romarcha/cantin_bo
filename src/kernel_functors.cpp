#include "log.hpp"
#include "parser.hpp"
#include "kernel_functors.hpp"
#include "kernel_atomic.hpp"
#include "kernel_combined.hpp"

namespace bayesopt
{

  KernelFactory::KernelFactory()
  {
    registry["kConst"] = & create_func<ConstKernel>;
    registry["kLinear"] = & create_func<LinKernel>;
    registry["kLinearARD"] = & create_func<LinKernelARD>;

    registry["kMaternISO1"] = & create_func<MaternIso1>;
    registry["kMaternISO3"] = & create_func<MaternIso3>;
    registry["kMaternISO5"] = & create_func<MaternIso5>;
    registry["kMaternARD1"] = & create_func<MaternARD1>;
    registry["kMaternARD3"] = & create_func<MaternARD3>;
    registry["kMaternARD5"] = & create_func<MaternARD5>;
  
    registry["kSEARD"] = & create_func<SEArd>;
    registry["kSEISO"] = & create_func<SEIso>;

    registry["kSum"] = & create_func<KernelSum>;
    registry["kProd"] = & create_func<KernelProd>;
  }


  /** \brief Factory method for kernels. */
  Kernel* KernelFactory::create(kernel_name name, size_t input_dim)
  {
    Kernel* k_ptr;

    switch(name)
      {
      case K_MATERN_ISO1: k_ptr = new MaternIso1(); break;
      case K_MATERN_ISO3: k_ptr = new MaternIso3(); break;
      case K_MATERN_ISO5: k_ptr = new MaternIso5(); break;
      case K_SE_ISO: k_ptr = new SEIso(); break;
      case K_SE_ARD: k_ptr = new SEArd(); break;
      default:
	FILE_LOG(logERROR) << "Error: kernel function not supported.";
	return NULL;
      }

    k_ptr->init(input_dim);
    return k_ptr;
  };

  /** 
   * \brief Factory model for kernel functions
   * This function is based on the libgp library by Manuel Blum
   *      https://bitbucket.org/mblum/libgp
   * which follows the squeme of GPML by Rasmussen and Nickisch
   *     http://www.gaussianprocess.org/gpml/code/matlab/doc/
   * @param name string with the kernel structure
   * @param imput_dim number of input dimensions
   * @return kernel pointer
   */
  Kernel* KernelFactory::create(std::string name, size_t input_dim)
  {
    Kernel *kFunc;
    std::string os, os1, os2;
    utils::parseExpresion(name,os,os1,os2);

    std::map<std::string,KernelFactory::create_func_definition>::iterator it = registry.find(os);
    if (it == registry.end()) 
      {
	FILE_LOG(logERROR) << "Error: Fatal error while parsing "
			   << "kernel function: " << os 
			   << " not found" << std::endl;
	return NULL;
      } 
    kFunc = it->second();
    if (os1.length() == 0 && os2.length() == 0) 
      {
	kFunc->init(input_dim);
      } 
    else 
      {
	kFunc->init(input_dim, create(os1,input_dim), create(os2,input_dim));
      }
    return kFunc;

  };

} //namespace bayesopt
