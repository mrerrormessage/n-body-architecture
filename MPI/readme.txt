So I think I want to use two MPI implementations. I've installed one- OpenMPI. The other major one, MPICH, I have downloaded but not yet installed. 

OpenMPI is run by invoking mpicc. The OpenMPI FAQ is at http://www.open-mpi.org/faq/

I needed to get mpirun to get this working. To do that I needed to download lam-runtime package. After installing the package and using mpirun -np 2 hello (for a simple "hello world" program, I was informed I needed to run lamboot first. 

got libopenmpi-dbg, which also installed openmpi-bin, think that helped slightly, no longer missing ORTE libraries. 

after getting libopal3.6.1-dev (although I'm not sure that's what did it), I was able to use mpirun.openmpi to run hello world (even multiple times) without trouble, although I did need to use my login. 

as regards mpich, I am through step 5, but the temp directory will likely go on my next reboot, so.....

