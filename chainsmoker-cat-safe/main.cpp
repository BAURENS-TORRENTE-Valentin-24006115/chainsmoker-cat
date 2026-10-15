/**
 * @authors Baurens-Torrente Valentin
 * @brief C++ source file for a solution to Cigarette Smoker problem, NOTE: this solution is purposefully wrong, it will make a deadlock.
 */
#include <iostream>
#include <thread>
#include <semaphore>
#include <cstdlib>
#include <chrono>
#include <mutex>

using namespace std;

binary_semaphore peterParker{ 1 };
// represent the available resources
binary_semaphore spiderWeb { 0 };
binary_semaphore suit{ 0 };
binary_semaphore spiderBot{ 0 };

binary_semaphore spiderWebGwen{ 0 }; // Missing suit and spiderBots
binary_semaphore suitSpiderPunk{ 0 }; // Missing spiderWeb and spiderBots
binary_semaphore spiderBotMilles{ 0 }; // Missing suit and spiderWeb
// bool used by the pusher
bool is_SpiderWeb = false;
bool is_Suit = false;
bool is_SpiderBot = false;
mutex pusher_mutex;

// Peter Parker Thread
/**
 * @brief Logic for the agent.
 * Function that represents the logic of the agent. The agent will generate a random number (between 0 and 2 included),
 * this number represents the ingredients that the smokers will need. This number is stored in the local var randInt.
 * Depending on the value of randInt, the agent will release the semaphore associated with the disposable ingredients..
 * @return void
 */
void peterParkerLogic() {
    int randInt;
    for (int i = 0; i < 10; ++i) {
        peterParker.acquire();

        randInt = rand() % 3;

        if (randInt == 0) {
            cout << "Peter Parker met à disposition des toiles et des spider-bots" << endl;
            spiderWeb.release();
            spiderBot.release();
        }
        else if (randInt == 1) {
            cout << "Peter Parker met à disposition des spider-bots et des tenues." << endl;
            spiderBot.release();
            suit.release();
        }
        else if (randInt == 2) {
            cout << "Peter Parker met à disposition des toiles et des tenues." << endl;
            spiderWeb.release();
            suit.release();
        }
    }
}

// Pusher Thread
/**
 * @brief Pusher logic for the spiderWeb resource.
 * Pusher logic for the spiderWeb resource. This pusher checks if his resources and another one are available.
 * If this is the case: release the semaphore of the corresponding character or just turn its corresponding boolean to true.
 * @return void
 */
void spiderWeb_pusher() {
    while (true) {
        spiderWeb.acquire();

        lock_guard<mutex> lock(pusher_mutex);
        if (is_Suit) {
            is_Suit = false;
            spiderBotMilles.release(); // We have spiderWeb and suit, so we call the one with spiderBot
        }
        else if (is_SpiderBot) {
            is_SpiderBot = false;
            suitSpiderPunk.release(); // We have spiderWeb and spiderBot, so we call the one with suit
        }
        else {
            is_SpiderWeb = true;
        }
    }
}

/**
 * @brief Pusher logic for the suit resource.
 * Pusher logic for the suit resource. This pusher checks if his resources and another one are available.
 * If this is the case: release the semaphore of the corresponding character or just turn its corresponding boolean to true.
 * @return void
 */
void suit_pusher() {
    while (true) {
        suit.acquire();

        lock_guard<mutex> lock(pusher_mutex);
        if (is_SpiderWeb) {
            is_SpiderWeb = false;
            spiderBotMilles.release(); // We have spiderWeb and suit, so we call the one with spiderBot
        }
        else if (is_SpiderBot) {
            is_SpiderBot = false;
            spiderWebGwen.release(); // We have suit and spiderBot, so we call the one with spiderWeb
        }
        else {
            is_Suit = true;
        }
    }
}

/**
 * @brief Pusher logic for the spiderBot resource.
 * Pusher logic for the spiderBot resource. This pusher checks if his resources and another one are available.
 * If this is the case: release the semaphore of the corresponding character or just turn its corresponding boolean to true.
 * @return void
 */
void spiderBot_pusher() {
    while (true) {
        spiderBot.acquire();

        lock_guard<mutex> lock(pusher_mutex);
        if (is_Suit) {
            is_Suit = false;
            spiderWebGwen.release(); // We have spiderBot and suit, so we call the one with spiderWeb
        }
        else if (is_SpiderWeb) {
            is_SpiderWeb = false;
            suitSpiderPunk.release(); // We have spiderWeb and spiderBot, so we call the one with suit
        }
        else {
            is_SpiderBot = true;
        }
    }
}

// Smoker Thread

/**
 * @brief Logic for one that need a suit and spider web (Gwen).
 * Function that represent the logic of the one that need a suit and spider web (Gwen). Gwen wait that the semaphores for
 * the resources are avaible and if they are, print that they are used and that she is going to a mission.
 */
void spiderWebLogic() {
    while (true) {
        spiderWebGwen.acquire();
        cout << "Spider-Gwen a ses toiles et recupere sa tenue et son spider-bot." << endl;
        cout << "Elle part en mission";
        for (int i = 0; i < 3; ++i)
        {
            cout << ".";
            this_thread::sleep_for(chrono::milliseconds(750));
        }
        cout << endl;
        peterParker.release();
    }
}

/**
 * @brief Logic for the one that need spider web and spider bot (Spider-Punk).
 * Function that represent the logic of the one that need spider web and spider bot (Spider-Punk). Spider-Punk wait that the semaphores for
 * the resources are avaible and if they are, print that they are used and that he is going to a mission.
 */
void suitLogic() {
    while (true) {
        suitSpiderPunk.acquire();

        cout << "Spider-Punk a sa tenue et recupere ses toiles et son spider-bot." << endl;
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

/**
 * @brief Logic for the one that need spider web and a suit (Miller Moralles).
 * Function that represent the logic of the one that need spider web and a suit (Miller Morales). Milles Moralles wait that the semaphores for
 * the resources are avaible and if they are, print that they are used and that he is going to a mission.
 */
void spiderBotLogic() {
    while (true) {
        spiderBotMilles.acquire();

        cout << "Milles Moralles a son spider-bot et recupere ses toiles et sa tenue." << endl;
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

    // start the end processus of the programmn by waiting that the agent thread finish his execution
    thPeterParker.join();

    // detach these threads from the thread handle, they can now execute indenpendly, any resources allocated are freed after the threads end.
    thSpiderMan1.detach();
    thSpiderMan2.detach();
    thSpiderMan3.detach();

    thPusher1.detach();
    thPusher2.detach();
    thPusher3.detach();

    cout << endl << "--- Fin de la simulation ---" << endl;
    return 0;
}
