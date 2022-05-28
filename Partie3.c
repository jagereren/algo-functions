#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define AND &&
#define OR ||
#define ISNOT !=
#define NOT !
#define then
#define RAND_MAX 100 // pas sûr qu'il fonctionne

typedef enum { FALSE, TRUE} bool;

typedef struct bloc_image{ 
	bool toutnoir ;
	struct bloc_image * fils[4] ;
	} bloc_image ;
	
typedef bloc_image *image ;

// Déclaration des fonctions
bool EstNoire(image I);
bool EstBlanche(image I);
void affichageNormal(image I);
void affichageProfondeur(image I,int prof);
image Construit_blanc();
image Construit_noir();
image Construit_composee(image ihg, image ihd, image ibg, image ibd);
double Aire(image I);
void Negatif(image *I);
image Copie(image I);
void Rendmemoire(image I);
image Lecture();
bool UnionNoire(image I1, image I2);
void Intersection(image *I1, image I2);
int CompteSousImagesGrises(image I);


double fraction(int a, int b){
	return a/b;
}

bool EstNoire(image I){ 
	if(I!=NULL){
		if((I->toutnoir))
			return TRUE;
		else
			return FALSE;
	}
	return FALSE;
}

bool EstBlanche(image I){ 
	if(I==NULL)
		return TRUE;
	else if(I->toutnoir)
		return FALSE;
	else {
		for(int j=0;j<4;j++) {
			if(EstBlanche(I->fils[j])==FALSE)
				break;
		}
	}
}

void affichageNormal(image I) { 
	if(I==NULL) // Cas Blanche Simple
		printf("B");
	else if(I->toutnoir) // Cas Noire simple
		printf("N");
	else { // Cas composée
		printf("+");
		for(int j=0;j<4;j++)
			affichageNormal(I->fils[j]);
		printf(" ");
	}
}

void affichageProfondeur(image I,int prof) { 
	if(I==NULL) // Cas Blanche Simple
		printf("B%d ",prof);
	else if(I->toutnoir) // Cas Noire simple
		printf("N%d ",prof);
	else { // Cas composée
		printf("+%d ",prof);
		for(int j=0;j<4;j++)
			affichageProfondeur(I->fils[j],prof+1);
	}
}

image Construit_blanc(){
	return NULL;
}

image Construit_noir(){
	image I = (image) malloc(sizeof(bloc_image)); 
	I->toutnoir = TRUE;
	for(int j=0;j<4;j++)
		I->fils[j] = NULL;
	return I;
}

image Construit_composee(image ihg, image ihd, image ibg, image ibd){
	image I = (image) malloc(sizeof(bloc_image));
	I->toutnoir = FALSE;
	I->fils[0] = ihg;
	I->fils[1] = ihd;
	I->fils[2] = ibg;
	I->fils[3] = ibd;
}

double Aire(image I){ 
	if(I==NULL) // Blanche
		return 0;
	else if(I->toutnoir) // Noire
		return 1;
	else //plutôt que EstBlanche car complexité trop grande
		return Aire(I->fils[0])/4+Aire(I->fils[1])/4+Aire(I->fils[2])/4+Aire(I->fils[3])/4;
}

void Negatif(image *I) {
	if(*(I)==NULL) {
		Rendmemoire(*I);
		*I=Construit_noir();
	}
	else if((*I)->toutnoir) {
		Rendmemoire(*I);
		*I=Construit_blanc();
	}
	else {
		for(int i=0;i<4;i++) {
			Negatif(&(*I)->fils[i]);
		}
	}
}

image Copie(image I){
	if(I==NULL) {
		return Construit_blanc();
	}
	else if(I->toutnoir){
		return Construit_noir();
	}
	else { 
		Construit_composee(Copie(I->fils[0]),Copie(I->fils[1]),Copie(I->fils[2]),Copie(I->fils[3]));
	}
	
}

void Rendmemoire(image I){ 
	if(I!=NULL AND !I->toutnoir) {
		for(int i=0;i<4;i++){
			Rendmemoire(I->fils[i]);
		}
	}
	free(I);
}

image Lecture(){ 
	char symb;
	scanf(" %c",&symb);
	if(symb == 'B') 
		return Construit_blanc();
	else if(symb=='N')
		return Construit_noir();
	else if(symb=='+') {
		image ihg = Lecture();
		image ihd = Lecture();
		image ibg = Lecture();
		image ibd = Lecture();
		return Construit_composee(ihg,ihd,ibg,ibd); // on instancie d'abord les quatre lectures, 
													//car le compilateur ne lit pas dans le bon ordre les arguments pris directement
	}
}


