import OCC
import OCC.BRepAlgoAPI
import OCC.BRepPrimAPI
from OCC.gp import *
import OCCUtils
import OCC.BRepMesh

class PostSetter:

  def __init__(self,shape):
    self.shape = shape
    self.positions = []
    self.outputScale = 1/25.4
    self.rapid_z = 1.9/25.4

    OCC.BRepMesh.BRepMesh_IncrementalMesh(shape,1e-1);
    bb = OCC.Bnd.Bnd_Box()
    OCC.BRepBndLib.brepbndlib_Add(shape,bb)
    x0,y0,z0,x1,y1,z1 = bb.Get()
    self.z0 = z0 - 100
    self.delta_z = z1 - z0 + 200


  def addPosition(self,x,y,r):
    ax = gp_Ax2(gp_Pnt(x,y,self.z0),gp_Dir(0,0,1),gp_Dir(1,0,0));
    mc = OCC.BRepPrimAPI.BRepPrimAPI_MakeCylinder(ax,r,self.delta_z)
    cyl = mc.Shape()
    ms = OCC.BRepAlgoAPI.BRepAlgoAPI_Section(self.shape,cyl)
    sectionShape = ms.Shape()

    OCC.BRepMesh.BRepMesh_IncrementalMesh(sectionShape,1e-2);
    bb = OCC.Bnd.Bnd_Box()
    OCC.BRepBndLib.brepbndlib_Add(sectionShape,bb)
    x0,y0,z0,x1,y1,z1 = bb.Get()

    self.positions.append(gp_Pnt(x,y,z0))

  def gcode(self):
    gcode = 'G90\n'
    gcode += 'G0 Z{}\n'.format(self.rapid_z)

    for pnt in self.positions:
      x = pnt.X() * self.outputScale
      y = pnt.Y() * self.outputScale
      z = pnt.Z() * self.outputScale

      gcode += 'G0 X{} Y{}\n'.format(x,y)
      gcode += 'G0 Z{}\n'.format(z)
      gcode += 'M0\n'
      gcode += 'G0 Z{}\n'.format(self.rapid_z)
    return gcode
