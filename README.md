# MIndices

## Algorithm implementation for the derivation of morphological indices (trabecullar thickness and number) using rays and bounding volume hierarchies (BVH) to accelarate the operation of ray tracing.

  An algorithm has been developed and implemented, for the purpose of measuring the morphology of the structures of cancelous bone. Human samples which are represented in the form of binary voxels from magnetic micro-tomography scans, have been used to derive parameters, such as trabecullar thickness and number. These parameters serve as an important tool in bone histomorphometry for the assessment and treatment of systemic skeletal diseases like osteoporosis. The algorithm directly derives these morphological indices using rays that are directed towards the structures.

 The first step of the algorithm is to triangulate the binary voxel data, this is achieved using the marching cubes algorithm. The algorithm uses bounding volumes that wrap the triangulated surfaces, forming a bounding volume hierarchy, in order to reduce the computational complexity of ray tracing. A parallel version of the algorithm has also been implemented that further decreases the time required to process each sample.

 The software takes as input the filepath of the voxel file (inside Samples folder) from the command line. It then requires the dimenssions of the file in order to load the voxel array and triangulate it.