void Intersection(image *I1, image I2){
	if((*I1)!=NULL){ // Si je ne vérifie pas cette condition d'abord, le compilateur n'accepte pas de vérifier le toutnoir pour une image NULL
		if((*I1)->toutnoir){
			if(I2==NULL){
				Rendmemoire(*I1);
				*I1=NULL;
			}
			else if(!I2->toutnoir) {
				for(int i=0;i<4;i++)
					Intersection(I1,I2->fils[i]);
			}
		}			
		else if(!(*I1)->toutnoir){
			if(I2==NULL){
				for(int i=0;i<4;i++)
					Intersection(&(*I1)->fils[i],I2);
			}
			else if(!I2->toutnoir){
				for(int i=0;i<4;i++)
					Intersection(&(*I1)->fils[i],I2->fils[i]);					
			}	
		}
	}
}

int CompteSousImagesGrises(image I){
	if(I==NULL OR I->toutnoir){
		return 0;
	}
	else {
		double a = Aire(I);
		if(a >=0.33 AND a <=0.66)
			return 1+CompteSousImagesGrises(I->fils[0]) + CompteSousImagesGrises(I->fils[1]) + CompteSousImagesGrises(I->fils[2]) + CompteSousImagesGrises(I->fils[3]);
		else if(a>0.66)
			return CompteSousImagesGrises(I->fils[0]) + CompteSousImagesGrises(I->fils[1]) + CompteSousImagesGrises(I->fils[2]) + CompteSousImagesGrises(I->fils[3]);
		else // Si l'aire est inférieure à 0,33 pas besoin de continuer car l'aire sera forcément plus petite par la suite
			return 0;
	}
}


bool UnionNoire(image I1, image I2){
		if(I1==NULL){
			if(I2==NULL) {
				return FALSE;
			}
			else if(!I2->toutnoir)
				return UnionNoire(I1,I2->fils[0]) AND UnionNoire(I1,I2->fils[1]) AND UnionNoire(I1,I2->fils[2]) AND UnionNoire(I1,I2->fils[3]);
			else
				return TRUE;
		}	
		else if(I2==NULL) {
			if(!I1->toutnoir)
				return UnionNoire(I1->fils[0],I2) AND UnionNoire(I1->fils[1],I2) AND UnionNoire(I1->fils[2],I2) AND UnionNoire(I1->fils[3],I2);	
			else
				return TRUE;
		}
		else if(!I1->toutnoir AND !I2->toutnoir)
			return UnionNoire(I1->fils[0],I2->fils[0]) AND UnionNoire(I1->fils[1],I2->fils[1]) AND UnionNoire(I1->fils[2],I2->fils[2]) AND UnionNoire(I1->fils[3],I2->fils[3]);
		else // Deux noires
			return TRUE;
}



/* brouillon pour afficher la première ligne

void bistest2(image I, int pac, int pk) {
	if(I==NULL)
		for(int i=0;i<pac;i++)
			printf(".");
	else if(I->toutnoir)
		for(int i=0;i<pac;i++)
			printf("8");
	else if(pac==1) 
		printf("-");
	else{
		bistest2(I->fils[0],pac/2,pk);
		bistest2(I->fils[1],pac/2,pk);
	}
}	
	
void premligne2k(image I, int k){ // Affiche premier ligne si la résolution le permet
	//int pac = pow(2,k);
	
	int pk=1;
	// calcul de puissance car le pow ne marchait pas
	for(int i=0;i<k;i++)
		pk *= 2;
	bistest2(I,pk,pk);
}

*/

void afficheligne2k(image I, int pk, int partie, int longueur){ 
	if(pk==1){ // Si on arrive à une taille "minimum" on peut afficher
		if(I==NULL)
			//for(int i=0;i<longueur;i++) {
				printf(".");
				//longueur++;
			//}
		else if(I->toutnoir)
			//for(int i=0;i<longueur;i++) {
				printf("8");
			//	longueur++;
			//}
		else {
			printf("-");
			//longueur++;
		}
	}
	else{
		pk /= 2; // On divise par deux lorsqu'on change d'échelle en décomposé
		if(I==NULL OR I->toutnoir){
			afficheligne2k(I,pk,partie,longueur); // On lance 2 appels récursifs à chaque changement d'échelle (vu qu'on divise pk par 2) pour afficher par la suite l'image blanche ou noire (ou -)
			afficheligne2k(I,pk,partie,longueur);
		}
		else if(I != NULL AND !I->toutnoir) { // Image décomposée, on lance en récursif
			if(longueur < partie){ // On se situe dans la partie haute, longueur nous permettant d'avancer
				afficheligne2k(I->fils[0],pk,partie-pk/2,longueur); 
				afficheligne2k(I->fils[1],pk,partie-pk/2,longueur);
			}
			else{ // Partie basse donc on lance sur les deux fils du bas
				afficheligne2k(I->fils[2],pk,partie+pk/2,longueur); 
				afficheligne2k(I->fils[3],pk,partie+pk/2,longueur);
			}
		}
	}
}

