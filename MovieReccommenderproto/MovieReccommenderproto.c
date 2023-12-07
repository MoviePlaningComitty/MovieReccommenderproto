#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAENGDE 7662
#define WORD_LENGTH 200
#define GENRE_MAX_AMOUNT 100

typedef struct
{
    char nameOfFilm[WORD_LENGTH];
    char ratings[WORD_LENGTH];
    char genre[GENRE_MAX_AMOUNT][WORD_LENGTH];
    int year;
    char releaseDate[WORD_LENGTH];
    double score;
    int IMDB_user_votes;
    char director[WORD_LENGTH];
    char writer[WORD_LENGTH];
    char star_actor[WORD_LENGTH];
    char country[WORD_LENGTH];
    int budget;
    int gross;
    char company[WORD_LENGTH];
    int runtime;
    int id;
    double userScore;
}Movies;

//Movie Stats
typedef struct
{
    int movieId;
    double movieScore;
    char movieName[WORD_LENGTH];

} movieStats;

//Genre Stats
typedef struct
{
    int genreId;
    double genrePoint;
    char genreName[WORD_LENGTH];
} genreStats;

//Actor Stats
typedef struct
{
    int actorId;
    double actorPoint;
    char actorName[WORD_LENGTH];
} actorStats;

//Director Stats
typedef struct
{
    int directorId;
    double directorPoint;
    char directorName[WORD_LENGTH];
} directorStats;

//Main Struct
typedef struct {
    movieStats statsMovies[MAENGDE];
    genreStats statsGenre[GENRE_MAX_AMOUNT];
    actorStats statsActor[MAENGDE];
    directorStats statsDirector[MAENGDE];
}userStats;

void printCommandLines(void);

int runCommand(char* command, Movies* movieArray, userStats* stats);

int checkForCommand(Movies* movieArray, userStats* stats);

void fraFilTilMovie(Movies* movieArray);

Movies scanFunktion(FILE* f);

void printFunktion(Movies ThisMoviemovieArray);

void cold_start(Movies* movieArray, userStats* stats);

void ask_user(Movies* movieArray, userStats* stats);

void read_stats(userStats* statistics);

double ScoreGetActor(Movies movieArray, userStats* stats);


int recommendRandom(Movies movieArray[]);

void delete_all(void);

void concatenateStrings(char strings[][WORD_LENGTH], int size);

void savefiles(userStats* statForFilm);

void printMovieName(Movies ThisMovie);

void getMovie(Movies* movieArray, int id);

double ScoreGetGenres(Movies movieArray, userStats* stats);

double ScoreGetDirector(Movies movieArray, userStats* stats);

int comparator(const void* p1, const void* p2);

int haveseen(Movies movieArray, userStats* stats);

void RecommendMovie(Movies* movieArray, userStats* stats);

void rateMovie(int movieID, userStats* stats, Movies* movieArray);

void movieRatedAlgorithm(movieStats movieArray[], Movies theMovie, int movieRating);

void genreRatedAlgorithm(genreStats genreArray[], Movies theMovie, int movieRating);

void actorRatedAlgorithm(actorStats actorArray[], Movies theMovie, int movieRating);

void directorRatedAlgorithm(directorStats directorArray[], Movies theMovie, int movieRating);

int main(void)
{
    srand(time(NULL));
    Movies* movieArray = (Movies*)malloc(sizeof(Movies) * MAENGDE);
    if (movieArray == NULL)
    {
        printf("Filen blev ikke aabnet\n");
        exit(EXIT_FAILURE);

    }

    fraFilTilMovie(movieArray);

    userStats* stats = (userStats*)malloc(sizeof(userStats));
    cold_start(movieArray, stats);
    int i = 0;
    printCommandLines();

    do {
        i = checkForCommand(movieArray, stats);
    } while (i);
    free(movieArray);
    free(stats);
    return 0;
}

void getMovie(Movies* movieArray, int id)
{
    for (int i = 0; i < MAENGDE; i++)
    {
        if (movieArray[i].id == id) 
        {
            printFunktion(movieArray[i]);
            break;
        }
    }
}

