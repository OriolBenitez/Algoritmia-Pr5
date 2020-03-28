#include <stdio.h>
#define NAR 7
#define MAXARNO 5

typedef struct{
    unsigned n1,n2,estat;
}aresta;
typedef struct
{
    unsigned nodes[NAR+1];
    unsigned arvis[NAR];
    int naresvisit;
}estructuracami;

unsigned arestesdelnode(unsigned ,unsigned []);
int calculavalencies(unsigned,unsigned[]);
aresta larestes[NAR]={{0,1,0},{0,2,0},{0,2,0},{0,3,0},{0,3,0},
    {1,2,0},{1,3,0}};

int main(){
    unsigned i,arestespos[MAXARNO],npos=0,seguir=0,arestaposible=0;
    int valencies[4];
    estructuracami cami;

    cami.naresvisit=0;
    for(i=0;i<NAR;i++){
        printf("Aresta %d -> [%d,%d]\n",i,larestes[i].n1,larestes[i].n2);
    }
    printf("Les valencies dels nodes son:\n");
    calculavalencies(4,valencies);
    for(i=0;i<4;i++) printf("El node %u té: %u camins\n",i, valencies[i] );

    printf("Node Inici: ");
    cami.nodes[0]=-1;
    scanf("%u",cami.nodes);
    npos=arestesdelnode(cami.nodes[0], arestespos);
    seguir=1;
    do{
        if(npos > 0){
            printf("Teniu %d arestes per triar: \n",npos);
            for(i=0;i<npos;i++){
                printf("%d -> [%d,%d]\n",arestespos[i],larestes[arestespos[i]].n1,
                       larestes[arestespos[i]].n2);
            }
            do{
                printf("Trieu: ");
                scanf("%u",&cami.arvis[cami.naresvisit]);
                arestaposible=0;
                for(i=0;i<npos;i++){
                    if(arestespos[i]==cami.arvis[cami.naresvisit]) arestaposible=1;
                }
            }while(arestaposible==0);
            larestes[cami.arvis[(cami.naresvisit)]].estat=1;
            (cami.naresvisit)++;
            if(larestes[cami.arvis[(cami.naresvisit)-1]].n1==cami.nodes[(cami.naresvisit)-1]){
                cami.nodes[cami.naresvisit]=larestes[cami.arvis[(cami.naresvisit)-1]].n2;
            }
            else{
                cami.nodes[cami.naresvisit]=larestes[cami.arvis[(cami.naresvisit)-1]].n1;
            }
            for(i=0; i<cami.naresvisit; i++){
                printf("%d [%d,%d] ",cami.nodes[i],
                       larestes[cami.arvis[i]].n1,larestes[cami.arvis[i]].n2);
            }
            printf("%d\n",cami.nodes[cami.naresvisit]);
            npos=arestesdelnode(cami.nodes[cami.naresvisit],arestespos);
        }
        else{
            printf("No queden arestes disponibles.\n");
            printf("Arestes visitades (%d):\n",cami.naresvisit);
            for(i=0;i<cami.naresvisit;i++){
                printf("%d ",cami.arvis[i]);
            }
            seguir=0;
        }

    }while(seguir);
    printf("\n");
    return 0;
}

unsigned arestesdelnode(unsigned nn,unsigned llistap[]){
    unsigned i,nombre=0;
    for(i=0;i<NAR;i++){
        if(larestes[i].estat==0 && (larestes[i].n1==nn||larestes[i].n2==nn)){
            llistap[nombre]=i;
            nombre++;
        }
    }
    return nombre;
}
int calculavalencies(unsigned nnodes,unsigned val[]){
  unsigned i;
  for(i=0;i<nnodes;i++){
    val[i]=0;
  }
  for(i=0;i<NAR;i++){
        val[larestes[i].n1]++;
        val[larestes[i].n2]++;

        }
  unsigned max=0;
  for(i=0;i<nnodes;i++){
    if (val[i]>max) max=val[i];
  }
  return max;}
