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
        if (flag) {
            if (ch == 'X') { this->x -= rest; }
            if (ch == 'Y') { this->y -= rest; }

        }
        else {
            if (ch == 'X') { this->x += rest; }
            if (ch == 'Y') { this->y += rest; }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        printf("%dx%x\n", this->giveX(), this->giveY());

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
    
    printf("Punkt %d - %dx%d; %d; dest - %dx%d\n", v1.giveID(), v1.giveX(), v1.giveY(),v1.giveSpeed(),v1.giveDestX(),v1.giveDestY());
    printf("Punkt %d - %dx%d; %d; dest - %dx%d\n", v2.giveID(), v2.giveX(), v2.giveY(), v2.giveSpeed(), v2.giveDestX(), v2.giveDestY());
    Plane p1 = Plane(mapa.giveL(), mapa.giveH(), 10);
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
