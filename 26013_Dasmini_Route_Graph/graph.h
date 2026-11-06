#ifndef GRAPH_H
#define GRAPH_H

typedef struct route
{
    char destination[50];
    float distance;
    struct route *next;
    int isblocked;
} Route;
typedef struct city
{
    char name[50];
    struct city *link;   
    Route *head;
    float dist;          // best known distance from source
    int visited;
    struct city *prev;   // city we came from
} City;

extern City *city_head;
extern unsigned int city_count;

void init_route();
City *check_city_existance(char cityName[]);
Route *check_route_existance(City *city_ptr, char destination[]);
void create_route(City *src_city_ptr,char destination[], float distance);
int add_city(char cityName[]);
int add_route(char source[], char destination[], float distance);
void display_roads();
int remove_city(char cityName[]);
void clear_routes(City *city);
void delete_route_in_dest(char cityName[], City *city);
int remove_route(char source[], char destination[]);
int shortest_path(char source[], char dest[]);
City *get_min_city();
void print_path(City *city);

#endif