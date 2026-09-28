#ifndef AETHER_ENGINE_FSM_HPP
#define AETHER_ENGINE_FSM_HPP

#include <string_view>
#include <mutex>
#include <functional>

namespace Aether
{

    /**
     * @brief Estados posibles de la máquina de juego.
     *
     * Piensa en estos como los "modos" en que puede estar la máquina:
     * - IDLE: Durmiendo, esperando dinero
     * - ARMED: Tiene dinero, listo para que pulses el botón
     * - SPINNING: Calculando resultado (los "rodillos" girando)
     * - EVALUATING: Mirando si ganaste
     * - PAYOUT: Dando dinero si ganaste
     * - ERROR: Algo explotó (puerta abierta, sensores rotos)
     */
    enum class State
    {
        IDLE,
        ARMED,
        SPINNING,
        EVALUATING,
        PAYOUT,
        ERROR
    };

    /**
     * @brief Eventos que hacen cambiar de estado.
     *
     * Un evento es "algo que pasó" que puede cambiar el estado:
     * - Alguien metió una moneda → INSERT_CREDIT
     * - Alguien pulsó el botón → SPIN_REQUESTED
     * - La máquina terminó de girar → SPIN_COMPLETE
     * - El sensor se rompió → TRIGGER_ERROR
     */
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

    /**
     * @brief Máquina de Estados Finitos (FSM) thread-safe.
     *
     * Esta clase es el corazón de la máquina. Gestiona:
     * 1. El estado actual (IDLE, ARMED, etc)
     * 2. Las transiciones (cambios de estado)
     * 3. Validación (no permite transiciones ilegales)
     * 4. Callbacks (notificar cuando cambia de estado)
     */
    class GameFSM
    {
    public:
        // Tipo para la función que se ejecuta cuando cambia el estado
        using StateChangeCallback = std::function<void(State oldState, State newState)>;

        /**
         * @brief Constructor. Inicializa la FSM en estado IDLE.
         */
        explicit GameFSM(State initialState = State::IDLE);

        // Destructor (la máquina se limpia sola cuando termina)
        ~GameFSM() = default;

        // Deshabilitar copia (dos máquinas no pueden compartir estado)
        GameFSM(const GameFSM &) = delete;
        GameFSM &operator=(const GameFSM &) = delete;

        // Permitir mover (para usar con std::unique_ptr)
        GameFSM(GameFSM &&) noexcept = default;
        GameFSM &operator=(GameFSM &&) noexcept = default;

        /**
         * @brief Intenta procesar un evento.
         *
         * @param event El evento que ocurrió (ej: INSERT_CREDIT)
         * @return true si la transición fue válida, false si fue rechazada
         *
         * Ejemplo:
         *   if (fsm.handleEvent(Event::INSERT_CREDIT)) {
         *       std::cout << "Dinero aceptado!\n";
         *   } else {
         *       std::cout << "No se puede insertar dinero ahora!\n";
         *   }
         */
        bool handleEvent(Event event);

        /**
         * @brief Obtiene el estado actual de forma segura.
         *
         * "De forma segura" significa que usa un std::mutex para evitar
         * que dos hilos lean al mismo tiempo que uno está escribiendo.
         */
        [[nodiscard]] State getCurrentState() const;

        /**
         * @brief Convierte un estado a texto (ej: State::IDLE → "IDLE")
         *
         * constexpr significa que puede calcularse en tiempo de compilación.
         * Eso la hace super rápida.
         */
        [[nodiscard]] static constexpr std::string_view stateToString(State state) noexcept;

        /**
         * @brief Convierte un evento a texto (ej: Event::INSERT_CREDIT → "INSERT_CREDIT")
         */
        [[nodiscard]] static constexpr std::string_view eventToString(Event event) noexcept;

        /**
         * @brief Registra una función que se ejecuta cada vez que cambia el estado.
         *
         * Ejemplo:
         *   fsm.setOnStateChangeCallback([](State old, State new) {
         *       std::cout << "Cambio de " << old << " a " << new << "\n";
         *   });
         */
        void setOnStateChangeCallback(StateChangeCallback callback);

    private:
        State m_currentState;                // El estado actual
        mutable std::mutex m_stateMutex;     // "Cerradura" para acceso seguro
        StateChangeCallback m_onStateChange; // Función que se ejecuta al cambiar

        /**
         * @brief Valida si una transición es legal.
         *
         * Por ejemplo: ¿Puedo ir de IDLE a ARMED? SÍ (metiste dinero)
         * ¿Puedo ir de IDLE a PAYOUT? NO (no está permitido)
         */
        [[nodiscard]] bool isValidTransition(State from, Event event, State &to) const noexcept;
    };

} // namespace Aether

#endif // AETHER_ENGINE_FSM_HPP