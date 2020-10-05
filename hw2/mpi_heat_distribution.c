#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>



int mesh_cols =300, mesh_rows = 300;



int main(int argc, char **argv){

int rank, size;
MPI_Status status;

//initialize MPI environment
MPI_Initialize();
MPI_Comm_Rank(MPI_COMM_WORLD, &rank);
MPI_Comm_Size(MPI_COMM_WORLD,&size);

//define settings
int iter = 100;
//set temperature
int edge_temp = 20;
int fireplace_temp = 300;
int fireplace_width = 0.4* mesh_cols;
int fireplace_displ[] = {1, 0.5*(mesh_cols- fireplace_width) };
int fireplace_counts[] = {1, fireplace_width};

//buffer to store final result
float result[mesh_rows][mesh_cols];
//buffer used for computing in each partition with ghost points
float new_mesh[mesh_rows/size][mesh_cols];
float ghost_points[2][mesh_cols];
//float old_mesh[mesh_rows/size ][mesh_cols];

// Initialize  degree of the grid and the fireplace
for(int c=0; c< mesh_cols;c++ ){
	for(int r=0; r < mesh_rows;r++){
		if (	r>= fireplace_displ[0] && 
			r < (fireplace_displ[0]+fireplace_counts[0])&&	
			c >= fireplace_displ[1] && 
			c <(fireplace_displ[1]+fireplace_counts[1])){
			// set fireplace with 300 degree
			result[r][c] = fireplace_temp;
		}
		else{
			// set interior of mesh to 20 degree Celsius
			result[r][c] = edge_temp;
		}
	}
}

//iteration
for (int i =0;i < iter;i++){
//send , receive rows to/ from ghost point
if(rank==0){
	MPI_Send(new_mesh[mesh_rows/size-1],mesh_cols, MPI_FLOAT, rank+1,0, MPI_COMM_WORLD );
	MPI_Recv(ghost_points[1], mesh_cols, MPI_FLOAT, rank+1, 0, MPI_COMM_WORLD);
}
else if(rank!= size-1){
	MPI_Send(new_mesh[mesh_rows/size-1],mesh_cols, MPI_FLOAT, rank-1,0, MPI_COMM_WORLD );
	MPI_Recv(ghost_points[0], mesh_cols, MPI_FLOAT, rank-1, 0, MPI_COMM_WORLD);
	MPI_Recv(ghost_points[1], mesh_cols, MPI_FLOAT, rank+1, 0, MPI_COMM_WORLD);
	MPI_Send(new_mesh[mesh_rows/size-1],mesh_cols, MPI_FLOAT, rank+1,0, MPI_COMM_WORLD );
}
else if (rank == size-1){
	MPI_Send(new_mesh[0],mesh_cols, MPI_FLOAT, rank-1,0, MPI_COMM_WORLD );
	MPI_Recv(ghost_points[0], mesh_cols, MPI_FLOAT, rank-1, 0, MPI_COMM_WORLD);

}

//copy new buffer to old buffer
//update new buffer
//print buffer

}
//synchronize all computing nodes and collect partial results to the result buffer
MPI_Barrier(MPI_COMM_WORLD);
MPI_Gather(new_mesh,N/size,MPI_FLOAT, result, N/size, MPI_FLOAT, 0, MPI_COMM_WORLD );

if(rank ==0){
//print and save bitmap to pnm file in process 0
}

MPI_Finalize();

return 0;
}
