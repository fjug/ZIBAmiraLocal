/////////////////////////////////////////////////////////////////
//
// Example of an ITK compute module
//
/////////////////////////////////////////////////////////////////

#include "MyITKFilter.h"

#include <hxcore/HxMessage.h>
#include <hxfield/HxUniformScalarField3.h>

#include <hxitk/HxItkImageImporter.h>
#include <hxitk/HxItkImageExporter.h>
#include <hxitk/HxItkProgressObserver.h>

#include "itkMeanImageFilter.h"

HX_INIT_CLASS(MyITKFilter,HxCompModule)

MyITKFilter::MyITKFilter() :
     HxCompModule(HxUniformScalarField3::getClassTypeId())
    ,portKernelSize(this,"kernelSize")
    ,portDoIt(this, "action", 1)
{
    portKernelSize.setMinMax(1,5);
    portKernelSize.setValue(1);

    portDoIt.setLabel(0,"Apply");
}

template <typename Type>
void meanImageFilter(HxUniformScalarField3* inField, McHandle<HxUniformScalarField3>& resultField, const int kernelSize) {

    typedef itk::Image< Type, 3 > ImageType;
    typedef itk::MeanImageFilter<ImageType, ImageType > FilterType;

    typename FilterType::Pointer filter = FilterType::New();

    HxItkImageImporter<Type> importer(inField);
    filter->SetInput(importer.GetOutput());
    HxItkImageExporter<ImageType> exporter(filter->GetOutput(), resultField.ptr());

    /// Display filter progress within Amira's progressbar
    HxItkProgressObserver progress;
    progress.startObservingProcessObject(filter);
    progress.setProgressMessage(McString("Applying mean filter..."));

    // Set the filter kernel size
    typename ImageType::SizeType radius;
    radius[0] = kernelSize;
    radius[1] = kernelSize;
    radius[2] = kernelSize;
    filter->SetRadius(radius);

    // Execute the filter
    filter->Update();
    
    resultField = exporter.getResult();
}

void MyITKFilter::compute()
{

    // Check whether doIt button was hit
    if (!portDoIt.wasHit()) return;

    // Access the input data object. The member portData (which is of
    // type HxConnection) is inherited from the base class HxModule.
    HxUniformScalarField3* inField = hxconnection_cast<HxUniformScalarField3>( portData ); 

    // Check whether the input port is connected
    if (!inField) return;

    // Access size of data volume:
    const int* dims = inField->lattice.dims();

    // Check if there is a result which we can reuse.
    McHandle<HxUniformScalarField3> resultField = mcinterface_cast<HxUniformScalarField3>( getResult() );

    // Check for proper type.
    if (resultField && !resultField->isOfType(HxUniformScalarField3::getClassTypeId()))
        resultField = 0;

    // Check if size and primType still match the current input:
    if (resultField) {
        const int* outdims = resultField->lattice.dims();
        if (dims[0]!=outdims[0] || dims[1]!=outdims[1] || dims[2]!=outdims[2] || 
            resultField->primType() != resultField->primType())
        resultField=0;
    }

    int kernelSize = portKernelSize.getValue();

    switch (inField->lattice.primType()) {
            case McPrimType::mc_uint8:
                meanImageFilter<unsigned char>(inField, resultField, kernelSize);
                break;
            case McPrimType::mc_uint16:
                meanImageFilter<unsigned short>(inField, resultField, kernelSize);
                break;
            case McPrimType::mc_int16:
                meanImageFilter<short>(inField, resultField, kernelSize);
                break;
            case McPrimType::mc_int32:
                meanImageFilter<int>(inField, resultField, kernelSize);
                break;
            case McPrimType::mc_float:
                meanImageFilter<float>(inField, resultField, kernelSize);
                break;
            default:
                theMsg->printf("Unsupported data type");
                return;
        }

    if (resultField)
        resultField->composeLabel(inField->getName(),"-filtered");
    setResult(resultField); // register result
}
