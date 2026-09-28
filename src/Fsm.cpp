#include "Fsm.hpp"
#include <iostream>

namespace Aether
{

    /**
     * CONSTRUCTOR
     * ============
     * Inicializa la FSM con un estado inicial (por defecto IDLE).
     */
    GameFSM::GameFSM(State initialState)
        : m_currentState(initialState), m_onStateChange(nullptr) {}

    /**
     * HANDLE EVENT
     * ============
     * Este es el método más importante. Aquí ocurre la "magia":
     * 1. Alguien envía un evento (ej: INSERT_CREDIT)
     * 2. Miramos si ese evento es válido en el estado actual
     * 3. Si es válido, cambiamos de estado
     * 4. Si no es válido, rechazamos con un error
     */
    bool GameFSM::handleEvent(Event event)
    {
        // std::lock_guard = Cierra la puerta mientras procesamos
        // Así, si otro hilo intenta acceder, espera a que terminemos
        std::lock_guard<std::mutex> lock(m_stateMutex);

        // Intentar encontrar el siguiente estado válido
        State nextState = m_currentState;
        if (!isValidTransition(m_currentState, event, nextState))
        {
            // ❌ Transición RECHAZADA
            std::cerr << "[FSM ERROR] Transicion invalida desde estado '"
                      << stateToString(m_currentState)
                      << "' con evento '" << eventToString(event) << "'\n";
            return false;
        }

        // ✅ Transición ACEPTADA
        State previousState = m_currentState;
        m_currentState = nextState;

        // Ejecutar el callback si está registrado
        // (Ej: imprimir en pantalla que cambió)
        if (m_onStateChange)
        {
            m_onStateChange(previousState, m_currentState);
        }

        return true;
    }

    /**
     * GET CURRENT STATE
     * =================
     * Obtiene el estado actual de forma SEGURA (usando mutex).
     *
     * "mutable" significa que este método puede modificar m_stateMutex
     * aunque el método sea const (es una excepción especial).
     */
    State GameFSM::getCurrentState() const
    {
        std::lock_guard<std::mutex> lock(m_stateMutex);
        return m_currentState;
    }

    /**
     * IS VALID TRANSITION
     * ===================
     * La lógica central: ¿Qué transiciones están permitidas?
     *
     * Las reglas de negocio están aquí. Por ejemplo:
     * "No puedo girar la máquina si no tengo dinero dentro"
     * "No puedo pagar si la máquina no ha terminado de girar"
     */
    bool GameFSM::isValidTransition(State from, Event event, State &to) const noexcept
    {
        // Caso especial: Si algo explota, ir a ERROR desde cualquier lado
        if (event == Event::TRIGGER_ERROR)
        {
            to = State::ERROR;
            return true;
        }

        // Caso especial: Desde ERROR, solo podemos volver a IDLE con RESET
        if (from == State::ERROR && event == Event::RESET)
        {
            to = State::IDLE;
            return true;
        }

        // Ahora, la máquina de estado real:
        // "Desde el estado X, con el evento Y, vamos al estado Z"
        switch (from)
        {
        case State::IDLE:
            // Desde IDLE solo aceptamos dinero
            if (event == Event::INSERT_CREDIT)
            {
                to = State::ARMED;
                return true;
            }
            break;

        case State::ARMED:
            // Desde ARMED solo aceptamos solicitud de spin
            if (event == Event::SPIN_REQUESTED)
            {
                to = State::SPINNING;
                return true;
            }
            break;

        case State::SPINNING:
            // Desde SPINNING solo aceptamos "terminó de girar"
            if (event == Event::SPIN_COMPLETE)
            {
                to = State::EVALUATING;
                return true;
            }
            break;

        case State::EVALUATING:
            // Desde EVALUATING solo aceptamos "evalué el resultado"
            if (event == Event::EVALUATION_COMPLETE)
            {
                to = State::PAYOUT;
                return true;
            }
            break;

        case State::PAYOUT:
            // Desde PAYOUT volvemos a IDLE
            if (event == Event::PAYOUT_COMPLETE)
            {
                to = State::IDLE;
                return true;
            }
            break;

        case State::ERROR:
            // Manejado arriba con el RESET
            break;
        }

        // Si llegamos aquí, la transición NO es válida
        return false;
    }

    /**
     * STATE TO STRING
     * ===============
     * Convierte un enum de estado a texto legible.
     *
     * constexpr = se calcula en tiempo de compilación si es posible
     * string_view = no aloca memoria, es muy eficiente
     */
    constexpr std::string_view GameFSM::stateToString(State state) noexcept
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

    /**
     * EVENT TO STRING
     * ===============
     * Mismo concepto que stateToString pero para eventos.
     */
    constexpr std::string_view GameFSM::eventToString(Event event) noexcept
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

    /**
     * SET ON STATE CHANGE CALLBACK
     * ============================
     * Permite registrar una función que se ejecute cada vez
     * que la máquina cambia de estado.
     *
     * Ejemplo de uso:
     *   fsm->setOnStateChangeCallback([](State old, State nu) {
     *       std::cout << "De " << old << " a " << nu << "\n";
     *   });
     */
    void GameFSM::setOnStateChangeCallback(StateChangeCallback callback)
    {
        std::lock_guard<std::mutex> lock(m_stateMutex);
        m_onStateChange = std::move(callback);
    }

} // namespace Aether