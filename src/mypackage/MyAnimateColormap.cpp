///////////////////////////////////////////////////////////////////////
//
// Illustrates the use of time-out methods
//
///////////////////////////////////////////////////////////////////////

#include <hxcore/HxController.h>
#include <hxcolor/HxColormap256.h>
#include <mypackage/MyAnimateColormap.h>  // declaration of this class

HX_INIT_CLASS(MyAnimateColormap,HxCompModule)

MyAnimateColormap::MyAnimateColormap() :
    HxCompModule(HxColormap256::getClassTypeId()),
    portInterval(this,"interval"),
    portAnimate(this,"animate")
{
    portInterval.setMinMax(0,500);
    portInterval.setValue(500);
}

MyAnimateColormap::~MyAnimateColormap()
{
    stopAnimating();
}

void MyAnimateColormap::compute()
{
    HxColormap256* input = (HxColormap256*) portData.source();

    if (!input) {
        stopAnimating();
        return;
    }

    HxColormap256* output = (HxColormap256*) getResult();
    if (output && !output->isOfType(HxColormap256::getClassTypeId()))
        output = 0;

    if (!output) {
        output = (HxColormap256*) input->duplicate();
        output->setLabel(input->getName());
    } else if (portData.isNew())
        *output = *input;

    if (portInterval.isNew())
        stopAnimating();

    if (portAnimate.getValue())
        startAnimating();
    else stopAnimating();

    if (isAnimating) {
        int n = output->getLength();
        float* data = output->getDataPtr();

        float save[16];
        memcpy(save,data,16*sizeof(float));
        memmove(data,&data[16],(4*n-16)*sizeof(float));
        memcpy(&data[4*n-16],save,16*sizeof(float));
    }

    output->touch();
    setResult(output);
}

void MyAnimateColormap::startAnimating()
{
    if (!isAnimating) {
        int interval = portInterval.getValue();
        theController->addTimeOutMethod(this,
	    (HxTimeOutMethod)&HxObject::fire2,interval);
        isAnimating = 1;
    }
}

void MyAnimateColormap::stopAnimating()
{
    if (isAnimating) {
        theController->removeTimeOutMethod(this,
	    (HxTimeOutMethod)&HxObject::fire2);
        isAnimating = 0;
    }
}

