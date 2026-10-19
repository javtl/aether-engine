#include <gtest/gtest.h>
#include "Fsm.hpp"

using namespace Aether;

TEST(FsmTest, StartsInIdle)
{
    GameFSM fsm;
    EXPECT_EQ(fsm.getCurrentState(), State::IDLE);
}

TEST(FsmTest, CompleteValidCycle)
{
    GameFSM fsm;
    EXPECT_TRUE(fsm.handleEvent(Event::INSERT_CREDIT));
    EXPECT_EQ(fsm.getCurrentState(), State::ARMED);

    EXPECT_TRUE(fsm.handleEvent(Event::SPIN_REQUESTED));
    EXPECT_EQ(fsm.getCurrentState(), State::SPINNING);

    EXPECT_TRUE(fsm.handleEvent(Event::SPIN_COMPLETE));
    EXPECT_EQ(fsm.getCurrentState(), State::EVALUATING);

    EXPECT_TRUE(fsm.handleEvent(Event::EVALUATION_COMPLETE));
    EXPECT_EQ(fsm.getCurrentState(), State::PAYOUT);

    EXPECT_TRUE(fsm.handleEvent(Event::PAYOUT_COMPLETE));
    EXPECT_EQ(fsm.getCurrentState(), State::IDLE);
}

TEST(FsmTest, RejectsInvalidEvents)
{
    GameFSM fsm;
    EXPECT_FALSE(fsm.handleEvent(Event::SPIN_REQUESTED));
    EXPECT_FALSE(fsm.handleEvent(Event::SPIN_COMPLETE));
    EXPECT_EQ(fsm.getCurrentState(), State::IDLE);
}

TEST(FsmTest, TriggerErrorFromAnyState)
{
    GameFSM fsm;
    EXPECT_TRUE(fsm.handleEvent(Event::INSERT_CREDIT));
    EXPECT_TRUE(fsm.handleEvent(Event::TRIGGER_ERROR));
    EXPECT_EQ(fsm.getCurrentState(), State::ERROR);
}

TEST(FsmTest, ResetFromError)
{
    GameFSM fsm;
    fsm.handleEvent(Event::TRIGGER_ERROR);
    EXPECT_TRUE(fsm.handleEvent(Event::RESET));
    EXPECT_EQ(fsm.getCurrentState(), State::IDLE);
}

TEST(FsmTest, ResetOutsideErrorRejected)
{
    GameFSM fsm;
    EXPECT_FALSE(fsm.handleEvent(Event::RESET));
}

TEST(FsmTest, CallbackFires)
{
    int count = 0;
    GameFSM fsm([&](State, State)
                { ++count; });
    fsm.handleEvent(Event::INSERT_CREDIT);
    EXPECT_EQ(count, 1);

    fsm.handleEvent(Event::SPIN_REQUESTED);
    EXPECT_EQ(count, 2);
}

TEST(FsmTest, CallbackNotFiredOnInvalidEvent)
{
    int count = 0;
    GameFSM fsm([&](State, State)
                { ++count; });
    fsm.handleEvent(Event::SPIN_REQUESTED);
    EXPECT_EQ(count, 0);
}