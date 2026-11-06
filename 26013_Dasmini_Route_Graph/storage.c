#include <stdio.h>
#include "graph.h"
#include "storage.h"

void save_routes()
{
    FILE *fptr = fopen("routes.txt", "w");
    if(fptr == NULL)
    {
        perror("fopen");
        return;
    }
    City *city_temp = city_head;
    fprintf(fptr, "%u\n",city_count);
    while(city_temp != NULL)
    {
        fprintf(fptr, "%s ",city_temp->name);
        Route *route_temp = city_temp->head;
        int route_count = 0;
        while(route_temp != NULL)
        {
            route_count++;
            route_temp = route_temp->next;
        }
        fprintf(fptr, "%d ", route_count);
        route_temp = city_temp->head;
        while(route_temp != NULL)
        {
            fprintf(fptr, " %s %g",route_temp->destination, route_temp->distance);
            route_temp = route_temp->next;
        }
        city_temp = city_temp->link;
        fprintf(fptr, "\n");
    }

    fclose(fptr);

}

void load_routes()
{
    FILE *fptr = fopen("routes.txt", "r");
    if(fptr == NULL)
    {
        perror("fopen");
        return;
    }

    char city[50];
    int count;
    int route_count;
    fscanf(fptr, "%u", &count);
    for(int i = 0; i < count; i++)
    {
        fscanf(fptr, "%s", city);
        add_city(city);
        fscanf(fptr, "%d", &route_count);
        char destination[50];
        float distance;
        for(int j = 0; j < route_count; j++)
        {
            fscanf(fptr, "%s", destination);
            fscanf(fptr, "%f", &distance);
            City *source = check_city_existance(city);
            create_route(source, destination, distance);
        }
        
    }

    fclose(fptr);

}