void printCommandLines(void)
{
    printf("\nWrite \"Exit\" or \"0\" to exit the program\n");
    printf("Write \"Commands\" or \"1\" for the list of commands\n");
    printf("Write \"RateMovieId\" or \"2\" followed by the Id for the movie you want to review\n");
    printf("Write \"RandomMovie\"  or \"3\" This will give you 10 random movies\n");
    printf("Write \"DeleteMyInfo\"  or \"4\" Delets all information we have on you\n");
    printf("Write \"GetMovie\"  or \"5\" Followed by the movie Id to get all info on mthe movie\n");
    printf("Write \"RecommendMovie\"  or \"6\" Followed by the movie Id to get all info on mthe movie\n");
}

int runCommand(char* command, Movies* movieArray, userStats* stats)
{
    if (strcmp(command, "Exit") == 0 || strcmp(command, "exit") == 0 || strcmp(command, "0") == 0)
    {
        return 0;
    }
    else if (strcmp(command, "Commands") == 0 || strcmp(command, "commands") == 0 || strcmp(command, "1") == 0)
    {
        printCommandLines();
    }
    else if (strcmp(command, "RateMovieId") == 0 || strcmp(command, "ratemovieid") == 0 || strcmp(command, "2") == 0)
    {
        int i;
        scanf(" %i", &i);
        rateMovie(i, stats, movieArray);
    }
    else if (strcmp(command, "RandomMovie") == 0 || strcmp(command, "randommovie") == 0 || strcmp(command, "3") == 0)
    {
        recommendRandom(movieArray);
    }
    else if(strcmp(command, "DeleteMyInfo") == 0 || strcmp(command, "deletemyinfo") == 0 || strcmp(command, "4") == 0)
    {
        delete_all();
        cold_start(movieArray, stats);
    }
    else if (strcmp(command, "GetMovie") == 0 || strcmp(command, "getmovie") == 0 || strcmp(command, "5") == 0)
    {
        int i;
        scanf(" %i", &i);
        getMovie(movieArray, i);
    }
    else if (strcmp(command, "RecommendMovie") == 0 || strcmp(command, "recommendmovie") == 0 || strcmp(command, "6") == 0)
    {
        RecommendMovie(movieArray,stats);
    }
    else
    {
        printf("Type \"Commands\" for the list of commands\n");
    }

    return 1;
}

int checkForCommand(Movies* movieArray, userStats* stats)
{
    char string[50];
    int i;
    printf("Write command here: ");
    scanf(" %s", string);
    i = runCommand(string, movieArray, stats);
    return i;
}

void fraFilTilMovie(Movies* movieArray)
{
    printf("Loading movies...\n");
    FILE* f = fopen("movie1.txt", "r");
    if (f == NULL)
    {
        printf("Filen blev ikke aabnet\n");
        exit(EXIT_FAILURE);
    }
    char buffer[5000];

    fscanf(f," %[^\n]\n",buffer);
    //printf("%s \n", buffer);
    for (int i = 0; i < MAENGDE; i++) //her går programmet igennem hele arrayet og kører scanFunktion indtil den har nået alle filmene
    {
        movieArray[i] = scanFunktion(f);
        movieArray[i].id = i +1;
    }
    fclose(f);
    printf("Loadig complete.\n");
}

Movies scanFunktion(FILE* f) //denne funktion gemmer enkelte film
{
    char tek[1000] = "";
    Movies ThisMovie;
    fscanf(f, "%[^;];", ThisMovie.nameOfFilm);
    fscanf(f, "%[^;];(", ThisMovie.ratings);
    char ch;
    int i = 0;
    ThisMovie.genre;
    do
    {
        
        fscanf(f, " %[^,^)]%c", ThisMovie.genre[i], &ch);
        i++;
    } while (ch != ')');
    int z = i;
    fscanf(f, ";");
    for (i; i < GENRE_MAX_AMOUNT; i++)
    {
        strcpy(ThisMovie.genre[i], "");
    }
    concatenateStrings(ThisMovie.genre, z);
    //cobine function
    fscanf(f, "%[^;];", tek);

    ThisMovie.year = atoi(tek);

    fscanf(f, " %[^;];", ThisMovie.releaseDate);
    fscanf(f, " %lf;", &ThisMovie.score);

    fscanf(f, " %[^;];", tek);
    ThisMovie.IMDB_user_votes = atoi(tek);

    fscanf(f, " %[^;];", ThisMovie.director);
    fscanf(f, " %[^;];", ThisMovie.writer);
    fscanf(f, " %[^;];", ThisMovie.star_actor);
    fscanf(f, " %[^;];", ThisMovie.country);
    fscanf(f, " %[^;];", tek);
    ThisMovie.budget = atoi(tek);

    fscanf(f, " %[^;];", tek);
    ThisMovie.gross = atoi(tek);

    fscanf(f, " %[^;];", ThisMovie.company);
    fscanf(f, " %[^\n]\n", tek);
    ThisMovie.runtime = atoi(tek);

    //printFunktion(ThisMovie); //denne linje er til at finde fejl i koden
    return ThisMovie;
}

