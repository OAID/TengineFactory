#ifndef LINEFUNC
#define LINEFUNC

#include "LineBase.hpp"
namespace TFactory {

class LineFunc : public LineBase
{
private:
    /* data */
public:
    LineFunc(/* args */);
    ~LineFunc();
protected:
    void NoPostProcess();
    void NMS(Anchor anchor);
    void SplitTensorOutput();
};
}
#endif