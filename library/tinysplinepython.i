%module tinysplinepython

//********************************************************
//*                                                      *
//* TsFloatList (Python)                                 *
//*                                                      *
//********************************************************
%ignore TsFloatList;

%pythoncode %{
import abc
import collections

class TsFloatList(collections.MutableSequence):
    """A wrapper for swig float arrays"""
    
    @abc.abstractmethod
    def getSwigPtr(self):
        return None

    def __getitem__(self, index):
        return float_array_getitem(self.getSwigPtr(), index)
        
    def __setitem__(self, index, value):
        prev = float_array_getitem(self.getSwigPtr(), index)
        float_array_setitem(self.getSwigPtr(), index, value)
        return prev
        
    def __str__(self):
        s = "["
        lst = self.__len__() - 1
        for i in range(0, lst):
            s = s + str(self.__getitem__(i)) + ", "
        s = s + str(self.__getitem__(lst)) + "]"
        return s

    def __delitem__(self, index):
        raise NotImplementedError("Deleting items is not supported.")
        
    def insert(self, index, value):
        raise NotImplementedError("Inserting items is not supported.")
%}

//********************************************************
//*                                                      *
//* TsBSpline (Python)                                   *
//*                                                      *
//********************************************************
%rename(ctrlp_p) TsBSpline::ctrlp;
%rename(knots_p) TsBSpline::knots;
%ignore TsBSpline::operator=;

%pythoncode %{
class TsCtrlpList(TsFloatList):
    def __init__(self):
       self.bspline = None
       
    def getSwigPtr(self):
        return self.bspline.ctrlp_p()
        
    def __len__(self):
        return self.bspline.nCtrlp * self.bspline.dim

class TsKnotList(TsFloatList):
    def __init__(self):
       self.bspline = None
       
    def getSwigPtr(self):
        return self.bspline.knots_p()
        
    def __len__(self):
        return self.bspline.nKnots
%}
        
//********************************************************
//*                                                      *
//* TsDeBoorNet (Python)                                 *
//*                                                      *
//********************************************************
%rename(points_p) TsDeBoorNet::points;
%rename(result_p) TsDeBoorNet::result;
%ignore TsDeBoorNet::operator=;

%pythoncode %{
class TsPointList(TsFloatList):
    def __init__(self):
       self.deboornet = None
       
    def getSwigPtr(self):
        return self.deboornet.points_p()
        
    def __len__(self):
        return self.deboornet.nPoints * self.deboornet.dim

class TsResultList(TsFloatList):
    def __init__(self):
       self.deboornet = None
       
    def getSwigPtr(self):
        return self.deboornet.result_p()
        
    def __len__(self):
        return self.deboornet.dim
%}


%include "tinyspline.i"


// NOTE:
//
// The following code must be included AFTER "tinyspline.i",
// otherwise the classes TsBSpline and TsDeBoorNet are unknown

%pythoncode %{
TsBSpline.ctrlpList = TsCtrlpList()
TsBSpline.knotList = TsKnotList()

def ts_init_ctrlpList(c, b):
    c.bspline = b
    return c

def ts_init_knotList(k, b):
    k.bspline = b
    return k

TsBSpline.ctrlp = property(lambda self: ts_init_ctrlpList(self.ctrlpList, self))
TsBSpline.knots = property(lambda self: ts_init_knotList(self.knotList, self))


TsDeBoorNet.pointList = TsPointList()
TsDeBoorNet.resultList = TsResultList()
    
def ts_init_pointList(p, d):
    p.deboornet = d
    return p

def ts_init_resultList(r, d):
    r.deboornet = d
    return r
    
TsDeBoorNet.points = property(lambda self: ts_init_pointList(self.pointList, self))
TsDeBoorNet.result = property(lambda self: ts_init_resultList(self.resultList, self))
%}


