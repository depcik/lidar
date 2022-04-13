from lidar_lite import Lidar_Lite
lidar = Lidar_Lite()
import math

connected = lidar.connect(1)

# while connected < -1:
#     print ("Not Connected")
# else:
#     print ("Connected")

for i in range(100):
    distance = lidar.getDistance()
    print("Distance = %s" % (distance))
    height= distance
    print("Hieght = %s" % (distance))
    width= distance
    print("Width = %s" % (distance))
    


    sample_data= open("sample_data.xyz", "a")

    with open("sample_data.xyz", "a") as f:
        lis= [[distance],[height], [width]]
        for x in zip(*lis):
            f.write("{0}\t{1}\t{2}\n".format(*x))
            
sample_data.close()


        




