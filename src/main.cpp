#include <iostream>
#include "Fsm.hpp"

int main()
{
    Aether::GameFSM fsm(
        [](Aether::State oldState, Aether::State newState)
        {
            std::cout << Aether::GameFSM::stateToString(oldState)
                      << " -> " << Aether::GameFSM::stateToString(newState) << '\n';
        });

    bool result = true;
    result = fsm.handleEvent(Aether::Event::INSERT_CREDIT);
    result = fsm.handleEvent(Aether::Event::SPIN_REQUESTED);
    result = fsm.handleEvent(Aether::Event::SPIN_COMPLETE);
    result = fsm.handleEvent(Aether::Event::EVALUATION_COMPLETE);
    result = fsm.handleEvent(Aether::Event::PAYOUT_COMPLETE);

    return result ? 0 : 1;
}