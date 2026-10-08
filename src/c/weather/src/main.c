/* User interface: reads the city, downloads the weather with curl and prints the report. */
#include "weather.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define CITY_SIZE 128
#define URL_SIZE 512
#define REPORT_SIZE 4096
#define CURL_HTTP_ERROR 22

/* Joins the command-line words into one city name, e.g. "New" "York" -> "New York". */
static void join_args(char *city, size_t size, int argc, char *argv[]) {
    city[0] = '\0';
    for (int i = 1; i < argc; i++) {
        if (i > 1) {
            strncat(city, " ", size - strlen(city) - 1);
        }
        strncat(city, argv[i], size - strlen(city) - 1);
    }
}

/* Asks for the city when none was given on the command line. */
static int read_city(char *city, size_t size) {
    printf("City or postal code: ");
    fflush(stdout);
    if (fgets(city, (int)size, stdin) == NULL) {
        return -1;
    }
    city[strcspn(city, "\n")] = '\0';
    return 0;
}

/* Runs curl and returns the reply (caller frees it). exit_code is curl's exit status. */
static char *download(const char *url, int *exit_code) {
    char command[URL_SIZE + 32];
    snprintf(command, sizeof(command), "curl -sf -m 10 '%s'", url);
    FILE *pipe = popen(command, "r");
    if (pipe == NULL) {
        return NULL;
    }

    size_t capacity = 65536;
    size_t length = 0;
    char *data = malloc(capacity);
    if (data == NULL) {
        pclose(pipe);
        return NULL;
    }
    size_t count;
    while ((count = fread(data + length, 1, capacity - length - 1, pipe)) > 0) {
        length += count;
        if (length == capacity - 1) {
            char *bigger = realloc(data, capacity * 2);
            if (bigger == NULL) {
                free(data);
                pclose(pipe);
                return NULL;
            }
            data = bigger;
            capacity *= 2;
        }
    }
    data[length] = '\0';
    *exit_code = WEXITSTATUS(pclose(pipe));
    return data;
}

int main(int argc, char *argv[]) {
    char city[CITY_SIZE];
    if (argc >= 2) {
        join_args(city, sizeof(city), argc, argv);
    } else if (read_city(city, sizeof(city)) != 0) {
        fprintf(stderr, "No city given.\n");
        return EXIT_FAILURE;
    }

    char url[URL_SIZE];
    if (strlen(city) == 0 || weather_url(url, sizeof(url), city) != 0) {
        fprintf(stderr, "Please enter a city name or postal code.\n");
        return EXIT_FAILURE;
    }

    int exit_code = 0;
    char *json = download(url, &exit_code);
    if (json == NULL) {
        fprintf(stderr, "Could not start curl. Is it installed?\n");
        return EXIT_FAILURE;
    }
    if (exit_code == CURL_HTTP_ERROR) {
        fprintf(stderr, "City not found: %s\n", city);
    } else if (exit_code != 0) {
        fprintf(stderr, "Network error: could not reach wttr.in (curl exit code %d).\n", exit_code);
    } else {
        Weather weather;
        if (weather_parse(json, &weather) == 0) {
            char report[REPORT_SIZE];
            weather_report(&weather, city, report, sizeof(report));
            fputs(report, stdout);
            free(json);
            return EXIT_SUCCESS;
        }
        fprintf(stderr, "Empty or unexpected response from wttr.in for: %s\n", city);
    }
    free(json);
    return EXIT_FAILURE;
}
