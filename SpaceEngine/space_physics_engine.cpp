#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#define PI 3.14
#include <vector>
#include <bits/stdc++.h>
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;

int seconds_past = 0;
double pres = 100, speed = 1;

vector<dvec3> marks, mark_colors;

struct Planet{
    dvec3 pos 	= {0, 0, 0};
    dvec3 vel 	= {0, 0, 0};
	dvec3 force = {0, 0, 0};
    dvec3 color = {255, 255, 255};
	int radius = 1;
    double mass = 1e5;
    bool can_move = true, mark = false;

	Planet(double Mass, dvec3 Vel, dvec3 Pos, int Radius = 1, bool Can_move = true, bool Mark = false, dvec3 Color = {255, 255, 255}){
		pos = Pos;
		vel = Vel;
		mass = Mass;
		radius = Radius;
        can_move = Can_move;
        mark = Mark;
        color = Color;
	}
    Planet copy(){
        return Planet(mass, vel, pos, radius, can_move, mark);
    }
    void Integrate(double dt){
        if(!can_move) return;
		dvec3 acc = force / mass;
        vel += acc * dt;
		pos += vel * dt;
    }

    void drawPlanet(){

        vector<double> points;
        points.push_back(pos[0]);
        points.push_back(pos[1]);
        points.push_back(pos[2]);
        vector<double> colours;
        colours.push_back(0);
        colours.push_back(0);
        colours.push_back(0);
        int segments = 360;
        unsigned int indices[segments * 2];
        int r = 0, g = 0, b = 0;                                                                                                                                                              
        for(int i = 0; i <= segments; i++){
            double x = pos[0] + radius * cos(2 * PI * i / segments), y = pos[1] + radius * sin(2 * PI * i / segments), z = 0;
            points.push_back(x);
            points.push_back(y);
            points.push_back(z);
            colours.push_back(255);
            colours.push_back(255);
            colours.push_back(255);
        }

        for(int i = 0; i <= segments; i++){
            indices[2 * i] = 0; indices[2 * i + 1] = i + 1;
        }

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(3, GL_DOUBLE, 0, &points[0]);
        glColorPointer(3, GL_DOUBLE, 0, &colours[0]);
        unsigned int ibo;
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        glDrawElements(GL_LINES, 2 * segments, GL_UNSIGNED_INT, nullptr);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

    }

};
const double gravity = 1.0;
void Integrate(vector<Planet>& planets, double dt){
	for(auto& planet : planets) planet.force = {0, 0, 0};
	for(int i = 0; i < planets.size(); i++){
		for(int j = i + 1; j < planets.size(); j++){
			Planet& p1 = planets[i];
			Planet& p2 = planets[j];

			dvec3 diff = p2.pos - p1.pos;
			double distance = glm::length2(diff);

			dvec3 last_term = dvec3{diff[0] * diff[0], diff[1] * diff[1], diff[2] * diff[2]} / sqrt(distance);
			dvec3 normalized = glm::normalize(diff);
	
			dvec3 force = gravity * normalized * p1.mass * p2.mass / distance;
			p1.force += force;
			p2.force -= force;
		}
	}
    for(auto& i : planets){
        i.Integrate(dt);
    }
}

vector<Planet> planets;
void display(){
    
    static int last = glutGet(GLUT_ELAPSED_TIME);
    int cur = glutGet(GLUT_ELAPSED_TIME);
    double dt = (cur - last) / 1000.0 * speed;
    last = cur;

    Integrate(planets, dt);

    glClearColor(0.0, 0.0, 0.3, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-pres, pres, -pres, pres, -1, 1);   

    for(auto& particle : planets){
        particle.drawPlanet();
    }

    for(auto& planet : planets){
        if(planet.mark == true){
            if(marks.size() == 2000){
                marks.erase(marks.begin());
                mark_colors.erase(mark_colors.begin());
            }
            marks.push_back(planet.pos);
            mark_colors.push_back(planet.color);
        }
    }
    // std::cout << marks.size() << std::endl;
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, &marks[0]);
    glDrawArrays(GL_POINTS, 0, marks.size()); 
    glDisableClientState(GL_VERTEX_ARRAY);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, &marks[0]);
    glColorPointer(3, GL_DOUBLE, 0, &mark_colors[0]);
    glDrawArrays(GL_POINTS, 0, marks.size());// NORMAL TOP 
    //glDrawElements(GL_LINES, 2 * segments, GL_UNSIGNED_INT, nullptr);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glutSwapBuffers();
}

void timer(int value){
    glutPostRedisplay();
    glutTimerFunc(1000.0/60.0, timer, value);
}

int main(int argc, char **argv){
    Planet Ultimum =  Planet(1e5, {0, 0, 0}, {-30, 0, 0}, 5, false);
	Planet Sun     =  Planet(1e5, {0, 0, 0}, {0, 0, 0}, 5, false);
	Planet Earth   =  Planet(1e4, {0, -50, 0}, {56, 0, 0}, 2, true, true, {255, 255, 0});
    Planet Moon    =  Planet(1e0, {0, -90, 0}, {59, 0, 0}, 1, true);
    Planet Ultimum2 = Ultimum.copy();
    Ultimum2.pos = {30, 0, 0};
    Planet Ultimum3 = Ultimum.copy();
    Ultimum3.pos = {0, -30, 0};


	planets = {Sun, Earth, Moon};
             
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Space!");

    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}