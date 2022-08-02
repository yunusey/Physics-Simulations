#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#define PI 3.14
#include <vector>
#include <bits/stdc++.h>
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;

// For trials you can use the variables below...

// For window settings...
double l = -50, r = 50, t = 30, b = -30;

// For accurance of the animation
long long time_steps = 10000; // sets the accurence
// Warning! : to increase much may cause animation to get slower

// For mass ratio
double mass_ratio = 10000;
// Reminder! : Can also be changed using the initialization (NOT THE DECLARATION!) of Square1 in function(main) as well...

// For starting velocity
double starting_velocity = -120.0;
// Reminder! : It's for the starting velocity of the heavy Square so can be changed using the settings below as well...

// Counts collisions detected...
long long collision_count = 0;
bool is_paused = false;

// Our class for Squares...
struct Square{ 
    dvec3 pos = {0, 0, 0};
    dvec3 vel = {0, 0, 0};
    int width = 20, height = 20;
    double mass = 1;
    // Constructor 1
    Square(dvec3 Pos, dvec3 Vel, double Mass, int Width, int Height){
        pos    = Pos;
        vel    = Vel;
        mass   = Mass;
        width  = Width;
        height = Height;
    }
    // Constructor 2
    Square(){

    }
    // Allows squares move
    void Integrate(double dt){
        pos += vel * dt;
    }
    // Handles Collisions
    void HandleCollisions(){
        if(pos[0] <= l && vel[0] < 0){
            collision_count++;
            vel[0] = -vel[0];
        }
    }
    // Draws the Squares
    void draw(){
        double vertices[] = {
            pos[0],         pos[1],          pos[2],
            pos[0] + width, pos[1],          pos[2],
            pos[0] + width, pos[1] + height, pos[2],
            pos[0],         pos[1] + height, pos[2]
        };
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_DOUBLE, 0, vertices);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
    }

};

// Constrols if the Sqaures are collided
bool AreCollided(Square& A, Square& B){
    if(A.pos[0] <= B.pos[0] && B.pos[0] <= A.pos[0] + A.width){
        return true;
    }
    if(B.pos[0] <= A.pos[0] && A.pos[0] <= B.pos[0] + B.width){
        return true;
    }
    return false;
}

// Declaration of the objects
Square A, B;

inline bool didAnimationEnd(){
    return (B.vel[0] >= 0 && A.vel[0] > B.vel[0]);
}

void drawEveryThing(){
    glClearColor(0.0, 0.0, 0.2, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(l, r, b, t, -1, 1);

    A.draw();
    B.draw();

    // The texts in the animation are declared and initialized here:
    string collision_count_str = "Collision Count : " + to_string(collision_count); 
    glRasterPos2d(30, 25);
    for(auto character : collision_count_str){
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, character);
    }
    string velocityA = "Velocity A : " + to_string(A.vel[0]); 
    glRasterPos2d(-30, -20);
    for(auto character : velocityA){
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, character);
    }
    string velocityB = "Velocity B : " + to_string(B.vel[0]); 
    glRasterPos2d(-30, -22);
    for(auto character : velocityB){
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, character);
    }
    glutSwapBuffers();
}

void display(){
    
    // How much time passed...
    static int last = glutGet(GLUT_ELAPSED_TIME);
    int cur = glutGet(GLUT_ELAPSED_TIME);
    double dt = (cur - last) / (1000.0);
    last = cur;

    if(didAnimationEnd()){
        glClearColor(0.0, 0.0, 0.2, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(l, r, b, t, -1, 1);
        string animation_ended = "Animation Ended!"; 
        glRasterPos2d(-10, 20);
        for(auto character : animation_ended){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, character);
        }
        string bracket1 = "Properties{"; 
        glRasterPos2d(-10, 15);
        for(auto character : bracket1){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, character);
        }
        string mass_str = "Mass Ratio Between Objects: " + to_string(mass_ratio); 
        glRasterPos2d(-7, 12);
        for(auto character : mass_str){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, character);
        }
        string velocity_str = "Starting Velocity: " + to_string(starting_velocity); 
        glRasterPos2d(-7, 9);
        for(auto character : velocity_str){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, character);
        }
        string collision_str = "Collisions Detected: " + to_string(collision_count); 
        glRasterPos2d(-7, 6);
        for(auto character : collision_str){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, character);
        }
        string bracket2 = "}"; 
        glRasterPos2d(-10, 3);
        for(auto character : bracket2){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, character);
        }
        string restart_string = "You can restart using 'R' key!"; 
        glRasterPos2d(-15, -20);
        for(auto character : restart_string){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, character);
        }
        glutSwapBuffers();
        return;
    }

    if(is_paused){
        return;
    }

    // For accurence we need to control animation much more than 60 frames per second
    // A reminder : All we do is to control objects and put them where they should be at as much as possible.
    // So we are not actually increasing FPS(frames per second for details you may want to look: https://en.wikipedia.org/wiki/FPS)
    for(int time_step = 0; time_step < time_steps; time_step++){
        A.Integrate(dt / time_steps);
        B.Integrate(dt / time_steps);
        A.HandleCollisions();
        B.HandleCollisions();
        bool collided = AreCollided(A, B);
        collision_count += collided;
        // Basically if the objects are collided their velocities should change.
        // For details you may want to look at : https://en.wikipedia.org/wiki/Elastic_collision
        if(collided){
            double sum_masses = A.mass + B.mass;
            double vel_A = (A.mass - B.mass) / sum_masses * A.vel[0] + 2 * (B.mass / sum_masses) * B.vel[0];
            double vel_B = (B.mass - A.mass) / sum_masses * B.vel[0] + 2 * (A.mass / sum_masses) * A.vel[0];
            A.vel[0] = vel_A;
            B.vel[0] = vel_B;
        }
    }    
    drawEveryThing();
}

void keyPressed(unsigned char key, int x, int y){
    if(key == 'r' || key == 'R'){
        collision_count = 0;
        A = Square(dvec3{40, 0, 0}, dvec3{starting_velocity, 0, 0}, mass_ratio, 20, 20);
        B = Square(dvec3{0, 0, 0}, dvec3{0, 0, 0}, 1, 5, 5);

        drawEveryThing();    
    }
    if(key == ' '){
        is_paused = !is_paused;
    }
}

void timer(int value){
    glutPostRedisplay();
    glutTimerFunc(1000.0/1000.0, timer, value);
}

int main(int argc, char **argv){

    // Initializing Objects
    A = Square(dvec3{40, 0, 0}, dvec3{starting_velocity, 0, 0}, mass_ratio, 20, 20);
    B = Square(dvec3{0, 0, 0}, dvec3{0, 0, 0}, 1, 5, 5);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(1000, 600);
    glutCreateWindow("Collision Detection PI");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyPressed);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}