#include "tinysplinecpp.h"

class TsFloatList {
public:
    TsDeBoorNet* ts_deboornet;
    TsBSpline* ts_bspline;

    TsFloatList() : ts_deboornet(nullptr), ts_bspline(nullptr) {}
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
        if (lst < 0) {
            s += std::to_string(ts_ptr()[lst]);
        }
        s += "]";
        return s;
    }

    virtual float* ts_ptr() = 0;
    virtual int ts_len() = 0;
};

class TsPointList : public TsFloatList {
public:
    virtual ~TsPointList() {}
    virtual float* ts_ptr() {return ts_deboornet->points();}
    virtual int ts_len()
    {
        return ts_deboornet == nullptr ? 0 :
            ts_deboornet->nPoints() * ts_deboornet->dim();
    }
};

class TsResultList : public TsFloatList {
public:
    virtual ~TsResultList() {}
    virtual float* ts_ptr() {return ts_deboornet->result();}
    virtual int ts_len()
    {
        return ts_deboornet == nullptr ? 0 :
            ts_deboornet->dim();
    }
};

class TsCtrlpList : public TsFloatList {
public:
    virtual ~TsCtrlpList() {}
    virtual float* ts_ptr() {return ts_bspline->ctrlp();}
    virtual int ts_len()
    {
        return ts_bspline == nullptr ? 0 :
            ts_bspline->nCtrlp() * ts_bspline->dim();
    }
};

class TsKnotList : public TsFloatList {
public:
    virtual ~TsKnotList() {}
    virtual float* ts_ptr() {return ts_bspline->knots();}
    virtual int ts_len()
    {
        return ts_bspline == nullptr ? 0 :
            ts_bspline->nKnots();
    }
};