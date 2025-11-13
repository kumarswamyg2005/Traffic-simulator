#ifndef PARKS_H
#define PARKS_H

#include <cmath>
#include <initializer_list>

void drawParkArea(float cx, float cz) {
    // Diverse items in circle - trees, sculptures, kiosks, gazebos, fountains
    for(int i=0;i<10;i++){
        float a=(i/10.0f)*2*3.14159f;
        float x=cx+cos(a)*56;
        float z=cz+sin(a)*56;
        
        int itemType = i % 5; // 5 different types of items
        
        if(itemType == 0 || itemType == 1) {
            // Trees (40% of items)
            glColor3f(0.4f,0.25f,0.1f);
            glPushMatrix();
            glTranslatef(x,3,z);
            glRotatef(-90,1,0,0);
            GLUquadric* trunk=gluNewQuadric();
            gluCylinder(trunk,0.8f,0.8f,6,8,1);
            gluDeleteQuadric(trunk);
            glPopMatrix();
            glColor3f(0.2f+(i%3)*0.1f,0.6f,0.3f);
            glPushMatrix();
            glTranslatef(x,8,z);
            glutSolidSphere(4.5f,10,10);
            glPopMatrix();
        }
        else if(itemType == 2) {
            // Modern Art Sculpture (abstract tower)
            glColor3f(0.7f,0.7f,0.8f); // Silver/gray
            glPushMatrix();
            glTranslatef(x,4,z);
            glRotatef(45,0,1,0);
            drawBox(1.5f,8,1.5f);
            glPopMatrix();
            glColor3f(0.9f,0.5f,0.2f); // Bronze accent
            glPushMatrix();
            glTranslatef(x,8.5f,z);
            glutSolidSphere(2.0f,12,12);
            glPopMatrix();
        }
        else if(itemType == 3) {
            // Small Kiosk/Info Board
            // Base
            glColor3f(0.3f,0.2f,0.15f);
            glPushMatrix();
            glTranslatef(x,2,z);
            drawBox(4,4,2);
            glPopMatrix();
            // Roof
            glColor3f(0.8f,0.3f,0.2f); // Red roof
            glPushMatrix();
            glTranslatef(x,4.5f,z);
            glRotatef(45,0,1,0);
            drawBox(5,0.5f,5);
            glPopMatrix();
            // Sign board
            glColor3f(0.9f,0.9f,0.7f);
            glPushMatrix();
            glTranslatef(x,2.5f,z-1.2f);
            drawBox(3.5f,2.5f,0.2f);
            glPopMatrix();
        }
        else if(itemType == 4) {
            // Small Fountain
            // Basin
            glColor3f(0.6f,0.6f,0.7f);
            glPushMatrix();
            glTranslatef(x,1,z);
            glRotatef(-90,1,0,0);
            GLUquadric* basin=gluNewQuadric();
            gluCylinder(basin,3.5f,3.0f,2,12,1);
            gluDeleteQuadric(basin);
            glPopMatrix();
            // Water
            glColor3f(0.3f,0.5f,0.8f);
            glPushMatrix();
            glTranslatef(x,2.8f,z);
            drawBox(5,0.2f,5);
            glPopMatrix();
            // Center pillar
            glColor3f(0.8f,0.8f,0.9f);
            glPushMatrix();
            glTranslatef(x,4.5f,z);
            glRotatef(-90,1,0,0);
            GLUquadric* pillar=gluNewQuadric();
            gluCylinder(pillar,0.5f,0.5f,4,8,1);
            gluDeleteQuadric(pillar);
            glPopMatrix();
        }
    }
    
    // Flowerbeds
    for(int fb=0;fb<2;fb++){
        float fbX=cx+(fb==0?-35:35);
        glColor3f(0.4f,0.25f,0.15f);
        glPushMatrix();
        glTranslatef(fbX,0.3f,cz);
        drawBox(12,0.6f,12);
        glPopMatrix();
        for(int f=0;f<15;f++){
            if((f+fb)%4==0) glColor3f(1,0.2f,0.3f);
            else if((f+fb)%4==1) glColor3f(1,0.8f,0.2f);
            else if((f+fb)%4==2) glColor3f(0.8f,0.3f,0.9f);
            else glColor3f(1,1,1);
            glPushMatrix();
            glTranslatef(fbX+(rand()%100/100.0f-0.5f)*10,0.8f,cz+(rand()%100/100.0f-0.5f)*10);
            glutSolidSphere(0.5f,8,8);
            glPopMatrix();
        }
    }
    
    // Benches
    for(int b=0;b<4;b++){
        float bx=cx+(b==2?50:(b==3?-50:0));
        float bz=cz+(b==0?50:(b==1?-50:0));
        glPushMatrix();
        glTranslatef(bx,0,bz);
        glColor3f(0.55f,0.35f,0.2f);
        glPushMatrix();
        glTranslatef(0,1.2f,0);
        drawBox(4,0.3f,1.2f);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0,1.8f,-0.5f);
        drawBox(4,1,0.2f);
        glPopMatrix();
        glPopMatrix();
    }
    
    // Lampposts
    for(int l=0;l<3;l++){
        glPushMatrix();
        glTranslatef(cx+(l-1)*30,0,cz+35);
        glColor3f(0.2f,0.2f,0.2f);
        glPushMatrix();
        glTranslatef(0,4,0);
        drawBox(0.4f,8,0.4f);
        glPopMatrix();
        glColor3f(1,0.95f,0.7f);
        glPushMatrix();
        glTranslatef(0,7.5f,0);
        glutSolidSphere(0.7,8,8);
        glPopMatrix();
        glPopMatrix();
    }
    
    // Picnic table
    glPushMatrix();
    glTranslatef(cx,0,cz);
    glColor3f(0.6f,0.4f,0.2f);
    glPushMatrix();
    glTranslatef(0,1.8f,0);
    drawBox(6,0.4f,4);
    glPopMatrix();
    for(float tx:{-2.5f,2.5f}){
        for(float tz:{-1.5f,1.5f}){
            glPushMatrix();
            glTranslatef(tx,0.9f,tz);
            drawBox(0.4f,1.8f,0.4f);
            glPopMatrix();
        }
    }
    glPopMatrix();
    
    // Hedges
    glColor3f(0.25f,0.55f,0.35f);
    for(int h=0;h<20;h++){
        float a=(h/20.0f)*2*3.14159f;
        glPushMatrix();
        glTranslatef(cx+cos(a)*70,0.8f,cz+sin(a)*70);
        drawBox(3,1.6f,3);
        glPopMatrix();
    }
}

