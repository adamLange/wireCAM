import OCC

class Slice:

  def __init__(wire,):
    self.shape = wire
    
class WireToGCode:

  def __init__(outputScale=1):
    self.outputScale = outputScale

  def addWire(wire):
    acc = OCC.BRepAdaptor.BRepAdaptor_CompoundCurve(wire)
    

  def gCode():
    
