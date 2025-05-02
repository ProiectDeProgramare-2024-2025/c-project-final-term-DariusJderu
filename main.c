#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#define N 100

//********************************************

#define COLOR_RED     (FOREGROUND_RED | FOREGROUND_INTENSITY)
#define COLOR_GREEN   (FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define COLOR_BLUE    (FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define COLOR_YELLOW  (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define COLOR_MAGENTA (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define COLOR_RESET   (-1)

WORD g_defaultAttributes = 0;
HANDLE g_hConsole = NULL;

void initConsole()
{
    g_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(g_hConsole, &info);
    g_defaultAttributes = info.wAttributes;
}

void setColor(int color)
{
    if (color == COLOR_RESET)
        SetConsoleTextAttribute(g_hConsole, g_defaultAttributes);

    else
        SetConsoleTextAttribute(g_hConsole, (WORD)color);
}

//********************************************

struct activitate
{
    char data[N];
    double durata, distanta;

}activitati[N];

int este_activitate_sportiva_existenta(char* activitate)
{
    if(!(strcmp(activitate,"Inot")==0 || strcmp(activitate,"inot")==0 || strcmp(activitate,"Alergat")==0 || strcmp(activitate,"alergat")==0 || strcmp(activitate,"Walk")==0 || strcmp(activitate,"walk")==0))
        return 0;

    return 1;
}

int este_numar_float_valid(char* numar)
{
    int nr_puncte=0;

    for(int i=0; numar[i]; ++i)
    {
        if(numar[i]=='.')
            nr_puncte++;

        else if(!isdigit(numar[i]))
            return 0;
    }

    if(nr_puncte>1 || numar[0]=='.')
        return 0;

    return 1;
}

int este_data_valida(char* data)
{
    if(strlen(data)!=(unsigned)10)
        return 0;

    char separator;
    int cnt=0;

    if(!isdigit(data[cnt++]) || !isdigit(data[cnt++]))
        return 0;

    if(data[cnt]!='/' && data[cnt]!='.')
        return 0;
    separator = data[cnt++];

    if(!isdigit(data[cnt++]) || !isdigit(data[cnt++]))
        return 0;

    if(data[cnt++]!=separator)
        return 0;

    if(!isdigit(data[cnt++]) || !isdigit(data[cnt++]) || !isdigit(data[cnt++]) || !isdigit(data[cnt]))
        return 0;

    return 1;
}

int validare_date_utilizator_adaugare_activitate(char* argv[], int argc)
{
    setColor(COLOR_RED);

    if(argc != 6)
    {
        printf("\nSintaxa invalida! Corect: <nume_activitate> <durata_activitate (numar real)>\n<km_parcursi (numar real)> <data_desfasurarii (zz.ll.aaaa)>\nAveti grija sa nu puneti spatii inutile!\n");
        return 0;
    }

    if(!este_activitate_sportiva_existenta(argv[2]))
    {
        printf("\nActivitatea sportiva pe care ati introdus-o nu este una disponibila pentru moment.\nCa sa vedeti ce activitati sunt disponibile, introduceti comanda ");
        setColor(COLOR_BLUE);
        printf("<view_activities>");
        setColor(COLOR_RESET);
        printf(".\n\n");
        return 0;
    }

    setColor(COLOR_RED);

    if(!este_numar_float_valid(argv[3]))
    {
        printf("\nSintaxa invalida! Durata introdusa nu este un numar real (exemplu: 12.5).\nAveti grija sa nu folositi numere negative si sa nu folositi virgula sau sa puneti mai multe puncte!\n");
        return 0;
    }

    if(!este_numar_float_valid(argv[4]))
    {
        printf("\nSintaxa invalida! Durata introdusa nu este un numar real (exemplu: 12.5).\nAveti grija sa nu folositi numere negative si sa nu folositi virgula sau sa puneti mai multe puncte!\n");
        return 0;
    }

    if(!este_data_valida(argv[5]))
    {
        printf("\nSintaxa invalida! Data introdusa nu este una valida (exemplu: 01.02.2025).\nAveti grija sa nu folositi caractere inutile si nu uitati sa delimitati zilele, lunile si anii!\n");
        return 0;
    }

    return 1;
}

