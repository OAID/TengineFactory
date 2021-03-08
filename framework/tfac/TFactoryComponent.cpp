#include "TFactoryComponent.hpp"

namespace TFactory {

TFactoryComponent::TFactoryComponent()
{
    mImageComponent = new ImageComponent();
}

void TFactoryComponent::setImageComponent(uint8_t* data, int w, int h, int channel)
{
    mImageComponent->width = w;
    mImageComponent->height = h;
    mImageComponent->channel = channel;
    mImageComponent->data = data;
}

void TFactoryComponent::setFunctionComponent(struct FunctionComponent* com)
{
    mFunctionComs.push_back(com);
}

void TFactoryComponent::clearCached()
{
    for (std::size_t i = 0; i < mFunctionComs.size(); i++)
    {
        delete mFunctionComs[i];
    }
    mFunctionComs.clear();
}

TFactoryComponent::~TFactoryComponent()
{
    delete mImageComponent;
    clearCached();
}

}