void drawModernSculpture(float x, float z) {
    // Modern Art Sculpture (abstract tower)
    glColor3f(0.7f,0.7f,0.8f); // Silver/gray
    glPushMatrix();
    glTranslatef(x,4,z);
    glRotatef(45,0,1,0);
    drawBox(1.5f,8,1.5f);
    glPopMatrix();
    glColor3f(0.9f,0.5f,0.2f); // Bronze accent
    glPushMatrix();
    glTranslatef(x,8.5f,z);
    glutSolidSphere(2.0f,12,12);
    glPopMatrix();
}

void drawKiosk(float x, float z) {
    // Small Kiosk/Info Board
    // Base
    glColor3f(0.3f,0.2f,0.15f);
    glPushMatrix();
    glTranslatef(x,2,z);
    drawBox(4,4,2);
    glPopMatrix();
    // Roof
    glColor3f(0.8f,0.3f,0.2f); // Red roof
    glPushMatrix();
    glTranslatef(x,4.5f,z);
    glRotatef(45,0,1,0);
    drawBox(5,0.5f,5);
    glPopMatrix();
    // Sign board
    glColor3f(0.9f,0.9f,0.7f);
    glPushMatrix();
    glTranslatef(x,2.5f,z-1.2f);
    drawBox(3.5f,2.5f,0.2f);
    glPopMatrix();
}

void drawSmallFountain(float x, float z) {
    // Small Fountain
    // Basin
    glColor3f(0.6f,0.6f,0.7f);
    glPushMatrix();
    glTranslatef(x,1,z);
    glRotatef(-90,1,0,0);
    GLUquadric* basin=gluNewQuadric();
    gluCylinder(basin,3.5f,3.0f,2,12,1);
    gluDeleteQuadric(basin);
    glPopMatrix();
    // Water
    glColor3f(0.3f,0.5f,0.8f);
    glPushMatrix();
    glTranslatef(x,2.8f,z);
    drawBox(5,0.2f,5);
    glPopMatrix();
    // Center pillar
    glColor3f(0.8f,0.8f,0.9f);
    glPushMatrix();
    glTranslatef(x,4.5f,z);
    glRotatef(-90,1,0,0);
    GLUquadric* pillar=gluNewQuadric();
    gluCylinder(pillar,0.5f,0.5f,4,8,1);
    gluDeleteQuadric(pillar);
    glPopMatrix();
}

void drawScatteredCityItems() {
    // Sculptures scattered throughout city - avoiding roads at -400,-200,0,200,400
    float sculptures[][2] = {
        {-300, 350}, {-100, 350}, {100, 350}, {300, 350},
        {-350, 150}, {-250, 150}, {250, 150}, {350, 150},
        {-300, 50}, {-100, 50}, {100, 50}, {300, 50},
        {-350, -50}, {-250, -50}, {250, -50}, {350, -50},
        {-300, -150}, {-100, -150}, {100, -150}, {300, -150},
        {-350, -250}, {-150, -250}, {150, -250}, {350, -250}
    };
    for(auto& s : sculptures) {
        drawModernSculpture(s[0], s[1]);
    }
    
    // Kiosks scattered throughout city - avoiding roads
    float kiosks[][2] = {
        {-320, 280}, {-120, 280}, {120, 280}, {320, 280},
        {-380, 150}, {-180, 150}, {180, 150}, {380, 150},
        {-320, 50}, {-120, 50}, {120, 50}, {320, 50},
        {-380, -50}, {-180, -50}, {180, -50}, {380, -50},
        {-320, -150}, {-120, -150}, {120, -150}, {320, -150},
        {-380, -280}, {-180, -280}, {180, -280}, {380, -280}
    };
    for(auto& k : kiosks) {
        drawKiosk(k[0], k[1]);
    }
    
    // Small fountains scattered throughout city - avoiding roads
    float fountains[][2] = {
        {-260, 260}, {-60, 260}, {60, 260}, {260, 260},
        {-300, 150}, {-100, 150}, {100, 150}, {300, 150},
        {-260, 50}, {-60, 50}, {60, 50}, {260, 50},
        {-300, -50}, {-100, -50}, {100, -50}, {300, -50},
        {-260, -150}, {-60, -150}, {60, -150}, {260, -150},
        {-300, -260}, {-100, -260}, {100, -260}, {300, -260}
    };
    for(auto& f : fountains) {
        drawSmallFountain(f[0], f[1]);
    }
}

void drawAllParks() {
    // All city blocks
    float blocks[][2] = {
        {-300,300},{-100,300},{100,300},
        {-300,100},{-100,100},{100,100},{300,100},
        {-300,-100},{-100,-100},{100,-100},{300,-100},
        {-300,-300},{-100,-300},{100,-300},{300,-300}
    };
    
    for(auto& b:blocks){
        drawParkArea(b[0],b[1]);
    }
    
    // Add scattered items throughout the city
    drawScatteredCityItems();
}

#endif
