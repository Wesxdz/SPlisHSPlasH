
import pysplishsplash as sph
import pysplishsplash.Utilities.SceneLoaderStructs as Scenes
import numpy as np
from scipy.spatial.transform import Rotation as R
import os

def main():
    base = sph.Exec.SimulatorBase()
    output_dir = os.path.abspath('/home/aeri/git/SPlisHSPlasH/bin/')
    base.init(useGui=True, sceneFile=sph.Extras.Scenes.Empty, outputDir=output_dir)
    
    # See complex_scene.py for syntax
    base.setValueFloat(base.PAUSE_AT, -1.0)
    base.setValueBool(base.PAUSE, False)
    base.setValueFloat(base.STOP_AT, 4.0)
    base.setValueUInt(base.NUM_STEPS_PER_RENDER, 4)
    base.setValueBool(base.STATE_EXPORT, False)
    base.setValueFloat(base.STATE_EXPORT_FPS, 24.0)
    base.setValueString(base.PARTICLE_EXPORT_ATTRIBUTES, "velocity;density")
    base.activateExporter("Partio Exporter", False)
    base.activateExporter("VTK Exporter", False)
    base.activateExporter("OpenVDB Exporter", True)
    
    gui = sph.GUI.Simulator_GUI_imgui(base)
    base.setGui(gui)

    scene = sph.Exec.SceneConfiguration.getCurrent().getScene()
    
    scene.particleRadius = 0.02
    scene.boundaryModels.append(Scenes.BoundaryData(meshFile="../models/bounds.obj", translation=[0.0, 0.0, 0.0], scale=[1.0, 1.0, 1.0], color=[0.1, 0.4, 0.5, 1.0], isWall=True, mapInvert=True, mapResolution=[25, 25, 25]))
    scene.fluidBlocks.append(Scenes.FluidBlock(id='Fluid', box=Scenes.Box([-0.46452391147613525, 0.6956751942634583, -0.46452391147613525], [0.46452391147613525, 1.5500272512435913, 0.46452391147613525]), mode=0, initialVelocity=[0.0, 0.0, 0.0]))
    base.run()
if __name__ == "__main__":
    main()