void show_help()
{
    printf("\nComenzi disponibile:\n\n");

    setColor(COLOR_RED);
    printf("add_activity");
    setColor(COLOR_MAGENTA);
    printf(" <nume_activitate> <durata_activitate (numar real)> <km_parcursi (numar real)>\n<data_desfasurarii (zz.ll.aaaa)> ");
    setColor(COLOR_RESET);
    printf("- Adauga o activitate sportiva\n\n");

    setColor(COLOR_GREEN);
    printf("view_stats ");
    setColor(COLOR_RESET);
    printf("- Afiseaza statisticile\n\n");

    setColor(COLOR_BLUE);
    printf("view_activities ");
    setColor(COLOR_RESET);
    printf("- Afiseaza toate activitatile disponibile\n\n");

    setColor(COLOR_YELLOW);
    printf("help ");
    setColor(COLOR_RESET);
    printf("- Afiseaza lista de comenzi disponibile\n");
}

int comanda_existenta(const char* comanda)
{
    if(strcmp(comanda, "add_activity") == 0)
        return 1;

    if(strcmp(comanda,"view_stats")==0)
        return 1;

    if(strcmp(comanda,"view_activities")==0)
        return 1;

    if(strcmp(comanda,"help")==0)
        return 1;

    return 0;
}

double calculare_durata_medie(int nr)
{
    double s=0;
    for(int i=0; i<nr; ++i)
        s+=activitati[i].durata;

    return (double)s/nr;
}

double calculare_total_km(int nr)
{
    double s=0;
    for(int i=0; i<nr; ++i)
        s+=activitati[i].distanta;

    return s;
}

void printare_numar_string(char* linie)
{
    char* p=strtok(linie," \n");

    while(p)
    {
        if(isdigit(p[0]))
            printf("%s\n", p);

        p=strtok(NULL," \n");
    }
}

void afisare_informatii_fisiere_stats(FILE* fisier_stats)
{
    char linie[N];

    fgets(linie, N, fisier_stats);
    setColor(COLOR_RED);
    printf("%s", linie);
    setColor(COLOR_RESET);

    fgets(linie, N, fisier_stats);
    setColor(COLOR_GREEN);
    printf("%s","Durata medie ");
    setColor(COLOR_YELLOW);
    printf("%s", "-> ");
    setColor(COLOR_BLUE);
    printare_numar_string(linie);


    fgets(linie, N, fisier_stats);
    setColor(COLOR_GREEN);
    printf("%s","Distanta totala ");
    setColor(COLOR_YELLOW);
    printf("%s", "-> ");
    setColor(COLOR_BLUE);
    printare_numar_string(linie);

    printf("\n");
    fclose(fisier_stats);
}

int citire_fisier(FILE* fisier)
{
    int nr=0;
    fscanf(fisier, "%d", &nr);

    if(nr==0)
        return 0;

    for(int i=0; i<nr; ++i)
    {
        fscanf(fisier, "%lf", &activitati[i].durata);
        fscanf(fisier, "%lf", &activitati[i].distanta);
        fscanf(fisier, "%s", activitati[i].data);
    }

    fclose(fisier);

    return nr;
}

void adauga_activitate(int* nr_activitati, const char *argv[], int argc, FILE* fisier)
{
    if(nr_activitati==0)
    {
        fprintf(fisier, "%d\n", 1);
        fprintf(fisier, "%.2f %.2f %s\n", atof(argv[3]), atof(argv[4]), argv[5]);
        return;
    }

    (*nr_activitati)++;

    activitati[(*nr_activitati)-1].durata = atof(argv[3]);
    activitati[(*nr_activitati)-1].distanta = atof(argv[4]);
    strcpy(activitati[(*nr_activitati)-1].data, argv[5]);

    fprintf(fisier, "%d\n", *nr_activitati);

    for(int i=0; i<(*nr_activitati); ++i)
    {
        fprintf(fisier,"%.2f ",activitati[i].durata);
        fprintf(fisier,"%.2f ",activitati[i].distanta);
        fprintf(fisier,"%s\n", activitati[i].data);
    }

    fclose(fisier);
}

