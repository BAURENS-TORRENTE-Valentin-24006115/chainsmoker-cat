/**
 * @authors Martin Demange, Aline Dumont
 * @brief C++ source file for a solution to Cigarette Smoker problem, NOTE: this solution is purposefully wrong, it will make a deadlock.
 */

#include <iostream>
#include <thread>
#include <semaphore>

using namespace std;

binary_semaphore agentSem{ 1 };
//represent the available ingredients
binary_semaphore tabacco{ 0 };
binary_semaphore paper{ 0 };
binary_semaphore match{ 0 };

// binary_semaphore tabaccoSmokerSem{ 0 }; // Missing paper and matches
// binary_semaphore paperSmokerSem{ 0 }; // Missing tabacco and matches
// binary_semaphore matchSmokerSem{ 0 }; // Missing paper and tabacco

/**
 * @brief Logic for the agent.
 * Function that represents the logic of the agent. The agent will generate a random number (between 0 and 2 included),
 * this number represents the ingredients that the smokers will need. This number is stored in the local var randInt.
 * Depending on the value of randInt, the agent will release the semaphore associated with the disposable ingredients..
 * @return void
 */
void agentLogic() {
    int randInt;
    while(true){
        agentSem.acquire();

        randInt = rand() % 3;

        // The agent has : tabacco and matches
        if (randInt == 0) {
            cout << "L'agent mets du tabac et des alumettes sur la table" << endl;
            tabacco.release();
            match.release();
        }
        // The agent has : paper and matches
        else if (randInt == 1) {
            cout << "L'agent mets du papier et des alumettes sur la table" << endl;
            match.release();
            paper.release();
        }
        // The agent has : tabacco and paper
        else if (randInt == 2) {
            cout << "L'agent mets du tabac et du papier sur la table" << endl;
            tabacco.release();
            paper.release();
        }
    }
}

/**
 * @brief Logic for the smoker that need paper and tabbaco.
 * Function that represent the logic of the smoker that need matches and paper. The smoker wait that the semaphores for
 * the ingredients are disponible and if they are, print that they are used and that the smoker is smoking.
 */
void tabaccoSmokerLogic() {
    while (true) {
        paper.acquire();
        match.acquire();

        cout << "Le fumeur qui a le tabac prend les alumettes et le papier" << endl;
        cout << "Il fume..." << endl;
        agentSem.release();
    }
}

/**
 * @brief Logic for the smoker that need matches and tabacco.
 * Function that represent the logic of the smoker that need matches and tabacco. The smoker wait that the semaphores for
 * the ingredients are disponible and if they are, print that they are used and that the smoker is smoking.
 */
void paperSmokerLogic() {
    while (true) {
        tabacco.acquire();
        match.acquire();

        cout << "Le fumeur qui a le papier prend les alumettes et le tabac" << endl;
        cout << "Il fume..." << endl;

        agentSem.release();
    }
}

/**
 * @brief Logic for the smoker that need tabacco and paper.
 * Function that represent the logic of the smoker that need tabacco and paper. The smoker wait that the semaphores for
 * the ingredients are disponible and if they are, print that they are used and that the smoker is smoking.
 */
void matchSmokerLogic() {
    while (true) {
        tabacco.acquire();
        paper.acquire();

        cout << "Le fumeur qui a les allumettes prend le tabac et le papier" << endl;
        cout << "Il fume..." << endl;

        agentSem.release();
    }
}

int main()
{
    cout << "--- Debut de la simulation ---" << endl;

    /* init the threads */
    thread thSmoker1(tabaccoSmokerLogic);
    thread thSmoker2(paperSmokerLogic);
    thread thSmoker3(matchSmokerLogic);
    thread thAgent(agentLogic);

    // start the end processus of the programmn by waiting that the agent thread finish his execution
    thAgent.join();
    // detach these threads from the thread handle, they can now execute indenpendly, any resources used are freed after the threads end.
    thSmoker1.detach();
    thSmoker2.detach();
    thSmoker3.detach();

    cout << endl << "--- Fin de la simulation ---" << endl;
    return 0;
}
