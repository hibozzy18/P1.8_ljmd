"""
working on the python interface
"""
from ctypes import *


#impotrt DSo
dso = CDLL("./lmjd.so")

#Create struct
class _mdsys(Structure):
    _fields_ = [("natoms", c_int), ("nfi", c_int), ("nsteps", c_int), ("dt", c_double), ("mass", c_double),  ("epsilon", c_double),  ("sigma", c_double), ("box", c_double), ("rcut", c_double), ("ekin", c_double), ("epot", c_double),("temp", c_double), ("rx", POINTER(c_double)), ("ry", POINTER(c_double)), ("rz", POINTER(c_double)), ("vx", POINTER(c_double)), ("vy", POINTER(c_double)), ("vz", POINTER(c_double))]


#pass object type
dso.test.argtypes = [POINTER(_mdsys)]

#Read from input file
def read_inp(filename):
    list = []
    with open(filename, 'r') as f:
        for line in f:
            values = [s for s in line.split()]
            try:
                list.append(float(values[0]))
            except ValueError:
                list.append(values[0])
    f.closed
    return list

#Read from rest file
def read_rest(filename, start, end):
    from itertools import islice
    x, y, z = [], [], []
    with open(filename, 'r') as f:
        for line in islice(f, start, end):
            values = [float(s) for s in line.split()]
            x.append(float(values[0]))
            y.append(float(values[1]))
            z.append(float(values[2]))
    f.closed
    return x, y, z


#return data from file to lists
rx, ry, rz = read_rest("data/argon_108.rest", r_start, r_end)
vx, vy, vz = read_rest("data/argon_108.rest", v_start, v_end)

#element size
elem_size = c_double*num_atoms

#create empty elements
array_rx = elem_size()
array_ry = elem_size()
array_rz = elem_size()
array_vx = elem_size()
array_vy = elem_size()
array_vz = elem_size()

#load vectors from th r file
array_rx = get_array(rx, array_rx)
array_ry = get_array(ry, array_ry)
array_rz = get_array(rz, array_rz)
array_vx = get_array(vx, array_vx)
array_vy = get_array(vy, array_vy)
array_vz = get_array(vz, array_vz)

# print(array_rx[0])
#populate  data of a data structure
#and return the mdsys

# print(list)


# mdsys = _mdsys(natoms=108, nfi=100, nsteps=1000, mass=600, epsilon=200, sigma=78, rcut=90.00, box=500,
#  dt = 8.9, ekin = 0.0, epot = 0.0, temp = 0.0, rx=array_rx,
# ry = array_ry, rz=array_rz, vx=array_vx, vy=array_vy, vz=array_vz)
mdsys = _mdsys(natoms=int(list[0]), mass=list[1], epsilon=list[2], sigma=list[3], rcut=list[4], box=list[5],
               nsteps=int(list[9]), dt=list[10], nfi=0, ekin=0.0, epot=0.0, temp=0.0, rx=array_rx,
               ry=array_ry, rz=array_rz, vx=array_vx, vy=array_vy, vz=array_vz)


print(mdsys.natoms)
print(mdsys.mass)
print(mdsys.epsilon)
print(mdsys.sigma)
print(mdsys.rcut)
print(mdsys.box)
print(mdsys.nsteps)
print(mdsys.dt)
print(mdsys.rx[0])
dso.test(byref(mdsys))
