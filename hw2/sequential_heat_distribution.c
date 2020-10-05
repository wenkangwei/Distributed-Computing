#include <stdlib.h>
#include <stdio.h>

#define WHITE    "15 15 15 "
#define RED      "15 00 00 "
#define ORANGE   "15 05 00 "
#define YELLOW   "15 10 00 "
#define LTGREEN  "00 13 00 "
#define GREEN    "05 10 00 "
#define LTBLUE   "00 05 10 "
#define BLUE     "00 00 10 "
#define DARKTEAL "00 05 05 "
#define BROWN    "03 03 00 "
#define BLACK    "00 00 00 "

int mesh_rows=10, mesh_cols=10;

void CalculateNew(float new[][mesh_cols], float old[][mesh_cols], int source_cnt[2], int source_displ[2], int rows, int cols);
void CopyNewToOld(float new[][mesh_cols], float old[][mesh_cols],int rows, int cols);
void PrintGrid(float grid[][mesh_cols], int xsource, int ysource, int rows, int cols);
void PrintImage(float grid[][mesh_cols], int rows, int cols);


int main(int argc, char **argv){


// temperature of edge
float edge_temp = 20;
//iterations for updating mesh values
int iter = 100;

char flag_print = 0;

if (argc >=2){
// set parameters if given parameters
	mesh_rows = atoi(argv[1]);
	mesh_cols = atoi(argv[1]);
	printf("Mesh width: %d, height: %d ", mesh_cols, mesh_rows);
	if (argc == 3){
	iter = atoi(argv[2]);
	printf("iterations: %d", iter);
	}
	printf("\n");
}
//define size of  a mesh and memory for the mesh
float old_mesh[mesh_rows][mesh_cols];
float new_mesh[mesh_rows][mesh_cols];

//define  a fireplace 
//temperature of fireplace =300 degree Celsius
float fireplace_temp = 300;
// count of rows/height and count of columns/width of fireplace
// The first element is height of fireplace, the second element is width of fireplace
int fireplace_counts[] =  {1, mesh_cols * 0.4};
//fireplace displacement, where it starts to draw fireplace
//The first element is the displacement of height of fireplace
//the second element is the displacement of width of fireplace
int fireplace_displ[] = { 1,0.5*(mesh_cols- fireplace_counts[1])} ;



//initialize interior of mesh and fireplace
for(int c=0; c< mesh_cols;c++ ){
	for(int r=0; r < mesh_rows;r++){
		if (	r>= fireplace_displ[0] && 
			r < (fireplace_displ[0]+fireplace_counts[0])&&	
			c >= fireplace_displ[1] && 
			c <(fireplace_displ[1]+fireplace_counts[1])){
			// set fireplace with 300 degree
			old_mesh[r][c] = fireplace_temp;
			new_mesh[r][c] = fireplace_temp;
		}
		else{
			// set interior of mesh to 20 degree Celsius
			old_mesh[r][c] = edge_temp;
			new_mesh[r][c] = edge_temp;
		}
	}
}

// iterations to calculate bitmap

for(int i=0; i< iter;i++ ){
	CopyNewToOld(new_mesh, old_mesh, mesh_rows, mesh_cols);
	CalculateNew(new_mesh, old_mesh, fireplace_counts, fireplace_displ, mesh_rows, mesh_cols);
	if (flag_print){
	PrintGrid(new_mesh, 0,0,mesh_rows,mesh_cols);
	}


}
	PrintImage(new_mesh,mesh_rows, mesh_cols);

return 0;
}


void CopyNewToOld(float new[][mesh_cols], float old[][mesh_cols],
		int rows, int cols)
{
// copy new computed array to old array
//
	for(int r=0; r <rows;r++){
		for(int c=0; c<cols; c++){
		old[r][c] = new[r][c];
		}
	}

}

void CalculateNew(float new[][mesh_cols], float old[][mesh_cols], int source_cnt[2], int source_displ[2], int rows, int cols){
//compute  new average value;
	for(int r=1; r <rows-1;r++){
		for(int c=1; c<cols-1; c++){
		if (	r>= source_displ[0] && 
			r < (source_displ[0]+source_cnt[0])&&	
			c >= source_displ[1] && 
			c <(source_displ[1]+source_cnt[1])){
		new[r][c] = old[r][c];
		}
		else{

		 new[r][c] = 0.25*(old[r-1][c]+ old[r+1][c]+ old[r][c-1]+old[r][c+1]);}
		}
}
}

void PrintGrid(float grid[][mesh_cols], int xsource, int ysource, int rows, int cols){
	// print mesh value
	for (int r=0; r<rows; r++){
		for(int c=0; c<cols; c++){
			printf(" %.2f", grid[r][c]);
		}
	printf("\n");
	}
	printf("\n\n\n");
}




void PrintImage(float grid[][mesh_cols], int rows, int cols){
	FILE *fp;
    fp = fopen("seq_graph.pnm","w");	

    fprintf(fp, "P3\n%d %d\n15\n", cols, rows);
	char * colors[10] = { RED, ORANGE, YELLOW, LTGREEN, GREEN, 
                         LTBLUE, BLUE, DARKTEAL, BROWN, BLACK };
	char * color;
	float color_range[] = {250, 180,120, 80,60,50,40,30,20};
	int num_ranges = sizeof(color_range)/sizeof(color_range[0]);
	for (int r=0; r<rows; r++){
		for(int c=0; c<cols; c++){
			// iterate each color range and select the color from color list
			for(int i=0; i< num_ranges+1; i++){
				if (i==0 && grid[r][c]>color_range[0] ){
					color= colors[0];
				}
				else if(i == num_ranges && grid[r][c]<=color_range[num_ranges-1]){
				color = colors[num_ranges-1];}
				else if(grid[r][c]<= color_range[i-1] && grid[r][c] >color_range[i]){
				color = colors[i];
				}
			}

				
				fprintf(fp, "%s", color);
		}
	fprintf(fp,"\n");
	}
	// close file pointer
	fclose(fp);
	system("pnmtojpeg seq_graph.pnm > seq_graph.jpg");

}