void printFunktion(Movies ThisMovie) //printer alle filmene hvis man skal bruge det
{
    char* nameOfFilm = ThisMovie.nameOfFilm;
    char* rating = ThisMovie.ratings;
    int year = ThisMovie.year;
    char* release = ThisMovie.releaseDate;
    double score = ThisMovie.score;
    int IMDB = ThisMovie.IMDB_user_votes;
    char* director = ThisMovie.director;
    char* writer = ThisMovie.writer;
    char* actor = ThisMovie.star_actor;
    char* country = ThisMovie.country;
    int budget = ThisMovie.budget;
    int gross = ThisMovie.gross;
    char* company = ThisMovie.company;
    int runtime = ThisMovie.runtime;

    printf("Movie Id: %i;", ThisMovie.id);
    printf("Name: %s;", nameOfFilm);
    printf("Rated: %s; ", rating);
    printf("Genre: (");
    printf("%s", ThisMovie.genre[0]);
    for (int i = 1; i < GENRE_MAX_AMOUNT; i++)
    {
        if (strcmp(ThisMovie.genre[i], "") == 0)
            break;
        printf(", %s", ThisMovie.genre[i]);
    }
    printf("); ");
    printf("Release year: %d; ", year);
    printf("Release date: %s; ", release);
    printf("Rating: %.1lf; ", score);
    printf("Votes: %d; ", IMDB);
    printf("Director: %s; ", director);
    printf("Writer: %s; ", writer);
    printf("Star actor: %s; ", actor);
    printf("Country: %s; ", country);
    printf("Budget: %d; ", budget);
    printf("Gross earnings: %d; ", gross);
    printf("Production Company: %s; ", company);
    printf("Runtime: %d;\n", runtime);
}

void printMovieName(Movies ThisMovie)
{
    printf("Movie Id: %i   ", ThisMovie.id);
    printf("Name: %s\n", ThisMovie.nameOfFilm);
}

void cold_start(Movies* movieArray, userStats* stats) {
    //pointer file placeholder
    FILE* file;
    
    if ((file = fopen("persondata.txt", "r")) != NULL) {
        fclose(file);
        read_stats(stats);
    }
    else {
        ask_user(movieArray,stats);
    }
}

int compareFunc(const void* a, const void* b) {
    return (*(int*)b - *(int*)a);
}

void ask_user(Movies* movieArray, userStats* stats) {
    char* genres[21] = { "Nothing", "Action", "Adventure", "Animation", "Biography",
                        "Comedy", "Crime", "Drama", "Family", "Fantasy",
                        "History", "Horror", "Music", "Musical", "Mystery",
                        "Romance", "Sci-Fi", "Sport", "Thriller", "War", "Western" };
    char x;
    int genre[3];

    do {
        printf("Start with random recommendations? (Y/N): ");
        scanf(" %c", &x);

        if (x == 'Y') {

            char s;
            do
            {
                recommendRandom(movieArray);
                printf("\n\n Do you want to review any of these movies? (Y/N): ");

                scanf(" %c", &s);

                if (s == 'y') 
                {
                    int id;
                    printf("What do you want you want to review (id): ");
                    scanf(" %i", &id);
                    rateMovie(id, stats, movieArray);
                }
                    

            } while (s != 'Y');
            


        }
        else if (x == 'N') {
            printf("Choose up to 3 genres:\n");
            for (int i = 0; i < 21; i++) {
                printf("%d. %s\n", i, genres[i]);
            }

            do {
                scanf(" %d %d %d", genre, genre + 1, genre + 2);
                int z = 0;
                for (int i = 0; i < 3; i++)
                {
                    if (genre[i] != 0)
                        z++;
                }

                
                qsort(genre, 3, sizeof(int), compareFunc);

                if (!z)
                    x = 'r';

                if (!(genre[0] < 0 || genre[0] > 21 || genre[1] < 0 || genre[1] > 21 || genre[2] < 0 || genre[2] > 21)) {
                    for (int i = 0; i < z; i++) {
                        stats[0].statsGenre[i].genreId = i + 1;
                        stats[0].statsGenre[i].genrePoint = 8;
                        strcpy(stats[0].statsGenre[i].genreName, genres[genre[i]]);
                    }
                    savefiles(stats);
                }

            } while (genre[0] < 0 || genre[0] > 21 || genre[1] < 0 || genre[1] > 21 || genre[2] < 0 || genre[2] > 21);

        }
        else {
            printf("INVALID INPUT\n");
        }
    } while (x != 'Y' && x != 'N');
    
}

