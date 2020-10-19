#include <iostream>
#include <stdlib.h>
#include <time.h>


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
    int x;
    int y;
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
        this->x = rand() % l;
        this->y = rand() % h;
    }
    Point() {
        this->id = Point::numOfPoints + 1;
        Point::numOfPoints += 1;
        this->x = 0;
        this->y = 0;
    }

};

int Point::numOfPoints = 0;

void Map::toString() {
    std::cout << "Dimensions : " << this->length << "x" << this->heigth << "\n";
}

int main()
{
    srand(time(NULL));
    Map mapa = Map(100,400);
    mapa.toString();
    Point p1 = Point(mapa.giveL(), mapa.giveH());
    Point p2 = Point(mapa.giveL(), mapa.giveH());
    
    //std::cout << Point::giveNumOfPoints()<<"\n\n";
    printf("Punkt %d - %dx%d\n", p1.giveID(), p1.giveX(), p1.giveY());
    

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
