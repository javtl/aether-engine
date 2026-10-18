#pragma once

#include <functional>
#include <string_view>

namespace Aether
{

    enum class State
    {
        IDLE,
        ARMED,
        SPINNING,
        EVALUATING,
        PAYOUT,
        ERROR
    };

    enum class Event
    {
        INSERT_CREDIT,
        SPIN_REQUESTED,
        SPIN_COMPLETE,
        EVALUATION_COMPLETE,
        PAYOUT_COMPLETE,
        TRIGGER_ERROR,
        RESET
    };

    class GameFSM
    {
    public:
        using StateChangeCallback = std::function<void(State, State)>;

        explicit GameFSM(StateChangeCallback callback = {}) noexcept;

        [[nodiscard]] State getCurrentState() const noexcept;
        [[nodiscard]] bool handleEvent(Event event) noexcept;

        void setOnStateChangeCallback(StateChangeCallback callback) noexcept;

        [[nodiscard]] static constexpr std::string_view stateToString(State state) noexcept
        {
            switch (state)
            {
            case State::IDLE:
                return "IDLE";
            case State::ARMED:
                return "ARMED";
            case State::SPINNING:
                return "SPINNING";
            case State::EVALUATING:
                return "EVALUATING";
            case State::PAYOUT:
                return "PAYOUT";
            case State::ERROR:
                return "ERROR";
            }
            return "UNKNOWN";
        }

        [[nodiscard]] static constexpr std::string_view eventToString(Event event) noexcept
        {
            switch (event)
            {
            case Event::INSERT_CREDIT:
                return "INSERT_CREDIT";
            case Event::SPIN_REQUESTED:
                return "SPIN_REQUESTED";
            case Event::SPIN_COMPLETE:
                return "SPIN_COMPLETE";
            case Event::EVALUATION_COMPLETE:
                return "EVALUATION_COMPLETE";
            case Event::PAYOUT_COMPLETE:
                return "PAYOUT_COMPLETE";
            case Event::TRIGGER_ERROR:
                return "TRIGGER_ERROR";
            case Event::RESET:
                return "RESET";
            }
            return "UNKNOWN";
        }

    private:
        [[nodiscard]] bool isValidTransition(State from, Event event, State &to) const noexcept;
        void notifyCallback(State oldState, State newState) noexcept;

        State m_currentState;
        StateChangeCallback m_callback;
    };

} // namespace Aether