#include <iostream>
#include "filter.h"

int main (int argc, char *argv[])
{
  if (argc<2) std::cout<<"Not enough parameters"<<std::endl;
  else
  {
    filter F;
    F.SetImage(argv[1]);
    F.SetParameters();
      
    std::cout<<"Program is processing ... Please wait"<<std::endl; 
    F.ComputeFilters();
    std::cout<<"Complete processing"<<std::endl;
    std::cout<<std::endl;
  }
  

  
  return 0;
}