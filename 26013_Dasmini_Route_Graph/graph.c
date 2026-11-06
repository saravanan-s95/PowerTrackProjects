#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "utils.h"

City *city_head;
unsigned int city_count;

void init_route()
{
    city_head = NULL;
    city_count = 0;
}



/**
 * @brief Add a city
 * 
 * @param
 * @return SUCCESS on adding city/city exists, FAILURE on failure to add
 */
int add_city(char cityName[])
{
    City *temp = city_head;
    City *prev = temp;
    while(temp != NULL)
    {
        if(strcmp(temp->name,cityName) == 0)
        {
            return CITY_EXISTS;
        }
        prev = temp;
        temp = temp->link;
    }
    City *newCity = malloc(sizeof(City));
    if (newCity == NULL)
        return FAILURE;
    if(city_head == NULL)
        city_head = newCity;
    else
        prev->link = newCity;
    snprintf(newCity->name, sizeof(newCity->name), "%s", cityName);
    newCity->link = NULL;
    newCity->head = NULL;
    city_count++;
    return SUCCESS;
}

/**
 * @brief Checks if routes exist already and create route to cities
 * 
 * @param
 * @return
 */
int add_route(char source[], char destination[], float distance)
{
    City *src_city_ptr, *dest_city_ptr;
    if((src_city_ptr = check_city_existance(source)) != NULL)
    {
        if((dest_city_ptr = check_city_existance(destination)) != NULL)
        {
            //update the route for the source and destination city 
            //call these contents inside while inside a function
            if(check_route_existance(src_city_ptr, destination) == NULL)    //if destination not exists already in src city
            {
                create_route(src_city_ptr,destination, distance);     
            }
            else
                return ROUTE_EXISTS;
            if(check_route_existance(dest_city_ptr, source) == NULL)    //if destination not exists already in src city
            {
                create_route(dest_city_ptr, source, distance);     
            }
            else
                return ROUTE_EXISTS;
            
        }
        else
            return FAILURE;
    }
    else
        return FAILURE;
    
}

/**
 * @brief Display all city and thier routes
 */
void display_roads()
{
    int is_found = 0;
    printf("            TOTAL CITIES : %d\n", city_count);
    printf("----------------------------------------\n");
    City *city_temp = city_head;
    while(city_temp != NULL)
    {
        is_found = 1;
        printf(GREY "%s\n" RESET,city_temp->name);
        Route *route_temp = city_temp->head;
        int i = 1;
        while(route_temp != NULL)
        {
            printf("    %d. %s : %gkm\n", i, route_temp->destination, route_temp->distance);
            i++;
            route_temp = route_temp->next;
        }
        city_temp = city_temp->link;
    }
    if(is_found == 0)
        printf("No cities found!\n");

}

int remove_city(char cityName[])
{
    City *temp = city_head;
    City *prev = temp;

    if (city_head == NULL) 
        return NOT_FOUND;

    if(strcmp(temp->name,cityName) == 0)
    {
        clear_routes(temp);         // to free up all the route nodes
        city_head = temp->link;
        free(temp);
        city_count--;
        return SUCCESS;
    }
    while(temp != NULL)
    {
        if(strcmp(temp->name,cityName) == 0)
        {
            //Remove city
            clear_routes(temp);         // to free up all the route nodes
            prev->link = temp->link;
            free(temp);
            city_count--;
            return SUCCESS;
        }
        prev = temp;
        temp = temp->link;
    }
    return NOT_FOUND;
}

int remove_route(char source[], char destination[])
{
    City *city_address = check_city_existance(source);
    delete_route_in_dest(destination, city_address);
    city_address = check_city_existance(destination);
    delete_route_in_dest(source, city_address);
}

int shortest_path(char source[], char dest[])
{
    if(check_city_existance(source) == NULL)
        return NOT_FOUND;
    City *dest_city = check_city_existance(dest);
        if(dest_city == NULL)
            return NOT_FOUND;
    City *temp = city_head;
    while(temp != NULL)
    {
        if(strcmp(temp->name, source) == 0)
        {
            temp->dist = 0;  
        }
        else
        {
            temp->dist = INFINITY;
        }
        temp->visited = 0;
        temp->prev = NULL;
        temp = temp->link;
    }
    while(1)
    {
        City *min_city = get_min_city();
        if(min_city == NULL || min_city->dist == INFINITY)
            break;
        min_city->visited = 1;

        Route *route = min_city->head;
        while(route != NULL)
        {
            City *neighbour = check_city_existance(route->destination);
            if(min_city->dist + route->distance < neighbour->dist)
            {
                neighbour->dist = min_city->dist + route->distance;
                neighbour->prev = min_city;
            }
            route = route->next;
        }
    }       
            // Check destination

        
        if(dest_city->dist == INFINITY)
            return NO_ROUTE;
        printf("Total distance: %.2fkm\n", dest_city->dist);

        print_path(dest_city);
        printf("\n");
}

/**
 * @brief Checks if the city is present already
 * 
 * @return Returns the city memory address if exists, otherwise, returns NULL.
 */
City *check_city_existance(char cityName[])
{
    City *temp = city_head;
    City *prev = temp;
    while(temp != NULL)
    {
        if(strcmp(temp->name,cityName) == 0)
        {
            return temp;
        }
        prev = temp;
        temp = temp->link;
    }
    return NULL;
}

/**
 * @brief Check if route exists already
 */
Route *check_route_existance(City *city_ptr, char destination[])
{
    Route *temp = city_ptr->head;
    Route *prev = temp;
    while(temp != NULL)
    {
        if(strcmp(temp->destination,destination) == 0)
        {
            return temp;
        }
        prev = temp;
        temp = temp->next;
    }
    return NULL;
}

void create_route(City *src_city_ptr,char destination[], float distance)
{
    Route *temp = src_city_ptr->head;
    Route *prev = temp;
    Route *newRoute = malloc(sizeof(Route));
    sscanf(destination, "%49s", newRoute->destination);
    newRoute->distance = distance;
    newRoute->isblocked = 0;
    newRoute->next = temp;
    src_city_ptr->head = newRoute; 
}

void clear_routes(City *city)
{
    Route *temp = city->head;
    Route *prev = temp;
    while(temp != NULL)
    {
        // Get the address of the destination city and remove the respective route in the destination city
        City *city_address = check_city_existance(temp->destination);
        delete_route_in_dest(city->name, city_address);
        prev = temp;
        temp = temp->next;
        free(prev);
    }
}

void delete_route_in_dest(char cityName[], City *city)
{
    Route *route_temp = city->head;
    Route *prev = route_temp;

    if(strcmp(route_temp->destination, cityName) == 0)
    {
        city->head = route_temp->next;
        free(route_temp);
        return;
    }
    while(route_temp != NULL)
    {
        if(strcmp(route_temp->destination, cityName) == 0)
        {
            prev->next = route_temp->next;
            free(route_temp);
            return;
        }
        prev = route_temp;
        route_temp = route_temp->next;
    }
}

City *get_min_city()
{
    City *min = NULL;
    City *temp = city_head;

    while(temp != NULL)
    {
        if(temp->visited == 0)   // only consider unvisited cities
        {
            if(min == NULL || temp->dist < min->dist)
            {
                min = temp;
            }
        }
        temp = temp->link;
    }
    return min;
}

void print_path(City *city)
{
    if(city == NULL)
        return;
    print_path(city->prev);
    if(city->prev != NULL)
    printf("-> ");
    printf("%s ", city->name );
    
}