void read_stats(userStats* statistics) {
    FILE* f = fopen("persondata.txt", "r");
    if (f == NULL)
    {
        printf("Filen blev ikke aabnet\n");
        exit(EXIT_FAILURE);
    }
    char ch = 'c';
    char tek[1000] = "";
    char tek2[1000] = "";
    int i = 0;
    
    while (fscanf(f, "%1000[^;^\n];%1000[^;^\n];%1000[^;^\n];", tek, tek2, statistics[0].statsMovies[i].movieName) == 3)
    {
        statistics[0].statsMovies[i].movieId = atoi(tek);
        statistics[0].statsMovies[i].movieScore = atof(tek2);
        i++;
    }
    i = 0;
    fscanf(f, " \n");
    while (fscanf(f, "%1000[^;^\n];%1000[^;^\n];%1000[^;^\n];", tek, tek2, statistics[0].statsGenre[i].genreName) == 3)
    {
        statistics[0].statsGenre[i].genreId = atoi(tek);
        statistics[0].statsGenre[i].genrePoint = atof(tek2);
        i++;
    }
    fscanf(f, " \n");
    i = 0;
    while (fscanf(f, "%1000[^;^\n];%1000[^;^\n];%1000[^;^\n];", tek, tek2, statistics[0].statsActor[i].actorName) == 3)
    {
        statistics[0].statsActor[i].actorId = atoi(tek);
        statistics[0].statsActor[i].actorPoint = atof(tek2);
        i++;
    }
    fscanf(f, " \n");
    i = 0;
    while (fscanf(f, "%1000[^;^\n];%1000[^;^\n];%1000[^;^\n];", tek, tek2, statistics[0].statsDirector[i].directorName) == 3)
    {
        statistics[0].statsActor[i].actorId = atoi(tek);
        statistics[0].statsActor[i].actorPoint = atof(tek2);
        i++;
    }
    fclose(f);
}

int recommendRandom(Movies movieArray[]) {
    for (int i = 0; i < 10; i++)
    {
        int y = rand() % MAENGDE;
        printMovieName(movieArray[y]);
    }
    return 0;
}

void delete_all(void) 
{ //prompter brugeren til at slette ens persondata
    char answer[4];

    do {
        printf("Do you want to delete this file (yes or no)?\n");
        scanf(" %s", answer);

        if (strcmp(answer, "yes") == 0) {
            remove("persondata.txt");
            printf("File deleted successfully\n");
        }
        else if (strcmp(answer, "no") == 0) {
            printf("OK\n");
        }
        else {
            printf("Not valid input\n");
        }
    } while (strcmp(answer, "yes") != 0 && strcmp(answer, "no") != 0);
}

void savefiles(userStats* statForFilm) //tager imod en  userStatsstruct
{
    FILE* f2 = fopen("persondata.txt", "w"); //laver en ny fil og printer  userStatsstructsene i filen.
    for (int i = 0; i < MAENGDE; i++) //print for film
    {
        if (statForFilm[0].statsMovies[i].movieId != 0) 
        {
            fprintf(f2, "%d;%.2lf;%s;", statForFilm[0].statsMovies[i].movieId, statForFilm[0].statsMovies[i].movieScore, statForFilm[0].statsMovies[i].movieName);
            
        }
        
    }
    fprintf(f2, "\n");
    for (int i = 0; i < GENRE_MAX_AMOUNT; i++) //print for genre
    {
        if (statForFilm[0].statsGenre[i].genreId != 0) 
        {
            fprintf(f2, "%d;%.2lf;%s;", statForFilm[0].statsGenre[i].genreId, statForFilm[0].statsGenre[i].genrePoint, statForFilm[0].statsGenre[i].genreName);
            
        }
        
    }
    fprintf(f2, "\n");
    for (int i = 0; i < MAENGDE; i++) //print for skuespillere
    {
        if(statForFilm[0].statsActor[i].actorId != 0)
        {
            fprintf(f2, "%d;%.2lf;%s;", statForFilm[0].statsActor[i].actorId, statForFilm[0].statsActor[i].actorPoint, statForFilm[0].statsActor[i].actorName);
            
        }
    }
    fprintf(f2, "\n");
    for (int i = 0; i < MAENGDE; i++) // print til director
    {
        if(statForFilm[0].statsDirector[i].directorId != 0)
        {
            fprintf(f2, "%d;%.2lf;%s;", statForFilm[0].statsDirector[i].directorId, statForFilm[0].statsDirector[i].directorPoint, statForFilm[0].statsDirector[i].directorName);
            
        }
    }
    fprintf(f2, "\n");
    fclose(f2);
}

