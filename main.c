#include <stdio.h>
#include <time.h>
#include "graphics.h"
#include "glm.h"

bool init();
void kill();
bool loop();

void cube();
void pyramid();
void clockf();

SDL_Window* window;
SDL_Renderer* renderer;

#define PI 3.1415927f

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("usage: %s model\n\t"
			"model - clock, cube, pyramid\n", 
			argv[0]);
		exit(1);
	}
	if (!init()) return 1;
	while (loop(argv[1])) SDL_Delay(100);
	kill();
	return 0;
}

bool loop(char *model) {
	SDL_Event e;

	color(renderer, 0, 0, 0, 0);
	clear(renderer);

	while (SDL_PollEvent(&e) != 0)
		switch (e.type) {
			case SDL_EVENT_QUIT:
				return false;
		}

	if (!strcmp(model, "clock")){
		clockf();
	} else if (!strcmp(model, "cube")){
		cube();
	} else if (!strcmp(model, "pyramid")){
		pyramid();
	} else { 
		printf("invalid model: %s\n", model); return false; 
	}

	update(renderer);
	return true;
}

bool init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("failed to initialize sdl: %s\n", SDL_GetError());
		return false;
	} 

	window = SDL_CreateWindow("Window", 256, 256, 0);
	if (!window) {
        printf("failed to create window: %s\n", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(window, NULL);
	if (!renderer) {
        printf("failed to create renderer: %s\n", SDL_GetError());
		return false;
	}

	color(renderer, 255, 255, 255, 255);
	clear(renderer);
	return true;
}

void kill() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

float *clockhands() {
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	int hour = tm->tm_hour;
	int min = tm->tm_min;
	int sec = tm->tm_sec;
	int hourdeg = 360 / 12 * (hour % 12) + min / 2 - 180;
	int mindeg  = 360 / 60 * min  - 180;
	int secdeg  = 360 / 60 * sec  - 120;
	float hourrad = hourdeg * PI / 180;
	float minrad  = mindeg  * PI / 180;
	float secrad  = secdeg  * PI / 180;
	float *v = malloc(3 * sizeof(float));
	v[0] = hourrad;
	v[1] = minrad;
	v[2] = secrad;
	return v;
}

void clockf() {
	float *a = clockhands();
	color(renderer, 211, 211, 211, 255);
	circle(renderer, FILL, 128, 128, 96);
	color(renderer, 0, 0, 255, 255);
	vec2 hand[] = {
		{-0.3f, -1.0f}, {-0.2f, 8.0f},
		{ 0.0f,  9.0f}, { 0.2f, 8.0f},
		{ 0.3f, -1.0f},
	};
	vec2 hour[5];
	memcpy(hour, hand, 5*sizeof(vec2));
	for (int i = 0; i < 5; i++) {
		vec2_scale(hour[i], 8);
		vec2_rot(hour[i], a[0], hour[i]);
		vec2_add(hour[i], (vec2){128, 128}, hour[i]);
	}
	polygon(renderer, FILL, (float *) hour, 5);
	vec2 minute[5];
	memcpy(minute, hand, 5*sizeof(vec2));
	for (int i = 0; i < 5; i++) {
		vec2_scale(minute[i], 10);
		vec2_rot(minute[i], a[1], minute[i]);
		vec2_add(minute[i], (vec2){128, 128}, minute[i]);
	}
	polygon(renderer, FILL, (float *) minute, 5);
	color(renderer, 255, 0, 0, 255);
	vec2 second[] = {
		{-0.3f, -1.0f}, {-0.3f,  9.0f},
		{ 0.3f,  9.0f}, { 0.3f, -1.0f},
	};
	for (int i = 0; i < 4; i++) {
		vec2_scale(second[i], 5);
		vec2_shear(second[i], (vec2){1.6f, 1}, second[i]);
		vec2_rot(second[i], a[2], second[i]);
		vec2_add(second[i], (vec2){128, 128}, second[i]);
	}
	polygon(renderer, FILL, (float *) second, 4);
	free(a);
}

void cube() {
	vec3 vertices[] = {
		{-0.5f, -0.5f, -0.5f},
		{-0.5f,  0.5f, -0.5f},
		{ 0.5f,  0.5f, -0.5f},
		{ 0.5f, -0.5f, -0.5f},
		{-0.5f, -0.5f,  0.5f},
		{-0.5f,  0.5f,  0.5f},
		{ 0.5f,  0.5f,  0.5f},
		{ 0.5f, -0.5f,  0.5f},
	};
	ivec3 faces[] = {
		{0, 1, 2}, {0, 2, 3},
		{4, 5, 1}, {4, 1, 0},
		{3, 2, 6}, {3, 6, 7},
		{7, 6, 5}, {7, 5, 4},
		{6, 2, 1}, {6, 1, 5},
		{3, 7, 4}, {3, 4, 0},
	};
	static float angle = 0;
	int nv = sizeof(vertices) / sizeof(vec3);
	int nf = sizeof(faces) / sizeof(ivec3);
	for (int i = 0; i < nv; i++) {
		vec3_roty(vertices[i], angle += 0.02f, vertices[i]);
		vec3_scale(vertices[i], 1.4f);
		vec3_add(vertices[i], (vec3){0, 0, 2}, vertices[i]);
	}
	color(renderer, 255, 255, 255, 255);
	mesh(renderer, (float *) vertices, (int *) faces, nv, nf, 255);
}

void pyramid() {
	vec3 vertices[] = {
		{-0.5f, 0.8f, -0.5f},
		{-0.5f, 0.8f,  0.5f},
		{ 0.5f, 0.8f,  0.5f},
		{ 0.5f, 0.8f, -0.5f},
		{ 0.0f, 0.2f,  0.0f},
	};
	ivec3 faces[] = {
		{0, 1, 2}, {0, 2, 3},
		{0, 1, 4}, {1, 2, 4},
		{3, 4, 2}, {0, 4, 3},
	};
	static float angle = 0;
	int nv = sizeof(vertices) / sizeof(vec3);
	int nf = sizeof(faces) / sizeof(ivec3);
	for (int i = 0; i < nv; i++) {
		vec3_roty(vertices[i], angle += 0.02f, vertices[i]);
		vec3_scale(vertices[i], 1.4f);
		vec3_add(vertices[i], (vec3){0, 0, 2}, vertices[i]);
	}
	color(renderer, 255, 255, 255, 255);
	mesh(renderer, (float *) vertices, (int *) faces, nv, nf, 255);
}
