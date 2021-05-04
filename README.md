# Object transform

This node transforms the **/detection/lidar_detector/objects** messages from the Lidars frame to the **map** frame and publish it to the **converted_euclidean_objects** topic as a MarkerArray.

The points of the detected objects are stored as points, the centroids of the polygons are stored as pose.  
For some reasons the Rviz don't show it correctly but it's only vizualisations problem. 