void concatenateStrings(char strings[][WORD_LENGTH], int size) {
    int originalSize = size; // Save the original size

    for (int i = 0; i < originalSize; i++) {
        for (int j = i + 1; j < originalSize; j++) {
            if (strings[i][0] == '\0' || strings[j][0] == '\0') {
                continue; // Skip empty strings
            }

            char concatenated[WORD_LENGTH]; // Adjust the size as needed
            strcpy(concatenated, strings[i]);
            strcat(concatenated, strings[j]);

            // Update the original array in place
            strcpy(strings[size], concatenated);
            size++;
        }
    }

    // Concatenate all strings of originalSize
    if (originalSize >= 3) {
        char concatenatedAll[WORD_LENGTH];
        concatenatedAll[0] = '\0';

        for (int i = 0; i < originalSize; i++) {
            strcat(concatenatedAll, strings[i]);
        }

        strcpy(strings[size], concatenatedAll);
        size++;
    }
}

double ScoreGetGenres(Movies movieArray, userStats* stats) 
{
    double x = 0, y = 0;
    for (int i = 0; i < GENRE_MAX_AMOUNT; i++)
    {
        if (movieArray.genre[i][0] == '\0') {
            continue; // Skip empty strings
        }
        for (int t = 0; t < GENRE_MAX_AMOUNT; t++)
        {
            if (stats[0].statsGenre[t].genreName[0] == '\0') {
                continue; // Skip empty strings
            }
            if (strcmp(movieArray.genre[i], stats[0].statsGenre[t].genreName) == 0) 
            {
                x += stats[0].statsGenre[t].genrePoint;
                y++;
                break;
            }
        }
    }
    if (x == 0)
    {
        x++;
        y++;
    }
    return x / y;
}

double ScoreGetActor(Movies movieArray, userStats* stats)
{
    double x = 0;
    for (int i = 0; i < MAENGDE; i++)
    {
        if (stats[0].statsActor[i].actorName[0] == '\n') {
            continue; // Skip empty strings
        }
        if (strcmp(movieArray.star_actor, stats[0].statsActor[i].actorName) == 0)
        {
            x = stats[0].statsActor[i].actorPoint;
            break;
        }
    }
    if (x == 0)
        x++;
    return x;
}

double ScoreGetDirector(Movies movieArray, userStats* stats)
{
    double x = 0;
    for (int i = 0; i < MAENGDE; i++)
    {
        if (stats[0].statsDirector[i].directorName[0] == '\n') {
            continue; // Skip empty strings
        }
        if (strcmp(movieArray.star_actor, stats[0].statsDirector[i].directorName) == 0)
        {
            x = stats[0].statsDirector[i].directorPoint;
            break;
        }
    }
    if (x == 0)
        x++;
    return x;
}

int haveseen(Movies movieArray, userStats* stats)
{
    int x = 0;
    for (int i = 0; i < MAENGDE; i++)
    {
        if (stats[0].statsMovies[i].movieName[0] == '\n') {
            continue; // Skip empty strings
        }
        if (strcmp(movieArray.nameOfFilm, stats[0].statsDirector[i].directorName) == 0)
        {
            x ++;
            break;
        }
    }
    return x;
}