void adaugare_inot(const char* argv[], const int argc)
{
    FILE* fisier_inot=fopen("inot.txt","r");
    int nr_activitati=citire_fisier(fisier_inot);

    fisier_inot=fopen("inot.txt","w");
    adauga_activitate(&nr_activitati,argv,argc,fisier_inot);

    double durata_medie=calculare_durata_medie(nr_activitati);
    double total_km=calculare_total_km(nr_activitati);

    FILE* fisier_inot_stats=fopen("inot_stats.txt","w");

    fprintf(fisier_inot_stats,"Inot\nDurata medie -> %.2f\nDistanta totala -> %.2f\n",durata_medie,total_km);
    fclose(fisier_inot_stats);
}

void adaugare_alergat(const char* argv[], const int argc)
{
    FILE* fisier_alergat=fopen("alergat.txt","r");
    int nr_activitati=citire_fisier(fisier_alergat);

    fisier_alergat=fopen("alergat.txt","w");
    adauga_activitate(&nr_activitati,argv,argc,fisier_alergat);

    double durata_medie=calculare_durata_medie(nr_activitati);
    double total_km=calculare_total_km(nr_activitati);

    FILE* fisier_alergat_stats=fopen("alergat_stats.txt","w");

    fprintf(fisier_alergat_stats,"Alergat\nDurata medie -> %.2f\nDistanta totala -> %.2f\n",durata_medie,total_km);
    fclose(fisier_alergat_stats);
}

void adaugare_walk(const char* argv[], const int argc)
{
    FILE* fisier_walk=fopen("walk.txt","r");
    int nr_activitati=citire_fisier(fisier_walk);

    fisier_walk=fopen("walk.txt","w");
    adauga_activitate(&nr_activitati,argv,argc,fisier_walk);

    double durata_medie=calculare_durata_medie(nr_activitati);
    double total_km=calculare_total_km(nr_activitati);

    FILE* fisier_walk_stats=fopen("walk_stats.txt","w");

    fprintf(fisier_walk_stats,"Walk\nDurata medie -> %.2f\nDistanta totala -> %.2f\n",durata_medie,total_km);
    fclose(fisier_walk_stats);
}

void adauga_activitate_sport(const char* argv[], const int argc)
{
    if(strcmp(argv[2],"Inot")==0 || strcmp(argv[2],"inot")==0)
    {
        adaugare_inot(argv,argc);
        return ;
    }

    if(strcmp(argv[2],"Alergat")==0 || strcmp(argv[2],"alergat")==0)
    {
        adaugare_alergat(argv,argc);
        return ;
    }

    if(strcmp(argv[2],"Walk")==0 || strcmp(argv[2],"walk")==0)
        adaugare_walk(argv,argc);
}

void executare_comenzi(const char* command, const char *argv[], const int argc)
{
    if(!comanda_existenta(command))
    {
        printf("Optiune lipsa! Pentru a vedea o lista cu toate comenzile accesibile folositi optiunea help!");
        return;
    }

    if(strcmp(command, "view_activities") == 0)
    {
        printf("\nLista de activitati disponibile: inot, alergat, walk\n");
        return ;
    }

    if(strcmp(command,"help") == 0)
    {
        show_help();
        return ;
    }

    if(strcmp(command, "add_activity") == 0)
    {
        int date_valide=validare_date_utilizator_adaugare_activitate(argv, argc);

        if(date_valide==0)
            return ;

        adauga_activitate_sport(argv, argc);

        return ;
    }

    if(strcmp(command, "view_stats") == 0)
    {
        FILE* fisier_stats=fopen("inot_stats.txt","r");
        afisare_informatii_fisiere_stats(fisier_stats);

        fisier_stats=fopen("alergat_stats.txt","r");
        afisare_informatii_fisiere_stats(fisier_stats);

        fisier_stats=fopen("walk_stats.txt","r");
        afisare_informatii_fisiere_stats(fisier_stats);
    }
}

int main(int argc, char *argv[])
{
    initConsole();

    if(argc==1)
    {
        show_help();
        return 0;
    }

    const char *command = argv[1];

    executare_comenzi(command,argv,argc);

    return 0;
}