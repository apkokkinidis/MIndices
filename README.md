# MIndices

##Algorithm implementation for the derivation of morphological indices using rays for the evaluation of said indices and bounding volume hierarchies to accelarate the operation of ray tracing.

  An algorithm has been developed and implemented, for the purpose of measuring the morphology of the structures of cancelous bone. Human samples which are represented in the form of binary voxels from magnetic micro-tomography scans, have been used to derive parameters, such as trabecullar thickness and number. These parameters server as an important tool in bone histomorphometry for the assessment of systemic skeletal diseases like osteoporosis. The algorithm directly derives these morphological indices by using rays that are directed towards the structures.

 The first step of the algorithm is to triangulate the binary voxels, this is achieved using the marching cubes algorithm. The algorithm uses bounding volumes that wrap the triangulated surfaces, forming a bounding volume hierarchy, in order to reduce the computational complexity of ray tracing. A parallel version of the algorithm has also been implemented that further reduces the time complexity needed to process each sample. The samples that were tested were anisotropic and the results of the metrics that were evaluated show a difference in their range of values.
