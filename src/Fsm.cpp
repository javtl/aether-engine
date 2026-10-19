#include "Fsm.hpp"
#include <utility>

namespace Aether
{

    GameFSM::GameFSM(StateChangeCallback callback) noexcept
        : m_currentState(State::IDLE),
          m_callback(std::move(callback)) {}

    State GameFSM::getCurrentState() const noexcept
    {
        return m_currentState;
    }

    bool GameFSM::handleEvent(Event event) noexcept
    {
        if (m_currentState == State::ERROR)
        {
            if (event == Event::RESET)
            {
                notifyCallback(State::ERROR, State::IDLE);
                m_currentState = State::IDLE;
                return true;
            }
            return false;
        }

        if (event == Event::TRIGGER_ERROR)
        {
            notifyCallback(m_currentState, State::ERROR);
            m_currentState = State::ERROR;
            return true;
        }

        State nextState = m_currentState;
        if (!isValidTransition(m_currentState, event, nextState))
        {
            return false;
        }

        notifyCallback(m_currentState, nextState);
        m_currentState = nextState;
        return true;
    }

    bool GameFSM::isValidTransition(State from, Event event, State &to) const noexcept
    {
        switch (from)
        {
        case State::IDLE:
            if (event == Event::INSERT_CREDIT)
            {
                to = State::ARMED;
                return true;
            }
            break;
        case State::ARMED:
            if (event == Event::SPIN_REQUESTED)
            {
                to = State::SPINNING;
                return true;
            }
            break;
        case State::SPINNING:
            if (event == Event::SPIN_COMPLETE)
            {
                to = State::EVALUATING;
                return true;
            }
            break;
        case State::EVALUATING:
            if (event == Event::EVALUATION_COMPLETE)
            {
                to = State::PAYOUT;
                return true;
            }
            break;
        case State::PAYOUT:
            if (event == Event::PAYOUT_COMPLETE)
            {
                to = State::IDLE;
                return true;
            }
            break;
        case State::ERROR:
            break;
        }
        return false;
    }

    void GameFSM::notifyCallback(State oldState, State newState) noexcept
    {
        if (m_callback)
        {
            m_callback(oldState, newState);
        }
    }

    void GameFSM::setOnStateChangeCallback(StateChangeCallback callback) noexcept
    {
        m_callback = std::move(callback);
    }

} // namespace Aether