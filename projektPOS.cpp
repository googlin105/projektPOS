#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <chrono>


class Map {
private:
    int length;
    int heigth;
public:
    void toString();
    int giveL() { return this->length; }
    int giveH() { return this->heigth; }
    Map(int l, int h) {
        this->length = l;
        this->heigth = h;
    }
    Map() {
        this->length = 500;
        this->heigth = 500;
    }
    ~Map() {
        std::cout << "Map deleted.\n";
    }
};

class Point {
private:
    int id;
    int static numOfPoints;

protected:
    int x;
    int y;
    int mapL;                                 //zamieszczenie informacji o wielkosci mapy w klasie Point
    int mapH;
public:

    int giveX() { return x; }
    int giveY() { return y; }
    void setPoint(int x, int y) {
        this->x = x;
        this->y = y;
    }
    int giveID() { return this->id; }
    int static giveNumOfPoints() { return numOfPoints; }
    Point(int l, int h) { 
        this->id = Point::numOfPoints+1;
        Point::numOfPoints+=1; 
        this->x = rand() % l;                 //losowa generacja punktu poczatkowego
        this->y = rand() % h;
        this->mapL = l;
        this->mapH = h;
    }
    Point() {
        this->id = Point::numOfPoints + 1;
        Point::numOfPoints += 1;
        this->x = 0;
        this->y = 0;
    }

};

class Vehicle : public Point {
protected:
    int speed;
    int destX;
    int destY;
    void walkLine(int start, int end, char ch) {
        int diff = start - end;
        bool flag;
        if (diff >= 0) {
            flag = true;
        }
        else {
            diff = -diff;
            flag = false;
        }
        int hops = diff / this->speed;
        int rest = diff % this->speed;
        
        while (hops > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            printf("%dx%d\n", this->giveX(), this->giveY());
            if (flag) {
                if (ch == 'X') { this->x -= speed; }
                if (ch == 'Y') { this->y -= speed; }

            }
            else {
                if (ch == 'X') { this->x += speed; }
                if (ch == 'Y') { this->y += speed; }
            }
            hops--;
            
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        printf("%dx%d\n", this->giveX(), this->giveY());
        if (flag) {
            if (ch == 'X') { this->x -= rest; }
            if (ch == 'Y') { this->y -= rest; }

        }
        else {
            if (ch == 'X') { this->x += rest; }
            if (ch == 'Y') { this->y += rest; }
        }
        
        

    }
public:
    Vehicle(int l, int h, int speed) : Point(l, h) {
            this->speed = speed;
            this->destX = rand() % this->mapL;            // wyznaczenie punktu koncowego
            this->destY = rand() % this->mapH;
    }
    int giveDestX() { return this->destX; }
    int giveDestY() { return this->destY; }
    int giveSpeed() { return this->speed; }
    
    
};

class Ship : public Vehicle {
public:
    Ship(int l, int h, int speed) :Vehicle(l, h, speed) {};
    void run() {                                                   //poruszanie sie po prostokacie
        int startX = this->x;
        int startY = this->y;
        while (true) {
            this->walkLine(this->x, this->destX, 'X');
            this->walkLine(this->y, this->destY, 'Y');
            this->walkLine(this->x, startX, 'X');
            this->walkLine(this->y, startY, 'Y');
        }      
    }
};

class Plane : public Vehicle {
public:
    Plane(int l, int h, int speed) :Vehicle(l, h, speed) {};
    void run() {                                                   //poruszanie sie po prostej
        int startX = this->x;
        int startY = this->y;
        int choice = rand() % 2;
        char direction = 'X';
        switch (choice) {
        case 0:direction = 'X';
        case 1:direction = 'Y';
        }
        while (true) {
            switch (choice) {
            case 0: 
                this->walkLine(this->x, this->destX, 'X');
                this->walkLine(this->x, startX, 'X');
                break;
            
            case 1: 
                this->walkLine(this->y, this->destY, 'Y');
                this->walkLine(this->y, startY, 'Y');
                break;
            
            }
        }
    }
};

class Car :public Vehicle {
private:
    int giveDirection() {
        bool flagOut = true;
        int tmp;
        while (flagOut) {
            tmp = rand() % 4;
            switch (tmp) {
            case 0:
                if (this->y + this->speed <= this->mapH) { flagOut = false; }
                break;
            case 1:
                if (this->x + this->speed <= this->mapL) { flagOut = false; }
                break;
            case 2:
                if (this->y - this->speed >= 0) { flagOut = false; }
                break;
            case 3:
                if (this->x - this->speed >= 0) { flagOut = false; }
            }
        }
        return tmp;
    }
public:
    Car(int l, int h, int speed) :Vehicle(l, h, speed) {};
    void run() {                                              //losowe poruszanie sie
        while (true) {
            int direction = this->giveDirection();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            printf("%dx%d\n", this->giveX(), this->giveY());
            switch (direction) {
            case 0:
                this->y += this->speed;
                break;
            case 1:
                this->x += this->speed;
                break;
            case 2:
                this->y -= this->speed;
                break;
            case 3:
                this->x -= this->speed;

            }

        }

    }

};


int Point::numOfPoints = 0;

void Map::toString() {
    std::cout << "Dimensions : " << this->length << "x" << this->heigth << "\n";
}

int main()
{
    srand(time(NULL));
    const int LEN = 200;
    const int HEI = 200;
    Map mapa = Map(LEN,HEI);
    mapa.toString();
    Ship v1 = Ship(mapa.giveL(), mapa.giveH(),10);
    Ship v2 = Ship(mapa.giveL(), mapa.giveH(),5);
    
    //printf("Punkt %d - %dx%d; %d; dest - %dx%d\n", p1.giveID(), p1.giveX(), p1.giveY(),p1.giveSpeed(),p1.giveDestX(),p1.giveDestY());
    //printf("Punkt %d - %dx%d; %d; dest - %dx%d\n", v2.giveID(), v2.giveX(), v2.giveY(), v2.giveSpeed(), v2.giveDestX(), v2.giveDestY());
    Car p1 = Car(mapa.giveL(), mapa.giveH(), 10);
    printf("Punkt %d - %dx%d; %d; dest - %dx%d\n", p1.giveID(), p1.giveX(), p1.giveY(), p1.giveSpeed(), p1.giveDestX(), p1.giveDestY());
    p1.run();

}

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
