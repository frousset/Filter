#include "filter.h"

filter::filter()
{
    lower_thresh=0;
    upper_thresh=0;
    radius=1;
   
    for (int i=0; i<6; i++)
    {
      if(i<3) radius_mask[i]=1;
      T[i]=0;
    }
}

void filter::SetImage(char* input)
{
  ReaderType::Pointer reader = ReaderType::New(); 
  reader->SetFileName(input);
  
  try 
  {
    reader->Update();
  }
  catch ( itk::ExceptionObject& err )
  {
  std::cout << "ExecptionObject caught : " << err << std::endl;
  exit(EXIT_FAILURE);
  }    
  
  im = reader->GetOutput();
}


void filter::SetParameters()
{ 
  char c;
  char S[6][18] = {"Binary filter","Dilatation filter","Erosion filter","Gradient filter","Mean filter","Median filter"};
  
  std::cout<<std::endl;
  std::cout<<"Choose which filter(s) you want to use : (y/n)"<<std::endl;
  for (int i=0; i<6; i++)
  {
     std::cout<<"   --> "<<S[i]<<" : "; 
     std::cin>>c;  
     if (c == 'y' || c == 'Y') T[i] = 1;
  }
  std::cout<<std::endl;   

  if (T[0] || T[1] || T[2])
  {
    std::cout<<"Parameters for the binary/erode/dilate filter(s) are needed : "<<std::endl;
    std::cout<<"   --> Lower Threshold : "; std::cin>>lower_thresh;
    std::cout<<"   --> Upper Threshold : "; std::cin>>upper_thresh; 
     
    if (T[1] || T[2])
    {
      std::cout<<"   --> Radius for the structural element : "; std::cin>>radius; 
    }
    std::cout<<std::endl;
  }
  
  if (T[4] || T[5])
  {
    std::cout<<"Parameters for the mean/median filter(s) are needed : "<<std::endl;
    std::cout<<"   --> Mask radius along x : "; std::cin>>radius_mask[0];
    std::cout<<"   --> Mask radius along y : "; std::cin>>radius_mask[1];
    std::cout<<"   --> Mask radius along z : "; std::cin>>radius_mask[2];
    std::cout<<std::endl;
  }
  
}


void filter::ComputeFilters()
{
    char S[6][18] = {"Binary filter","Dilatation filter","Erosion filter","Gradient filter","Mean filter","Median filter"};
    
    for (int i=0; i<6; i++)
    {
      if (T[i])
      {
	switch (i)
	{
	  case 0:
	    this->Binary_Filter(lower_thresh,upper_thresh);
	    break;
	  case 1:
	    this->Dilate_Filter(lower_thresh,upper_thresh,radius);
	    break;	    
	  case 2:
	    this->Erode_Filter(lower_thresh,upper_thresh,radius);
	    break;
	  case 3:
	    this->Gradient_Filter();
	    break;
	  case 4:
	    this->Mean_Filter(radius_mask);
	    break;
	  case 5:
	    this->Median_Filter(radius_mask);
	    break;	
	}
	std::cout<<S[i]<<" : done"<<std::endl;
      }
    }
}


void filter::Binary_Filter(int lower_thresh, int upper_thresh)
{    
  FilterThreshType::Pointer filter = FilterThreshType::New();
  filter->SetInput( im );
  
  filter->SetOutsideValue( 0 );
  filter->SetInsideValue( 1 ); 
  
  filter->SetLowerThreshold( lower_thresh );
  filter->SetUpperThreshold( upper_thresh );  
  
  filter->Update();
   
    
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( "binary_filter.nrrd" );
  writer->SetInput( filter->GetOutput() );
  writer->SetUseCompression( true );

  try 
  {
    writer->Update();
  }
  catch ( itk::ExceptionObject& err )
  {
    std::cout << "ExecptionObject caught : " << err << std::endl;
    exit(EXIT_FAILURE);
  }  
}