void RecommendMovie(Movies* movieArray, userStats* stats)
{
    double genrevegt = 3, actorvegt = 2, directorvegt = 1;
    for (int i = 0; i < MAENGDE; i++)
    {
        if (haveseen(movieArray[i], stats))
        {
            movieArray[i].userScore = 0;
            continue;
        }
         
        double genreScore = ScoreGetGenres(movieArray[i], stats);
        double actorScore = ScoreGetActor(movieArray[i], stats);
        double directorScore = ScoreGetDirector(movieArray[i], stats);
        movieArray[i].userScore = (genrevegt * genreScore + actorvegt * actorScore + directorvegt * directorScore) / (genrevegt + actorvegt + directorvegt);
    }

    qsort(movieArray, MAENGDE, sizeof(Movies), comparator);

    // give us 3 movies within top 200 
    printf("It's likely that you will like these movies\n");
    for (int i = 0; i < 3; i++)
    {
        int y = rand() % 200;
        printf("nr. %i; ", y);
        printMovieName(movieArray[y]);
    }

    // gives us 3 movies between 201-1000
    printf("their is a high probability that you will like these movies\n");
    for (int i = 0; i < 3; i++)
    {
        int y = rand() % 800 + 201;
        printf("nr. %i; ", y);
        printMovieName(movieArray[y]);
    }

    //gives us 2 movies between 1001-2000
    printf("their is a probability that you will like these movies\n");
    for (int i = 0; i < 2; i++)
    {
        int y = rand() % 1000 + 1001;
        printf("nr. %i; ", y);
        printMovieName(movieArray[y]);
    }


    //gives us 2 movies between 1001-6000
    printf("You may like these movies\n");
    for (int i = 0; i < 2; i++)
    {
        int y = rand() % 5000 + 1001;
        printf("nr. %i; ", y);
        printMovieName(movieArray[y]);
    }
}

int comparator(const void* p1, const void* p2)
{
    double movie1 = ((Movies*)p1)->userScore;
    double movie2 = ((Movies*)p2)->userScore;
    return movie2 - movie1;
}

void movieRatedAlgorithm(movieStats movieArray[], Movies theMovie, int movieRating)
{
    int pladsPaaArray = 0, compare = 0;
    double weight;
    while (movieArray[pladsPaaArray].movieName != (NULL))//måske ændrer NULL
    {
        pladsPaaArray++; //denne whileløkke finder den næste tomme plads på array'et
    }

    for (int r = pladsPaaArray; r >= 0; r--)
    {
        compare = strcmp(theMovie.nameOfFilm, movieArray[r].movieName);
        if (compare == 0)//hvis den finder en plads paa array'et hvor den allerede eksistere skal den ændrer point
        {
            movieArray[pladsPaaArray].movieScore = movieRating;
            break;
        }
    }

    if (compare != 0) //hvis ikke den kan finde noget om filmen i array, tilføj disse værdier
    {
        strcpy(movieArray[pladsPaaArray].movieName, theMovie.nameOfFilm);
        movieArray[pladsPaaArray].movieId = pladsPaaArray + 1;
        movieArray[pladsPaaArray].movieScore = movieRating;
    }
}

void genreRatedAlgorithm(genreStats genreArray[], Movies theMovie, int movieRating)
{
    int pladsPaaArray = 0, compare = 0;
    double weight;
    while (genreArray[pladsPaaArray].genreName != (NULL))//måske ændrer NULL
    {
        pladsPaaArray++; //denne whileløkke finder den næste tomme plads på array'et
    }
    for (int i = 0; i < GENRE_MAX_AMOUNT; i++)
    {
        if (theMovie.genre[i][0] == '\n') {
            continue;
        }
        for (int r = pladsPaaArray; r >= 0; r--)
        {
            compare = strcmp(theMovie.genre[i], genreArray[r].genreName);
            if (compare == 0)//hvis den finder en plads paa array'et hvor den allerede eksistere skal den ændrer point
            {
                if (movieRating > genreArray[r].genrePoint) //if statement er "har vi gemt en mindre score end det de har givet filmene?""
                {
                    weight = ((movieRating - genreArray[r].genrePoint) / 10) + 1; //tallende her er vægtberegning valgt af mo
                    genreArray[r].genrePoint *= weight; //efter weight beregning skal det ganges med scoren der var før
                }
                else if (movieRating < genreArray[r].genrePoint) //er det vi har gemt mere end mindre end det de taster
                {
                    weight = ((genreArray[r].genrePoint - movieRating) / 10) + 1; //den er nedvurderet med /20 i stedet for /10 sådan at numrene ikke bliver ændret for meget
                    genreArray[r].genrePoint /= weight;
                }
                break;
            }
        }
        if (compare != 0) //hvis ikke den kan finde noget om genren i array, tilføj disse værdier
        {
            strcpy(genreArray[pladsPaaArray].genreName, theMovie.genre[i]);
            genreArray[pladsPaaArray].genreId = pladsPaaArray + 1;
            genreArray[pladsPaaArray].genrePoint = movieRating;
        }
    }
    

    
}

