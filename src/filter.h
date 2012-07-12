#ifndef filters_h_

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkGradientMagnitudeImageFilter.h>
#include <itkMeanImageFilter.h>
#include <itkMedianImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkBinaryBallStructuringElement.h>
#include <itkBinaryErodeImageFilter.h>
#include <itkBinaryDilateImageFilter.h>

typedef short PixelType;
const unsigned int Dimension = 3;

typedef itk::Image< PixelType,Dimension > ImageType;
typedef itk::ImageFileReader< ImageType > ReaderType;
typedef itk::ImageFileWriter< ImageType > WriterType;

typedef itk::GradientMagnitudeImageFilter< ImageType,ImageType > FilterGradType;
typedef itk::MeanImageFilter< ImageType,ImageType > FilterMeanType;
typedef itk::MedianImageFilter< ImageType,ImageType > FilterMedianType;
typedef itk::BinaryThresholdImageFilter< ImageType,ImageType > FilterThreshType;

typedef itk::BinaryBallStructuringElement< PixelType,3 > StructuringElementType;
typedef itk::BinaryErodeImageFilter< ImageType,ImageType,StructuringElementType > ErodeFilterType;
typedef itk::BinaryDilateImageFilter< ImageType,ImageType,StructuringElementType > DilateFilterType;


class filter
{
  public:
    filter();
    ~filter(){};
    
    void SetImage(char* input);
    void SetParameters();
    void ComputeFilters();
    
    void Gradient_Filter();
    void Mean_Filter(int rad[]);
    void Median_Filter(int rad[]);    
    void Binary_Filter(int lower_thresh, int upper_thresh);
    void Erode_Filter(int lower_thresh, int upper_thresh, int radius);
    void Dilate_Filter(int lower_thresh, int upper_thresh, int radius);    
    
  private:  
    ImageType::Pointer im;
    int lower_thresh,upper_thresh,radius;
    int radius_mask[3];
    int T[6];
    
};

#endif