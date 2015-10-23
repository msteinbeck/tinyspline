/* NOTE:
 *
 * All method names start with 'ts_' to prevent clashes
 * with method/function names of target languages.
 */
#include "tinysplinecpp.h"

/********************************************************
*                                                       *
* TsFloatList                                           *
*                                                       *
********************************************************/
class TsFloatList {
public:
    virtual ~TsFloatList() {}

    float ts_get(const int index)
    {
        if (index < 0) {
            std::string text = "Negative index: " + std::to_string(index);
            throw std::out_of_range(text);
        }
        const int len = ts_len();
        if (index >= len) {
            std::string text = "Index: " + std::to_string(index);
            text += ", Size: " + std::to_string(len);
            throw std::out_of_range(text);
        }
        return ts_ptr()[index];
    }

    float ts_set(const int index, const float value)
    {
        const float prev = ts_get(index); // checks index implicitly
        ts_ptr()[index] = value;
        return prev;
    }

    int ts_indexOf(const float value)
    {
        const int len = ts_len();
        for (int i = 0; i < len; i++) {
            if (ts_fequals(ts_ptr()[i], value))
                return i;
        }
        return -1;
    }

    bool ts_contains(const float value)
    {
        return ts_indexOf(value) >= 0;
    }

    std::string ts_toString()
    {
        std::string s = "[";
        const int lst = ts_len() - 1;
        for (int i = 0; i < lst; i++) {
            s += std::to_string(ts_ptr()[i]);
            s += ", ";
        }
        if (lst >= 0) {
            s += std::to_string(ts_ptr()[lst]);
        }
        s += "]";
        return s;
    }

    virtual int ts_len() = 0;
protected:
    virtual float* ts_ptr() = 0;
};


/********************************************************
*                                                       *
* TsBSpline related lists                               *
*                                                       *
********************************************************/
class TsBSplineList : public TsFloatList {
protected:
    TsBSpline* bspline;
public:
    TsBSplineList(TsBSpline* bspline) : bspline(bspline) {}
    virtual ~TsBSplineList() {}
};

class TsCtrlpList : public TsBSplineList {
public:
    TsCtrlpList(TsBSpline* bspline) : TsBSplineList(bspline) {}
    virtual ~TsCtrlpList() {}
    virtual float* ts_ptr() {return bspline->ctrlp();}
    virtual int ts_len()
    {
        return bspline == nullptr ? 0 :
               static_cast<int>(bspline->nCtrlp() * bspline->dim());
    }
};

class TsKnotList : public TsBSplineList {
public:
    TsKnotList(TsBSpline* bspline) : TsBSplineList(bspline) {}
    virtual ~TsKnotList() {}
    virtual float* ts_ptr() {return bspline->knots();}
    virtual int ts_len()
    {
        return bspline == nullptr ? 0 :
               static_cast<int>(bspline->nKnots());
    }
};


/********************************************************
*                                                       *
* TsDeBoorNet related lists                             *
*                                                       *
********************************************************/
class TsDeBoorNetList : public TsFloatList {
protected:
    TsDeBoorNet* deboornet;
public:
    TsDeBoorNetList(TsDeBoorNet* deboornet) : deboornet(deboornet) {}
    virtual ~TsDeBoorNetList() {}
};

class TsPointList : public TsDeBoorNetList {
public:
    TsPointList(TsDeBoorNet* deboornet) : TsDeBoorNetList(deboornet) {}
    virtual ~TsPointList() {}
    virtual float* ts_ptr() {return deboornet->points();}
    virtual int ts_len()
    {
        return deboornet == nullptr ? 0 :
               static_cast<int>(deboornet->nPoints() * deboornet->dim());
    }
};

class TsResultList : public TsDeBoorNetList {
public:
    TsResultList(TsDeBoorNet* deboornet) : TsDeBoorNetList(deboornet) {}
    virtual ~TsResultList() {}
    virtual float* ts_ptr() {return deboornet->result();}
    virtual int ts_len()
    {
        return deboornet == nullptr ? 0 :
               static_cast<int>(deboornet->dim());
    }
};