void actorRatedAlgorithm(actorStats actorArray[], Movies theMovie, int movieRating)
{
    int pladsPaaArray = 0, compare = 0;
    double weight;
    while (actorArray[pladsPaaArray].actorName != (NULL))//måske ændrer NULL
    {
        pladsPaaArray++; //denne whileløkke finder den næste tomme plads på array'et
    }

    for (int r = pladsPaaArray; r >= 0; r--)
    {
        compare = strcmp(theMovie.star_actor, actorArray[r].actorName);
        if (compare == 0)//hvis den finder en plads paa array'et hvor den allerede eksistere skal den ændrer point
        {
            if (movieRating > actorArray[r].actorPoint) //if statement er "har vi gemt en mindre score end det de har givet filmene?""
            {
                weight = ((movieRating - actorArray[r].actorPoint) / 10) + 1; //tallende her er vægtberegning valgt af mo
                actorArray[r].actorPoint *= weight; //efter weight beregning skal det ganges med scoren der var før
            }
            else if (movieRating < actorArray[r].actorPoint) //er det vi har gemt mere end mindre end det de taster
            {
                weight = ((actorArray[r].actorPoint - movieRating) / 10) + 1; //det samme bare nedvurdered fordi movierating er mindre end actorScore
                actorArray[r].actorPoint /= weight;
            }
            break;
        }
    }

    if (compare != 0) //hvis ikke den kan finde noget om star_actorn i array, tilføj disse værdier
    {
        strcpy(actorArray[pladsPaaArray].actorName, theMovie.star_actor);
        actorArray[pladsPaaArray].actorId = pladsPaaArray + 1;
        actorArray[pladsPaaArray].actorPoint = movieRating;
    }
}

void directorRatedAlgorithm(directorStats directorArray[], Movies theMovie, int movieRating)
{
    int pladsPaaArray = 0, compare = 0;
    double weight;
    while (directorArray[pladsPaaArray].directorName != (NULL))//måske ændrer NULL
    {
        pladsPaaArray++; //denne whileløkke finder den næste tomme plads på array'et
    }

    for (int r = pladsPaaArray; r >= 0; r--)
    {
        compare = strcmp(theMovie.director, directorArray[r].directorName);
        if (compare == 0)//hvis den finder en plads paa array'et hvor den allerede eksistere skal den ændrer point
        {
            if (movieRating > directorArray[r].directorPoint) //if statement er "har vi gemt en mindre score end det de har givet filmene?""
            {
                weight = ((movieRating - directorArray[r].directorPoint) / 10) + 1; //tallende her er vægtberegning valgt af mo
                directorArray[r].directorPoint *= weight; //efter weight beregning skal det ganges med scoren der var før
            }
            else if (movieRating < directorArray[r].directorPoint) //er det vi har gemt mere end mindre end det de taster
            {
                weight = ((directorArray[r].directorPoint - movieRating) / 10) + 1; //det samme bare nedvurdered fordi movierating er mindre end directorScore
                directorArray[r].directorPoint /= weight;
            }
            break;
        }
    }

    if (compare != 0) //hvis ikke den kan finde noget om star_actorn i array, tilføj disse værdier
    {
        strcpy(directorArray[pladsPaaArray].directorName, theMovie.director);
        directorArray[pladsPaaArray].directorId = pladsPaaArray + 1;
        directorArray[pladsPaaArray].directorPoint = movieRating;
    }
}

void rateMovie(int movieID, userStats* stats, Movies* movieArray) {
    int i;
    int userRating = 0;

    for (i = 0; i < MAENGDE; i++) {
        if (movieID == movieArray[i].id) {
            break;
        }
    }

    do {
        printf("Please rate the movie from 1-10: ");
        scanf(" %d", &userRating);
        if (userRating < 1 || userRating > 10) {
            printf("INVALID INPUT");
        }
    } while (userRating < 1 || userRating > 10);

    movieRatedAlgorithm(stats->statsMovies, movieArray[i], userRating);
    genreRatedAlgorithm(stats->statsGenre, movieArray[i], userRating);
    actorRatedAlgorithm(stats->statsActor, movieArray[i], userRating);
    directorRatedAlgorithm(stats->statsDirector, movieArray[i], userRating);
}