#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <pthread.h>
#define NTHREADSD 10

typedef struct{
  double* vp;
  double md;
  int idx;
  struct vptree* inner;
  struct vptree* outer;
}vptree;

typedef struct{
  double * X
  int d;
  int n;
  int id;
  double * dis;
}dwrap;

typedef struct{
  double *X;
  int d;
  int n;
  int *ids;
  vptree * inner;
}treewrap;

double swap(double * arr,int a,int b){
  double temp=arr[a];
  arr[a]=arr[b];
  arr[b]=temp;
}

void * treew(void * arg){
  treewrap * temp= (treewrap*)arg;
  temp->inner = buildtree(temp->X,temp->ids,temp->n,temp->d);
  pthread_exit(NULL);
}

void * distWrapper(void * arg){
  dwrap  * temp = (dwrap *)arg;
  int block=(((temp->n)-1)/NTHREADSD)+1;
  for(int j=id*block;j<(id+1)*block||j<n-1;j++){
    temp->dis[j]=dist(temp->X[(n-1)*temp->d],temp->X[j*temp->d],temp->d);
  }
  pthread_exit(NULL);
}

double dist(double *point1,double *point2,int d){
  double distance =0;
  for(int i=0;i<d;i++){
    distance +=pow(point1[i]-point2[i],2);
  }
  distance = sqrt(distance);
  return distance;
}

int partition(double * X,int left ,int right,int pivotIndex){

  pivotValue = X[pivotIndex];
  swap(X,pivotIndex,right);
  int storeIndex = left;
  for(i=left;i<right;i++){
    if(X[i]<pivotValue){
      swap(X,storeIndex,i);
      storeIndex++;
    }
  }
  swap(X,right,storeIndex);
  return storeIndex;
}

double QS(double * X,int left,int right,int k){
  if(left==right){
    return left;
  }
  pivotIndex=left+(((int)rand())%(right-left+1));
  pivotIndex=partition(X,left,right,pivotIndex);
  if(k==pivotIndex){
    return X[k];
  }
  else if(k<pivotIndex){
    return QS(X,left,pivotIndex-1,k);
  }
  else{
    return QS(X,pivotIndex+1,right,k);
  }

}


vptree * buildtree(double *X,int* ids,int n,int d){
  vptree * tree ;
  double * innerP;
  double * outerP;
  int innerC=0;
  int outerC=0;
  int * innerIds;
  int * outerIds;

  dwrap points[n-1];

  pthread_t  dthreads[NTHREADSD];



  tree = (vptree*)malloc(sizeof(vptree));


  tree->vp=(double*)malloc(d*sizeof(double));
  for(int i=0;i<d;i++){
    tree->vp[i]=Χ[(n-1)*d+i];
  }
  tree->idx=ids[n-1];
  double * dis;
  dis= (double*)malloc((n-1)*sizeof(double));


  for(int i=0;i<(n-1);i++){
    dis[i]=dist(tree->vp,X[i*d],d);
  }

  for(int i=0;i<NTHREADSD;i++){
    dwrap[i].X=X;
    dwrap[i].d=d;
    dwrap[i].dis=dis;
    dwrap[i].n=n;
  }


  for(int i=0;i<NTHREADSD;i++){
    dwrap[i].id=i;
    pthread_create(&dthreads[i],NULL,distWrapper,((void*)(&dwrap[i]));
  }
  for(int i=0;i<NTHREADSD;i++){
    pthread_join(dthreads[i],NULL);
  }
  double * data;
  data =(double *)malloc((n-1)*sizeof(double));
  memcpy(data,dis,(n-1)*sizeof(double));
  tree->md=QS(data,0,n-2,n/2);


  for(int i=0;i<(n-1);i++){

    if(dis[i]<=tree->md){
      innerC++;
    }
    else{
      outerC++;
    }
  }
    innerP=(double *)malloc((innerC)*d*sizeof(double));
    outerP=(double *)malloc(outerC*d*sizeof(double));
    innerIds=(int *)malloc((innerC)*sizeof(int));
    outerIds=(int *)malloc(outerC*sizeof(int));
    innerC=0;
    outerC=0;
  for(int i=0;i<(n-1);i++){
    if(dis[i]<=tree->md){
      for(int dim=0;dim<d;dim++){
        innerP[innerC*d+dim]=X[i*d+dim];
      }
      innerIds[innerC]=ids[i];
      innerC++;
    }
    else{
      for(int dim=0;dim<d;dim++){
        outerP[outerC*d+dim]=X[i*d+dim];
      }
      outerIds[outerC]=ids[i];
      outerC++;
    }
  }
  treewrap * trp;
  trp->ids=innerIds;
  trp->X=innerP;
  trp->n=innerC;
  trp->d=d;
  trp->inner=tree->inner;
  pthread_t thr;
  pthread_create(&thr,NULL,treew,(void *)trp);

  tree->outer = buildtree(outerP,outerIds,outerC,d);

  return tree;
}




vptree* buildvp(double *X,int n,int d){
vptree * tree;
  double * data;
  data =(double *)malloc(n*d*sizeof(double));
  memcpy(data,X,n*d*sizeof(double));
  int * ids;
  ids =(int *)malloc(n*sizeof(int));
  for(int i=0;i<n;i++){
    ids[i]=i;
  }
  
  tree = buildtree(data,ids,n,d);
  return tree;
}


vptree * getInner(vptree * T){
  return T->inner;
}


vptree * getOuter(vptree * T){
  return T->outer;
}


double getMD(vptree * T){
  return T->md;
}

double * getVP(vptree * T){
  return T->vp;
}

int getIDX(vptree * T){
  return T->idx;
}
