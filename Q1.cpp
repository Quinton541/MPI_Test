#include<iostream>
#include<mpi.h>
#include<math.h>
#include<time.h>
using namespace std;
clock_t start,finish;
int main(){
	int numprocs,myid;
	double allSum=0.0,tempSum=0.0;
	int k;
	MPI_Init(NULL,NULL);
	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	numprocs--;
	if(myid==0){
		start=clock();
		cin>>k;
		for(int i=1;i<=numprocs;i++) MPI_Send(&k,1,MPI_INT,i,2,MPI_COMM_WORLD);
		int *p=new int[k+10];
		for(int i=1;i<=k;i++){
			p[i]=i*(i+1);
		}
		for(int i=1;i<=k;i++){
			MPI_Send(&p[i],1,MPI_INT,i%numprocs+1,1,MPI_COMM_WORLD);
		}
	}
	if(myid!=0){
		tempSum=0.0;
		MPI_Recv(&k,1,MPI_INT,0,2,MPI_COMM_WORLD,&status);
		for(int i=1;i<=k;i++){
			if(i%numprocs+1==myid){
				int temp;
				MPI_Recv(&temp,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);
				tempSum+=sqrt(double(temp));
			}
		}
	}
	MPI_Reduce(&tempSum,&allSum,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
	if(myid==0){
		cout<<allSum<<endl;
		finish=clock();
		printf("RunningTime:%lfs\n",double(finish-start)/CLOCKS_PER_SEC);
	}
	MPI_Finalize();
	return 0;
}