void Affichage2Pixel(image I, int k){
	//int pk = pow(2,k);
	int pk=1;
	// calcul de puissance car le pow ne marchait pas
	for(int i=0;i<k;i++)
		pk *= 2;
	for(int j=0;j<pk;j++){ // On boucle pk fois pour le nombre de lignes
		afficheligne2k(I,pk,pk/2,j);
		printf("\n");
	}
}

/* Première version de Aleabis qui ne fonctionne pas lors de la condition !..toutnoir, ce qui est étrange, 
 * donc premières conditions changées en valeur de k lors de la deuxième version
void Aleabis(image *I, int k, int n, bool *noirajoute){
	//affichageNormal(*I);
	if(*I==NULL) {
		if(k==0)
			*I = Construit_noir();
		else
			*I = Construit_composee(Construit_blanc(),Construit_blanc(),Construit_blanc(),Construit_blanc());
	}
	else if(!(&(*I)->toutnoir)) { 
		int randfils = rand() % 4; // Les valeurs de rand ne vont pas jusqu'à 100 mais beaucoup plus, or cela fonctionne quand même
		//printf("%d",randfils);
		Aleabis(&(*I)->fils[randfils],k-1,n);
	}
}
*/


void Aleabis(image *I, int k, bool *noirajoute){ // Le noirajoute va essentiellement nous servir à ne pas avoir plusieurs points noirs au même emplacement
	if(k==0){
		if(*I==NULL) {
			*I = Construit_noir();
			*noirajoute = TRUE;
		}
	}
	else {
		int randfils = rand() % 4; // Les valeurs de rand ne vont pas jusqu'à 100 mais beaucoup plus, or cela fonctionne quand même avec les bons restes 0 1 2 3
		if(*I==NULL)
			*I = Construit_composee(Construit_blanc(),Construit_blanc(),Construit_blanc(),Construit_blanc());
		Aleabis(&(*I)->fils[randfils],k-1,&(*noirajoute));
	}
}

image Alea(int k, int n){ // Note : j'aurais aussi pu utiliser un arbre, ce qui réduirait la complexité en temps mais augmenterait la complexité en espace
	bool noirajoute = FALSE;
	image I = NULL;
	while(n!=0){ // si un point noir a été ajouté on peut incrémenter le n sinon on recommence jusqu'à que cela soit le cas
		Aleabis(&I,k,&noirajoute);
		if(noirajoute) {
			n--;
			noirajoute = FALSE;
		}
	}
	return I;
}


// Aller jusqu'à profondeur 1 et tirer au hasard pour chacun des 4 fils ?
/*
image nbis(int k){
	if(k==0){
		if(*I==NULL) {
			*I = Construit_noir();
			*noirajoute = TRUE;
		}
	}
	else {
		int randfils = rand() % 4; // Les valeurs de rand ne vont pas jusqu'à 100 mais beaucoup plus, or cela fonctionne quand même avec les bons restes 0 1 2 3
		if(*I==NULL)
			*I = Construit_composee(Construit_blanc(),Construit_blanc(),Construit_blanc(),Construit_blanc());
		Aleabis(&(*I)->fils[randfils],k-1,&(*noirajoute));
	}	
}

image Nebuleuse(int k)
	int pk=1;
	// calcul de puissance 
	for(int i=0;i<k;i++)
		pk *= 2;
	nbis(pk);
		
}
*/

int main(int argc, char** argv)
{	
	srand(time(NULL)); // utile pour que le rand() n'affiche pas les mêmes valeurs à la suite
	image b = Construit_blanc();
	image n = Construit_noir();
	image I = Construit_composee(b,b,n,b);
	image I1 = Construit_composee(Construit_composee(n,n,n,n),n,n,Construit_composee(n,b,b,b));
	printf("Affichage normal de l'image I1: ");
	affichageNormal(I1);
	printf("\nTaux de noir de l'image I1: %f\n",Aire(I1));
	printf("Affichage en profondeur de l'image I1: ");
	affichageProfondeur(I1,0);
	//Negatif(&I);
	printf("\nAffichage normal de la copie de l'image I1: ");
	affichageNormal(Copie(I1));
	printf("\n");
	if(UnionNoire(I,I1))
		printf ("Union Noire entre I et I1 !\n");
	else
		printf ("Pas d'union noire entre I et I1\n");
	//Intersection(&I,I1);
	printf("Image(s) grise(s) : %d\n",CompteSousImagesGrises(I1));
	printf("Affichage 2Kpixel de I1\n");
	Affichage2Pixel(I1,2);
	printf("\n");
	Affichage2Pixel(Alea(2,3),2); // k profondeur, n noires
	return 0;
}
