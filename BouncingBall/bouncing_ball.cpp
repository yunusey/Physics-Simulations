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

struct Ball{
    int radius = 1; 
    dvec3 pos = {0, 0, 0};
    dvec3 vel = {5, 0, 0};
    double acc = -20;
    bool made = false;
    dvec3 last_vel;
    void Integrate(double dt){
        static int count = 0;
        count++;
        if(!made)
            vel += dvec3{0, acc * dt, 0};
        pos = pos +  vel * dt;
        std::cout << pos[1] << " " << count << std::endl;
        made = false;
    }
    void HandleCollisions(){
        if(pos[1] -radius < -9.8 && vel[1] < 0 || pos[1] + radius > 9.8 && vel[1] > 0){
            made = true;
            vel[1] = -vel[1];
        }
        if(pos[0] - radius < -9.8 || pos[0] + radius > 9.8){
            vel[0] = -vel[0];
        }
    }

    void drawParticle(){


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
        //glDrawArrays(GL_LINE_LOOP, 0, points.size() / 3); NORMAL TOP 
        glDrawElements(GL_LINES, 2 * segments, GL_UNSIGNED_INT, nullptr);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);


        glutSwapBuffers();
    }

};

void Integrate(vector<Ball>& particles, double dt){

    for(auto& i : particles){
        i.Integrate(dt);
        i.HandleCollisions();
    }
}

vector<Ball> particles;
void display(){
    
    static int last = glutGet(GLUT_ELAPSED_TIME);
    int cur = glutGet(GLUT_ELAPSED_TIME);
    double dt = (cur - last) / 1000.0;
    last = cur;

    Integrate(particles, dt);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10, 10, -10, 10, -1, 1);   

    particles[0].drawParticle();    
}

void timer(int value){
    glutPostRedisplay();
    glutTimerFunc(1000.0/60.0, timer, value);
}

int main(int argc, char **argv){

    particles.resize(1);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Ziplayan Muthis Top");

    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}