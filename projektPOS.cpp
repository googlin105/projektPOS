#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <chrono>
#include <vector>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

class Point {
private:
    int id;
    int static numOfPoints;

protected:
    int x;
    int y;
    int mapL;                               //zamieszczenie informacji o wielkosci mapy w klasie Point
    int mapH;
public:

    volatile int giveX() { return x; }
    volatile int giveY() { return y; }
    void setPoint(int x, int y) {
        this->x = x;
        this->y = y;
    }
    int giveID() { return this->id; }
    int static giveNumOfPoints() { return numOfPoints; }
    Point(int l, int h) {
        this->id = Point::numOfPoints + 1;
        Point::numOfPoints += 1;
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
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            //printf("%d:%dx%d\n", this->giveID(), this->giveX(), this->giveY());
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
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        //printf("%d:%dx%d\n", this->giveID(), this->giveX(), this->giveY());
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
    virtual void run() = 0;
    Vehicle(int l, int h, int speed) : Point(l, h) {
        this->speed = speed;
        this->destX = rand() % this->mapL;            // wyznaczenie punktu koncowego
        this->destY = rand() % this->mapH;
    }
    Vehicle() :Point() {
        this->speed = 5;
        this->destX = rand() % 100;
        this->destY = rand() % 100;
    };
    Vehicle(const Vehicle&) = default;
    Vehicle& operator=(const Vehicle&) = default;
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
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            //printf("%d:%dx%d\n", this->giveID(), this->giveX(), this->giveY());
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

class Map {
private:
    int length;
    int heigth;

public:
    std::vector <Car> cars;
    std::vector <Plane> planes;
    std::vector <Ship> ships;
    std::vector <std::thread> vecOfThreads;
    int numberOfCars = 0;
    int numberOfPlanes = 0;
    int numberOfShips = 0;
    int numberOfThreads = 0;
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

    void addCar(int speed) {
        Car tmp = Car(this->length, this->heigth, speed);
        this->cars.push_back(tmp);
        this->numberOfCars++;
    }
    void addShip(int speed) {
        Ship tmp = Ship(this->length, this->heigth, speed);
        this->ships.push_back(tmp);
        this->numberOfShips++;
    }
    void addPlane(int speed) {
        Plane tmp = Plane(this->length, this->heigth, speed);
        this->planes.push_back(tmp);
        this->numberOfPlanes++;
    }
    void run() {

        for (Vehicle& veh : this->cars) {
            vecOfThreads.push_back(std::move(std::thread([&](Vehicle* vehicle) {vehicle->run(); }, &veh)));
            this->numberOfThreads++;
        }
        for (Vehicle& veh : this->ships) {
            vecOfThreads.push_back(std::move(std::thread([&](Vehicle* vehicle) {vehicle->run(); }, &veh)));
            this->numberOfThreads++;
        }
        for (Vehicle& veh : this->planes) {
            vecOfThreads.push_back(std::move(std::thread([&](Vehicle* vehicle) {vehicle->run(); }, &veh)));
            this->numberOfThreads++;
        }

        WSADATA wsaData;

        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != NO_ERROR)
            printf("Initialization error.\n");

        SOCKET mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (mainSocket == INVALID_SOCKET)
        {
            printf("Error creating socket: %ld\n", WSAGetLastError());
            WSACleanup();
        }

        sockaddr_in service;
        memset(&service, 0, sizeof(service));
        service.sin_family = AF_INET;
        service.sin_port = htons(9002);

        if (bind(mainSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
        {
            printf("bind() failed.\n");
            closesocket(mainSocket);
        }
        if (listen(mainSocket, 1) == SOCKET_ERROR)
            printf("Error listening on socket.\n");

        SOCKET acceptSocket = SOCKET_ERROR;
        printf("Waiting for a client to connect...\n");

        while (acceptSocket == SOCKET_ERROR)
        {
            acceptSocket = accept(mainSocket, NULL, NULL);
        }

        printf("Client connected.\n");
        mainSocket = acceptSocket;

        int bytesSent;
        int bytesRecv = SOCKET_ERROR;
        char sendbuf[64] = "Server says hello!\n";
        char recvbuf[64] = "";

        bytesRecv = recv(mainSocket, recvbuf, 64, 0);
        printf("Bytes received: %ld\n", bytesRecv);
        printf("Received text: %s\n", recvbuf);

        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            for (Vehicle& veh : this->cars) {
                char buffer[64];
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                sprintf_s(buffer, "{\"id\":%d,\"x_pos\":%d,\"y_pos\":%d}", veh.giveID(), veh.giveX(), veh.giveY());
                bytesSent = send(mainSocket, buffer, strlen(buffer), 0);
            }
            for (Vehicle& veh : this->planes) {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                char buffer[64];
                sprintf_s(buffer, "{\"id\":%d,\"x_pos\":%d,\"y_pos\":%d}", veh.giveID(), veh.giveX(), veh.giveY());
                bytesSent = send(mainSocket, buffer, strlen(buffer), 0);
            }
            for (Vehicle& veh : this->ships) {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                char buffer[64];
                sprintf_s(buffer, "{\"id\":%d,\"x_pos\":%d,\"y_pos\":%d}", veh.giveID(), veh.giveX(), veh.giveY());
                bytesSent = send(mainSocket, buffer, strlen(buffer), 0);
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
    const int LEN = 90;
    const int HEI = 180;
    Map mapa = Map(LEN, HEI);
    mapa.toString();


    mapa.addCar(10);
    mapa.addCar(10);
    mapa.addCar(3);
    mapa.addCar(1);
    mapa.addCar(1);
    mapa.addPlane(10);
    mapa.addPlane(3);
    mapa.addPlane(12);
    mapa.addShip(4);
    mapa.addShip(3);
    mapa.addShip(2);
    mapa.run();



}