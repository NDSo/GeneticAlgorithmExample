
//#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

struct Room{
	char* name;
	int size;
	bool multimedia = false;
};

struct CRN{
	char* name;
	char* prof;
	int size;
	bool multimedia = false;
};

struct Period{
	char* day;
	char* time;
};

struct gene{
	int room;
	int period;
};
struct chromosome{
	gene genes[27];
	int fitness = 0;
};


static char* courseList[27] = {
	"cs015  ",
	"cs456  ", "cs456  ",
	"cs1783 ",
	"cs455  ", "cs455  ",
	"cs015  ", "cs015  ", "cs015  ", "cs015  ",
	"math001", "math001", "math001",
	"math002", "math002", "math002",
	"Soc100 ", "Soc100 ", "Soc100 ",
	"CS047  ", "CS047  ",
	"PSY200 ", "PSY200 ", "PSY200 ",
	"cs045  ", "cs045  ",
	"cs015  "
};
static char* proffesorList[27] = {
	"Hagerich   ",
	"Bilitski   ", "Bilitski   ", "Bilitski   ",
	"Hagerich   ", "Hagerich   ",
	"Smigla     ",
	"IM         ", "IM         ",
	"Frederick  ",
	"Ferencek   ", "Ferencek   ", "Ferencek   ",
	"Thomson    ", "Thomson    ", "Thomson    ",
	"Darling    ", "Darling    ", "Darling    ",
	"Bilitski   ", "Bilitski   ",
	"Hinderliter", "Hinderliter", "Hinderliter",
	"Hagerich   ", "Hagerich   ",
	"Smigla     "
};
static int courseSizeList[27] = {
	20, 20, 20, 15, 20, 20, 35, 35, 35, 35, 40, 50, 60, 40, 50, 60, 45, 40, 35, 15, 15, 30, 35, 30, 20, 20, 20
};
static char* dayList[2] = {
	"M W F ",
	"T H   "
};
static char* timeList[14] = {
	"8    ",
	"9    ",
	"10   ",
	"11   ",
	"12   ",
	"1    ",
	"2    ",
	"3    ",
	"8    ",
	"9:30 ",
	"11   ",
	"12:30",
	"2    ",
	"3:30 "
};
static char* roomList[9] = {
	"BL134    ",
	"BL138    ",
	"KR224    ",
	"KR124    ",
	"KR206    ",
	"Biddle123",
	"Biddle125",
	"Biddle205",
	"ES100    "
};
static int roomSizeList[9] = {
	30, 50, 40, 20, 30, 35, 40, 45, 100
};

static Period periods[14];
static Room rooms[9];
static CRN CRNs[27];

void CalculateFitness(chromosome* population, int n);
chromosome* ProCreate(chromosome* population, int n, float pc, float pm);
void Crossover(chromosome* parentA, chromosome* parentB, chromosome* newPopulation, float pc);
void MutatePopulation(chromosome* newPopulation, int n, float pm);
int MaxFitness(chromosome* population, int n);
void PrintPopulation(chromosome* population, int n);


int main(int argc, char *argv[]){

	if (argc != 5){
		printf("Incorrect argument length, run GAassignment.bat");
		getchar();
		return 0;
		}
	///////////////INITIALIZE//////////////////////////////////////
	srand(time(NULL));
	////////////////////////////////////////////////

	for (int i = 0; i < 14; i++){
		if (i < 8){
			periods[i].day = dayList[0];
		}
		else{
			periods[i].day = dayList[1];
		}
		periods[i].time = timeList[i];
	}

	/////////////////////////////////////////////

	for (int i = 0; i < 9; i++){
		rooms[i].name = roomList[i];
		rooms[i].size = roomSizeList[i];
	}

	rooms[0].multimedia = true;
	rooms[1].multimedia = true;
	rooms[4].multimedia = true;
	rooms[8].multimedia = true;
	
	////////////////////////////////////////////

	for (int i = 0; i < 27; i++){
		CRNs[i].size = courseSizeList[i];
		CRNs[i].name = courseList[i];
		CRNs[i].prof = proffesorList[i];
	}
	for (int i = 1; i < 7; i++){
		CRNs[i].multimedia = true;
	}
	for (int i = 16; i < 21; i++){
		CRNs[i].multimedia = true;
	}
	for (int i = 24; i < 27; i++){
		CRNs[i].multimedia = true;
	}
	///////////////////////////////////////////////////////////////////////////

	int n = atoi(argv[1]);
	int maxgen = atoi(argv[2]);
	float pc = atof(argv[3]);
	float pm = atof(argv[4]);
	chromosome* population = new chromosome[n];
	chromosome* temp = population;
	chromosome* bestChromosome = new chromosome;
	int max_fitness_ever = -90000;
	int max_fit_found_gen = -1;
	////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	
	// Random population
	for (int i = 0; i < n; i++){
		for (int k = 0; k < 27; k++){
			(population + i)->genes[k].period = rand() % 14;
			(population + i)->genes[k].room = rand() % 9;
		}
	}
	
	
	for (int k = 0; k < maxgen; k++){
		//Calculate Fitness
		CalculateFitness(population, n);

		//Check for Best fitness Ever
		if ((population + MaxFitness(population, n))->fitness > max_fitness_ever){
			max_fitness_ever = (population + MaxFitness(population, n))->fitness;
			bestChromosome->fitness = max_fitness_ever;
			max_fit_found_gen = k;
			for (int q = 0; q < 27; ++q){
				bestChromosome->genes[q] = (population + MaxFitness(population, n))->genes[q];
			}
		}

		//Print every 100 gen
		if (k % 100 == 0){
			printf("\n/////////////////////////////////////////////////\n");
			PrintPopulation(population, n);
		}

		// Begin proliferation
		temp = population;
		population = ProCreate(population, n, pc, pm);
		delete[] temp;
	}

	//Print best found specimen
	printf("\n/////////////////////////////\n ");
	PrintPopulation(bestChromosome, 1);
	printf("Found In Gen: %d\n", max_fit_found_gen);
	getchar();

	return 0;
}