void filter::Erode_Filter(int lower_thresh, int upper_thresh, int radius)
{    
  // Conversion of the image in a binary image
  FilterThreshType::Pointer thresh_filter = FilterThreshType::New();
  thresh_filter->SetInput( im );
  
  thresh_filter->SetOutsideValue( 0 );
  thresh_filter->SetInsideValue( 1 ); 
  
  thresh_filter->SetLowerThreshold( lower_thresh );
  thresh_filter->SetUpperThreshold( upper_thresh );  
    

  // Erosion
  StructuringElementType B;
  B.SetRadius( radius );
  B.CreateStructuringElement();
  
  ErodeFilterType::Pointer erode_filter = ErodeFilterType::New();
  erode_filter->SetKernel( B ); 
  erode_filter->SetInput( thresh_filter->GetOutput() );
  erode_filter->SetErodeValue( 1 );
    
  erode_filter->Update(); // It also calls thresh_filter->Update()
  
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( "erode_filter.nrrd" );
  writer->SetInput( erode_filter->GetOutput() );
  writer->SetUseCompression( true );

  try 
  {
    writer->Update();
  }
  catch ( itk::ExceptionObject& err )
  {
    std::cout << "ExecptionObject caught : " << err << std::endl;
    exit(EXIT_FAILURE);
  }  
}


void filter::Dilate_Filter(int lower_thresh, int upper_thresh, int radius)
{    
  // Conversion of the image in a binary image
  FilterThreshType::Pointer thresh_filter = FilterThreshType::New();
  thresh_filter->SetInput( im );
  
  thresh_filter->SetOutsideValue( 0 );
  thresh_filter->SetInsideValue( 1 ); 
  
  thresh_filter->SetLowerThreshold( lower_thresh );
  thresh_filter->SetUpperThreshold( upper_thresh );  
    

  // Erosion
  StructuringElementType B;
  B.SetRadius( radius );
  B.CreateStructuringElement();
  
  DilateFilterType::Pointer dilate_filter = DilateFilterType::New();
  dilate_filter->SetKernel( B ); 
  dilate_filter->SetInput( thresh_filter->GetOutput() );
  dilate_filter->SetDilateValue( 1 );
    
  dilate_filter->Update(); // It also calls thresh_filter->Update()
  
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( "dilate_filter.nrrd" );
  writer->SetInput( dilate_filter->GetOutput() );
  writer->SetUseCompression( true );

  try 
  {
    writer->Update();
  }
  catch ( itk::ExceptionObject& err )
  {
    std::cout << "ExecptionObject caught : " << err << std::endl;
    exit(EXIT_FAILURE);
  }  
}


void filter::Gradient_Filter()
{
  FilterGradType::Pointer filter = FilterGradType::New();
  filter->SetInput( im );
  filter->Update();
  
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName("gradient_filter.nrrd");
  writer->SetUseCompression( true );
    
    
  try 
  {
    writer->Update();
  }
  catch ( itk::ExceptionObject& err )
  {
    std::cout << "ExecptionObject caught : " << err << std::endl;
    exit(EXIT_FAILURE);
  }  
}


void filter::Mean_Filter(int radius[])
{
  FilterMeanType::Pointer filter = FilterMeanType::New();
  
  ImageType::SizeType indexRadius;
  indexRadius[0] = radius[0];
  indexRadius[1] = radius[1];
  indexRadius[2] = radius[2];
  
  filter->SetRadius( indexRadius );
  
  filter->SetInput( im );
  filter->Update();
  
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName("mean_filter.nrrd");
  writer->SetUseCompression( true );
    
    
  try 
  {
    writer->Update();
  }
  catch ( itk::ExceptionObject& err )
  {
    std::cout << "ExecptionObject caught : " << err << std::endl;
    exit(EXIT_FAILURE);
  }  
}


void filter::Median_Filter(int radius[])
{
  FilterMedianType::Pointer filter = FilterMedianType::New();
  
  ImageType::SizeType indexRadius;
  indexRadius[0] = radius[0];
  indexRadius[1] = radius[1];
  indexRadius[2] = radius[2];
  
  filter->SetRadius( indexRadius );
  
  filter->SetInput( im );
  filter->Update();
  
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName("median_filter.nrrd");
  writer->SetUseCompression( true );
    
    
  try 
  {
    writer->Update();
  }
  catch ( itk::ExceptionObject& err )
  {
    std::cout << "ExecptionObject caught : " << err << std::endl;
    exit(EXIT_FAILURE);
  }  
}
