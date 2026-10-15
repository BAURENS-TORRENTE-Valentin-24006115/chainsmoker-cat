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

binary_semaphore agentSem{ 1 };
binary_semaphore tabacco{ 0 };
binary_semaphore paper{ 0 };
binary_semaphore match{ 0 };

binary_semaphore tabaccoSmokerSem{ 0 }; // Missing paper and matches
binary_semaphore paperSmokerSem{ 0 }; // Missing tabacco and matches
binary_semaphore matchSmokerSem{ 0 }; // Missing paper and tabacco

bool is_tabacco = false;
bool is_paper = false;
bool is_match = false;
mutex pusher_mutex;

// Agent Thread

void agentLogic() {
    int randInt;
    for (int i = 0; i < 10; ++i) {
        agentSem.acquire();

        randInt = rand() % 3;

        if (randInt == 0) {
            cout << "L'agent mets du tabac et des alumettes sur la table" << endl;
            tabacco.release();
            match.release();
        }
        else if (randInt == 1) {
            cout << "L'agent mets du papier et des alumettes sur la table" << endl;
            match.release();
            paper.release();
        }
        else if (randInt == 2) {
            cout << "L'agent mets du tabac et du papier sur la table" << endl;
            tabacco.release();
            paper.release();
        }
    }
}

// Pusher Thread

void tabacco_pusher() {
    while (true) {
        tabacco.acquire();

        lock_guard<mutex> lock(pusher_mutex);
        if (is_paper) {
            is_paper = false;
            matchSmokerSem.release(); // We have tabacco and paper, so we call the smoker with match
        }
        else if (is_match) {
            is_match = false;
            paperSmokerSem.release(); // We have tabacco and match, so we call the smoker with paper
        }
        else {
            is_tabacco = true;
        }
    }
}

void paper_pusher() {
    while (true) {
        paper.acquire();

        lock_guard<mutex> lock(pusher_mutex);
        if (is_tabacco) {
            is_tabacco = false;
            matchSmokerSem.release(); // We have tabacco and paper, so we call the smoker with match
        }
        else if (is_match) {
            is_match = false;
            tabaccoSmokerSem.release(); // We have paper and match, so we call the smoker with tabacco
        }
        else {
            is_paper = true;
        }
    }
}

void match_pusher() {
    while (true) {
        match.acquire();

        lock_guard<mutex> lock(pusher_mutex);
        if (is_paper) {
            is_paper = false;
            tabaccoSmokerSem.release(); // We have match and paper, so we call the smoker with tabacco
        }
        else if (is_tabacco) {
            is_tabacco = false;
            paperSmokerSem.release(); // We have tabacco and match, so we call the smoker with paper
        }
        else {
            is_match = true;
        }
    }
}

// Smoker Thread

void tabaccoSmokerLogic() {
    while (true) {
        tabaccoSmokerSem.acquire();
        cout << "Le fumeur qui a le tabac prend les alumettes et le papier" << endl;
        cout << "Il fume";
        for (int i = 0; i < 3; ++i)
        {
            cout << ".";
            this_thread::sleep_for(chrono::milliseconds(750));
        }
        cout << endl;
        agentSem.release();
    }
}

void paperSmokerLogic() {
    while (true) {
        paperSmokerSem.acquire();

        cout << "Le fumeur qui a le papier prend les alumettes et le tabac" << endl;
        cout << "Il fume";
        for (int i = 0; i < 3; ++i)
        {
            cout << ".";
            this_thread::sleep_for(chrono::milliseconds(750));
        }
        cout << endl;

        agentSem.release();
    }
}

void matchSmokerLogic() {
    while (true) {
        matchSmokerSem.acquire();

        cout << "Le fumeur qui a les allumettes prend le tabac et le papier" << endl;
        cout << "Il fume";
        for (int i = 0; i < 3; ++i)
        {
            cout << ".";
            this_thread::sleep_for(chrono::milliseconds(750));
        }
        cout << endl;

        agentSem.release();
    }
}

int main() {
    cout << "--- Debut de la simulation ---" << endl;

    /* Thread */
    thread thSmoker1(tabaccoSmokerLogic);
    thread thSmoker2(paperSmokerLogic);
    thread thSmoker3(matchSmokerLogic);

    thread thPusher1(tabacco_pusher);
    thread thPusher2(paper_pusher);
    thread thPusher3(match_pusher);

    thread thAgent(agentLogic);

    thAgent.join();

    thSmoker1.detach();
    thSmoker2.detach();
    thSmoker3.detach();

    thPusher1.detach();
    thPusher2.detach();
    thPusher3.detach();

    cout << endl << "--- Fin de la simulation ---" << endl;
    return 0;
}