void PrintPopulation(chromosome* population, int n){


	for (int i = 0; i < n; i++){
		printf("--------------------------------------------------------------------\n");
		printf("CRN  CrsName  Prof        Size  MM?  | RmName  RmSize MM?  | Days  Time\n");
		for (int k = 0; k < 27; k++){

			printf("%3d  %s  %s %3d  %5s | %s %3d %5s | %s %s\n",
				k,
				CRNs[k].name,
				CRNs[k].prof,
				CRNs[k].size,
				CRNs[k].multimedia ? "true" : "false",
				rooms[population[i].genes[k].room].name,
				rooms[population[i].genes[k].room].size,
				rooms[population[i].genes[k].room].multimedia ? "true" : "false",
				periods[population[i].genes[k].period].day,
				////////////WTF IS GOING ON/////Fixed it
				periods[population[i].genes[k].period].time
				);

		}
		printf("FITNESS: %d \n", (population + i)->fitness);

	}

}

void CalculateFitness(chromosome* population, int n){

	for (int i = 0; i < n; i++){

		int fitnessAcc = 0;

		for (int k = 0; k < 27; k++){
			if (rooms[(population + i)->genes[k].room].multimedia && CRNs[k].multimedia){
				fitnessAcc += 20;
			}
			else if (!(rooms[(population + i)->genes[k].room].multimedia) && CRNs[k].multimedia){
				fitnessAcc -= 50;
			}

			if (rooms[(population + i)->genes[k].room].size < CRNs[k].size){
				fitnessAcc -= 70;
			}
			else {
				fitnessAcc += 20;
			}
		}
		(population + i)->fitness = fitnessAcc;
	}

	for (int i = 0; i < n; i++){

		int fitnessAcc = 0;

		for (int k = 0; k < 27; k++){
			for (int j = k + 1; j < 27; j++){
				if (((population + i)->genes[k].period == (population + i)->genes[j].period) &&
					((population + i)->genes[k].room == (population + i)->genes[j].room)){
					fitnessAcc -= 300;
				}
				if (((population + i)->genes[k].period == (population + i)->genes[j].period) &&
					(strcmp(CRNs[k].prof, CRNs[j].prof) == 0)){
					fitnessAcc -= 300;
				}
			}
		}
		(population + i)->fitness += fitnessAcc;
	}
}

int MaxFitness(chromosome* population, int n){

	int max_fit_gen_index = 0;

	for (int i = 0; i < n; i++){

		if ((population + i)->fitness > (population + max_fit_gen_index)->fitness){
			max_fit_gen_index = i;
		}
	}
	return max_fit_gen_index;
}

chromosome* ProCreate(chromosome* population, int n, float pc, float pm){

	chromosome* newPopulation = new chromosome[n];
	chromosome* parentA, *parentB;
	int best_fit_index;
	int prev_fit_value = 9000;


	for (int j = 0; j < n; j += 2){
		//////////////////Selection//////////////////////// Elitist/Random?
		best_fit_index = 0;
		for (int i = 0; i < n; i++){

			if ((population + i)->fitness > (population + best_fit_index)->fitness && (population + i)->fitness < prev_fit_value){

				best_fit_index = i;

			}
		}
		prev_fit_value = (population + best_fit_index)->fitness;

		parentA = (population + best_fit_index);
		parentB = (population + (rand() % n));
		/////////////////////////////////////////////////////

		Crossover(parentA, parentB, (newPopulation + j), pc);

	}

	MutatePopulation(newPopulation, n, pm);

	return newPopulation;

}

void Crossover(chromosome* parentA, chromosome* parentB, chromosome* newPopulation, float pc){

	int crosspoint = rand() % 27;

	if (rand() % 100 > (100 * pc)){
		crosspoint = 27;
	}

	for (int i = 0; i < crosspoint; i++){
		(newPopulation + 0)->genes[i] = parentA->genes[i];

		(newPopulation + 1)->genes[i] = parentB->genes[i];
	}
	for (int i = crosspoint; i < 27; i++){
		(newPopulation + 0)->genes[i] = parentB->genes[i];

		(newPopulation + 1)->genes[i] = parentA->genes[i];
	}
}

void MutatePopulation(chromosome* newPopulation, int n, float pm){

	for (int i = 0; i < n; i++){
		for (int k = 0; k < 27; k++){
			if (rand() % 100 < (100 * pm)){
				//new gene
				(newPopulation + i)->genes[k].period = rand() % 14;
				(newPopulation + i)->genes[k].room = rand() % 9;
			}
		}
	}

}