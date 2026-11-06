#include <stdio.h>
#include "graph.h"
#include "utils.h"
#include "storage.h"
#include "validation.h"

int main()
{
    init_route();
    load_routes();
    float distance;
    int option, res;
    char city[50], destination[50];

    /* Menu*/
    do
    {
        printf(YELLOW "==============================================\n");
        printf( "             WHAT DO YOU WANT?            \n");
        printf("==============================================\n" RESET);
        printf("1. Display city routes\n");
        printf("2. Add City\n");
        printf("3. Add route\n");
        printf("4. Find shortest path\n");
        printf("5. Remove city\n");
        printf("6. Remove route\n");
        printf("7. Exit\n");

        printf("Select option : ");
        scanf("%d", &option);

        switch(option)
        {
            case 1:
                display_roads();
                break;
            case 2:         // Add city
                printf("Enter city (No spaces): ");
                scanf(" %49[^\n]", city);
                if(check_city(city) == NOT_VALID)
                {
                    printf("City name is not valid\n");
                    break;
                }
                res = add_city(city);
                if(res == SUCCESS)
                    printf(GREEN "City added\n" RESET);
                else if(res == CITY_EXISTS)
                    printf("City already exists\n");
                else
                    printf(RED "City not added\n" RESET);
                break;
            case 3:         // Add route
                printf("Enter source city (No spaces): ");
                scanf(" %49[^\n]", city);
                if(check_city(city) == NOT_VALID)
                {
                    printf("Source city name is not valid\n");
                    break;
                }
                printf("Enter destination (No spaces): ");
                scanf(" %49[^\n]", destination);
                if(check_city(destination) == NOT_VALID)
                {
                    printf("Destination city name is not valid\n");
                    break;
                }
                printf("distance (km): ");
                scanf("%f", &distance);
                if(check_distance(distance) == NOT_VALID)
                {
                    printf(RED "Distance is not a valid value\n" RESET);
                    break;
                }
                res = add_route(city, destination, distance);
                if(res == SUCCESS)
                    printf(GREEN "Route added\n" RESET);
                else if(res == ROUTE_EXISTS)
                    printf(YELLOW "Route exists already\n" RESET);
                else if(res == FAILURE)
                    printf(RED "Route adding failed! Check if Source and destination cities exists\n" RESET);
                break;
            case 4:         //Find shortest path
                printf("Enter source city (No spaces): ");
                scanf(" %49[^\n]", city);
                printf("Enter destination (No spaces): ");
                scanf(" %49[^\n]", destination);
                int res = shortest_path(city, destination);
                if(res == NO_ROUTE)
                    printf("No route found\n");
                else if(res == NOT_FOUND)  
                    printf("Source or destination city not found\n"); 
                break;
            case 5:         //Remove city
                printf("Enter city (No spaces): ");
                scanf(" %49[^\n]", city);
                res = remove_city(city);
                if(res == SUCCESS)
                    printf(GREEN "City removed\n" RESET);
                else
                    printf("City not found\n");
                break;
            case 6:         //Remove Route
                printf("Enter source city (No spaces): ");
                scanf(" %49[^\n]", city);
                printf("Enter destination (No spaces): ");
                scanf(" %49[^\n]", destination);
                res = remove_route(city, destination);
                if(res == SUCCESS)
                    printf(GREEN "Route removed\n" RESET);
                else if(res == FAILURE)
                    printf(RED "Route removing failed! Check if Source and destination cities exists\n" RESET);
            case 7:
                save_routes();
                break;

        }
    } while(option != 7);

    return 0;
}