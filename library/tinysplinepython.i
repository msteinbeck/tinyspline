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
%ignore TsBSpline::operator=;

%pythoncode %{
class TsCtrlpList(TsFloatList):
    def __init__(self, bspline):
       self.bspline = bspline
       
    def getSwigPtr(self):
        return self.bspline.ctrlp()
        
    def __len__(self):
        return self.bspline.nCtrlp * self.bspline.dim

class TsKnotList(TsFloatList):
    def __init__(self, bspline):
       self.bspline = bspline
       
    def getSwigPtr(self):
        return self.bspline.knots()
        
    def __len__(self):
        return self.bspline.nKnots
%}
        
//********************************************************
//*                                                      *
//* TsDeBoorNet (Python)                                 *
//*                                                      *
//********************************************************
%ignore TsDeBoorNet::operator=;

%pythoncode %{
class TsPointList(TsFloatList):
    def __init__(self, deboornet):
       self.deboornet = deboornet
       
    def getSwigPtr(self):
        return self.deboornet.points()
        
    def __len__(self):
        return self.deboornet.nPoints * self.deboornet.dim

class TsResultList(TsFloatList):
    def __init__(self, deboornet):
       self.deboornet = deboornet
       
    def getSwigPtr(self):
        return self.deboornet.result()
        
    def __len__(self):
        return self.deboornet.dim
%}


%include "tinyspline.i"


// NOTE:
//
// The following code must be included AFTER "tinyspline.i",
// otherwise the classes TsBSpline and TsDeBoorNet are unknown

%pythoncode %{
def getCtrlp(self):
    return TsCtrlpList(self)

def getKnots(self):
    return TsKnotList(self)
    
TsBSpline.getCtrlp = getCtrlp
TsBSpline.getKnots = getKnots
    
def getPoints(self):
    return TsPointList(self)
    
def getResult(self):
    return TsResultList(self)
    
TsDeBoorNet.getPoints = getPoints
TsDeBoorNet.getResult = getResult
%}


