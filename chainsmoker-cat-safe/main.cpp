/**
 * @authors Baurens-Torrente Valentin
 */
#include <iostream>
#include <thread>
#include <semaphore>
#include <cstdlib>
#include <chrono>
#include <mutex>

using namespace std;


binary_semaphore peterParker{ 1 };
binary_semaphore spiderWeb { 0 };
binary_semaphore suit{ 0 };
binary_semaphore spiderBot{ 0 };

binary_semaphore spiderWebGwen{ 0 }; // Missing suit and spiderBots
binary_semaphore suitSpiderPunk{ 0 }; // Missing spiderWeb and spiderBots
binary_semaphore spiderBotMilles{ 0 }; // Missing suit and spiderWeb

bool is_SpiderWeb = false;
bool is_Suit = false;
bool is_SpiderBot = false;
mutex pusher_mutex;

// Peter Parker Thread

void peterParkerLogic() {
    int randInt;
    for (int i = 0; i < 10; ++i) {
        peterParker.acquire();

        randInt = rand() % 3;

        if (randInt == 0) {
            cout << "Peter Parker met a disposition des toilles et des spider bots" << endl;
            spiderWeb.release();
            spiderBot.release();
        }
        else if (randInt == 1) {
            cout << "Peter Parker met a disposition des spider bots et une tenue" << endl;
            spiderBot.release();
            suit.release();
        }
        else if (randInt == 2) {
            cout << "Peter Parker met a disposition des toilles et une tenue" << endl;
            spiderWeb.release();
            suit.release();
        }
    }
}

// Pusher Thread

void spiderWeb_pusher() {
    while (true) {
        spiderWeb.acquire();

        lock_guard<mutex> lock(pusher_mutex);
        if (is_Suit) {
            is_Suit = false;
            spiderBotMilles.release(); // We have spiderWeb and suit, so we call the smoker with spiderBot
        }
        else if (is_SpiderBot) {
            is_SpiderBot = false;
            suitSpiderPunk.release(); // We have spiderWeb and spiderBot, so we call the smoker with suit
        }
        else {
            is_SpiderWeb = true;
        }
    }
}

void suit_pusher() {
    while (true) {
        suit.acquire();

        lock_guard<mutex> lock(pusher_mutex);
        if (is_SpiderWeb) {
            is_SpiderWeb = false;
            spiderBotMilles.release(); // We have spiderWeb and suit, so we call the smoker with spiderBot
        }
        else if (is_SpiderBot) {
            is_SpiderBot = false;
            spiderWebGwen.release(); // We have suit and spiderBot, so we call the smoker with spiderWeb
        }
        else {
            is_Suit = true;
        }
    }
}

void spiderBot_pusher() {
    while (true) {
        spiderBot.acquire();

        lock_guard<mutex> lock(pusher_mutex);
        if (is_Suit) {
            is_Suit = false;
            spiderWebGwen.release(); // We have spiderBot and suit, so we call the smoker with spiderWeb
        }
        else if (is_SpiderWeb) {
            is_SpiderWeb = false;
            suitSpiderPunk.release(); // We have spiderWeb and spiderBot, so we call the smoker with suit
        }
        else {
            is_SpiderBot = true;
        }
    }
}

// Smoker Thread

void spiderWebLogic() {
    while (true) {
        spiderWebGwen.acquire();
        cout << "Spider-Gwen a ses toilles et recupere sa tenue et sont spider bot" << endl;
        cout << "Il part en mission";
        for (int i = 0; i < 3; ++i)
        {
            cout << ".";
            this_thread::sleep_for(chrono::milliseconds(750));
        }
        cout << endl;
        peterParker.release();
    }
}

void suitLogic() {
    while (true) {
        suitSpiderPunk.acquire();

        cout << "Spider-Punk qui a sa tenue et recupere ses toilles et son spider bot" << endl;
        cout << "Il part en mission";
        for (int i = 0; i < 3; ++i)
        {
            cout << ".";
            this_thread::sleep_for(chrono::milliseconds(750));
        }
        cout << endl;

        peterParker.release();
    }
}

void spiderBotLogic() {
    while (true) {
        spiderBotMilles.acquire();

        cout << "Milles Moralles qui a son spider bot et recupere ses toilles et sa tenue" << endl;
        cout << "Il part en mission";
        for (int i = 0; i < 3; ++i)
        {
            cout << ".";
            this_thread::sleep_for(chrono::milliseconds(750));
        }
        cout << endl;

        peterParker.release();
    }
}

int main() {
    cout << "--- Debut de la simulation ---" << endl;

    /* Thread */
    thread thSpiderMan1(spiderWebLogic);
    thread thSpiderMan2(suitLogic);
    thread thSpiderMan3(spiderBotLogic);

    thread thPusher1(spiderWeb_pusher);
    thread thPusher2(suit_pusher);
    thread thPusher3(spiderBot_pusher);

    thread thPeterParker(peterParkerLogic);

    thPeterParker.join();

    thSpiderMan1.detach();
    thSpiderMan2.detach();
    thSpiderMan3.detach();

    thPusher1.detach();
    thPusher2.detach();
    thPusher3.detach();

    cout << endl << "--- Fin de la simulation ---" << endl;
    return 0;
}
