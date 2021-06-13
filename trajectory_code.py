import serial.tools.list_ports
import openrouteservice
import folium



client=openrouteservice.Client(key='5b3ce3597851110001cf62483cb00499f7af4732bcb60001ac426576') # Specify


############################### serial input ########################
ports = serial.tools.list_ports.comports()
serialins = serial.Serial()

port_list = []
for port in ports:
    port_list.append(str(port))
    print(str(port))

val = input("the uart is in COM: ")

for x in range (0,len(port_list)):
    if port_list[x].startswith("COM"+str(val)):
        port_value = "COM"+str(val)
        print(port_list[x])


serialins.baudrate = 9600
serialins.port = port_value
serialins.open()
#######################
while 1:
    if serialins.in_waiting:
        msg = serialins.readline()
        print(msg.decode('ascii'))
        break
##################send u command #############
u=input()
serialins.write(u.encode('ascii'))  #if 'U'
#serialins.write('U')
print('DONE')

#####################read data ###############
points = []
count = 0

no_points = 150
counter = 0
while counter < no_points:
    if serialins.in_waiting:
        packet = serialins.readline()
        pp = packet.decode('ascii')      #'a'
        len_point = len(packet.decode('ascii'))
        print(pp)
        temp = pp[0:len_point - 3]
        print(temp)
        if temp == 'Fini':
            break
        points.append(pp[0:len_point - 3])
        counter+=1

#################################################################################
print(f'points: {points}')


########################### drawing maps #######################################
latt = []
long = []
for point in range(len(points)):
    p=float(points[point])
    if (point % 2) ==0:
        latt.append(p)
    else:
        long.append(p)

print(f'latt: {latt}')
print(f'long: {long}')

# latt=[49.417613,49.416496,49.404986]
# long=[8.667612, 8.676281, 8.676624]
y=len(latt)
coordinates = []

for i in range(0,y):
    coordinates.append([latt[i],long[i]])

coordinates_reversed = []
for i in range(0,y):
    coordinates_reversed.append([long[i],latt[i]])

# coordinates = [[49.417613 ,8.667612],[  49.416496, 8.676281],[49.404986  ,8.676624]]
# coordinates_reversed = [[8.667612,49.417613  ],[ 8.676281, 49.416496],[8.676624,49.404986]]
print(f'coordinates: {coordinates}')
print(f'coordinates: {coordinates_reversed}')

map_directions = folium.Map(location=coordinates_reversed[0],zoom_start=15)

#for i in range(0,y):
folium.Marker(location=coordinates_reversed[0],popup=(f'{coordinates_reversed[0]}')).add_to(map_directions)
folium.Marker(location=coordinates_reversed[len(latt)-1],popup=(f'{coordinates_reversed[len(latt)-1]}')).add_to(map_directions)

route = client.directions(coordinates=coordinates,profile='driving-car',format='geojson')


folium.GeoJson(route, name='route').add_to(map_directions)

folium.LayerControl().add_to(map_directions)



map_directions.save("map_directions.html")




######################################################################################
