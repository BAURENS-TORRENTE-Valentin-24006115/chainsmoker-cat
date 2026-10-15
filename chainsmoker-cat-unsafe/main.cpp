/**
 * @authors Martin Demange, Aline Dumont
 * @brief C++ source file for a solution to Cigarette Smoker problem, NOTE: this solution is purposefully wrong, it will make a deadlock.
 */

#include <iostream>
#include <thread>
#include <semaphore>

using namespace std;

// Peter parker is the agent
binary_semaphore peterParker{ 1 };
//represent the available resources
binary_semaphore spiderWeb{ 0 };
binary_semaphore suit{ 0 };
binary_semaphore spiderBot{ 0 };

/**
 * @brief Logic for the agent.
 * Function that represents the logic of the agent. The agent will generate a random number (between 0 and 2 included),
 * this number represents the ingredients that the smokers will need. This number is stored in the local var randInt.
 * Depending on the value of randInt, the agent will release the semaphore associated with the disposable ingredients..
 * @return void
 */
void peterParkerLogic() {
    int randInt;
    while(true){
        peterParker.acquire();

        randInt = rand() % 3;

        // The agent has : spider web and spider bot
        if (randInt == 0) {
            cout << "Peter Parker met à disposition des toiles et des spider-bots." << endl;
            spiderWeb.release();
            spiderBot.release();
        }
        // The agent has : spider bot and a suit
        else if (randInt == 1) {
            cout << "Peter Parker met à disposition des spider-bots et des tenues." << endl;
            spiderBot.release();
            suit.release();
        }
        // The agent has : spider web and a suit
        else if (randInt == 2) {
            cout << "Peter Parker met à disposition des toiles et des tenues." << endl;
            spiderWeb.release();
            suit.release();
        }
    }
}

/**
 * @brief Logic for one that need a suit and spider web (Gwen).
 * Function that represent the logic of the one that need a suit and spider web (Gwen). Gwen wait that the semaphores for
 * the resources are avaible and if they are, print that they are used and that she is going to a mission.
 * @return void
 */
void spiderWebLogic() {
    while (true) {
        suit.acquire();
        spiderBot.acquire();

        cout << "Spider-Gwen a ses toiles et recupere sa tenue et son spider-bot." << endl;
        cout << "Elle part en mission..." << endl;
        peterParker.release();
    }
}

/**
 * @brief Logic for the one that need spider web and spider bot (Spider-Punk).
 * Function that represent the logic of the one that need spider web and spider bot (Spider-Punk). Spider-Punk wait that the semaphores for
 * the resources are avaible and if they are, print that they are used and that he is going to a mission.
 * @return void
 */
void suitLogic() {
    while (true) {
        spiderWeb.acquire();
        spiderBot.acquire();

        cout << "Spider-Punk a sa tenue et recupere ses toiles et son spider-bot." << endl;
        cout << "Il part en mission..." << endl;

        peterParker.release();
    }
}

/**
 * @brief Logic for the one that need spider web and a suit (Miller Moralles).
 * Function that represent the logic of the one that need spider web and a suit (Miller Morales). Milles Moralles wait that the semaphores for
 * the resources are avaible and if they are, print that they are used and that he is going to a mission.
 * @return void
 */
void spiderBotLogic() {
    while (true) {
        spiderWeb.acquire();
        suit.acquire();

        cout << "Milles Moralles a son spider-bot et recupere ses toiles et sa tenue." << endl;
        cout << "Il part en mission..." << endl;

        peterParker.release();
    }
}

int main()
{
    cout << "--- Debut de la simulation ---" << endl;

    /* init the threads */
    thread thSpiderMan1(spiderWebLogic);
    thread thSpiderMan2(suitLogic);
    thread thSpiderMan3(spiderBotLogic);
    thread thPeterParker(peterParkerLogic);

    // start the end processus of the programmn by waiting that the agent thread finish his execution
    thPeterParker.join();
    // detach these threads from the thread handle, they can now execute indenpendly, any resources allocated are freed after the threads end.
    thSpiderMan1.detach();
    thSpiderMan2.detach();
    thSpiderMan3.detach();

    cout << endl << "--- Fin de la simulation ---" << endl;
    return 0;
}
