#include<iostream>
#include<time.h>
#include<mpi.h>
#define N 100000000
using namespace std;
clock_t start,finish;
int main(){
	int myid,numprocs;
	double local=0.0, dx=(90.0)/N;
	double inte,x,sum=0.0;
	double maxRunningTime;
	MPI_Status status;
	int temp;
	MPI_Init(NULL,NULL);
	start=clock();
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	numprocs--;
	if(myid==0){
		temp=0;
		for(int i=1;i<=numprocs;i++){
			MPI_Send(&temp,1,MPI_INT,i,2,MPI_COMM_WORLD);//发送一个无意义的变量，保证0进程最先运行 以便统计时间。
		}
	}
	if(myid!=0){
		MPI_Recv(&temp,1,MPI_INT,0,2,MPI_COMM_WORLD,&status);
		for(int i=myid;i<N;i+=numprocs){
                        x=10+i*dx+dx/2;
                        local+=x*x*x*dx;
                }
                MPI_Send(&local,1,MPI_DOUBLE,0,1,MPI_COMM_WORLD);
        }
	if(myid==0){
		for(int i=1;i<=numprocs;i++){
			MPI_Recv(&inte,1,MPI_DOUBLE,i,1,MPI_COMM_WORLD,&status);
			sum+=inte;
		}
		printf("The integration of x^3 between [10,100] is：%lf\n",sum);
	}
	finish=clock();
	double RunningTime=double(finish-start)/CLOCKS_PER_SEC;
	MPI_Reduce(&RunningTime,&maxRunningTime,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
	MPI_Finalize();
        if(myid==0) printf("RunningTime: %lfs\n",maxRunningTime);
	return 0;
}
