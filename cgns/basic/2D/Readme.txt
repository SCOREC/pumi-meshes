Naming convention requires some explanation:

The code that uses theses meshes to perform unit testing against has been exclusively developed using meshes from Salome.
At the time of writing Salome outputs meshes in an hdf5 format from the v 1.10.X lineage.
I've always had trouble getting the cgns library to work properly if I link to hdf5-1.10.X so I always use hdf5 from 1.8.X.
Therefore, the names of the files are: 
mesh.cgns == file that comes from Salome
mesh.adf.cgns == file converted from hdf5-1.10.X to adf-based format
mesh.adf.hdf.cgns == file converted back to hdf-based format but this time v 1.8.X lineage.

