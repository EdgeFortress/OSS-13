#include <Shared/Physics/MovePhysics.hpp>

#include <gtest/gtest.h>

TEST(test, ReturnsZeroWhenAllInputZero) {
    sf::Time elapsed = sf::Time::Zero;
    uf::vec2f shift = {0, 0};
    float moveSpeed = 0;
    uf::vec2i moveIntent = {0, 0};
    uf::vec2f constSpeed = {0, 0};
    uf::vec2f physSpeed = {0, 0};

    EXPECT_TRUE(uf::vec2f(0, 0) == uf::phys::countDeltaShift(elapsed, shift, moveSpeed, moveIntent, constSpeed, physSpeed));
}

TEST(test, ReturnsZeroWhenAllInputZeroExceptTime) {
    sf::Time elapsed = sf::seconds(2);
    uf::vec2f shift = {0, 0};
    float moveSpeed = 0;
    uf::vec2i moveIntent = {0, 0};
    uf::vec2f constSpeed = {0, 0};
    uf::vec2f physSpeed = {0, 0};

    EXPECT_TRUE(uf::vec2f(0, 0) == uf::phys::countDeltaShift(elapsed, shift, moveSpeed, moveIntent, constSpeed, physSpeed));
}

TEST(test, Intent_CorrectDeltaWhenMoved) {
    sf::Time elapsed = sf::seconds(0.5f);
    uf::vec2f shift = {0, 0};
    float moveSpeed = 2;
    uf::vec2i moveIntentX = {1, 0};
    uf::vec2i moveIntentY = {0, 1};
    uf::vec2f constSpeed = {0, 0};
    uf::vec2f physSpeed = {0, 0};

    EXPECT_TRUE(uf::vec2f(1, 0) == uf::phys::countDeltaShift(elapsed, shift, moveSpeed, moveIntentX, constSpeed, physSpeed));
    EXPECT_TRUE(uf::vec2f(0, 1) == uf::phys::countDeltaShift(elapsed, shift, moveSpeed, moveIntentY, constSpeed, physSpeed));
}

TEST(test, Intent_CuttedDeltaWhenMovedMoreThenIntent) {
    sf::Time elapsed = sf::seconds(1);
    uf::vec2f shift = {0, 0};
    float moveSpeed = 2;
    uf::vec2i moveIntentX = {1, 0};
    uf::vec2i moveIntentY = {0, 1};
    uf::vec2f constSpeed = {0, 0};
    uf::vec2f physSpeed = {0, 0};

    EXPECT_TRUE(uf::vec2f(1, 0) == uf::phys::countDeltaShift(elapsed, shift, moveSpeed, moveIntentX, constSpeed, physSpeed));
    EXPECT_TRUE(uf::vec2f(0, 1) == uf::phys::countDeltaShift(elapsed, shift, moveSpeed, moveIntentY, constSpeed, physSpeed));
}

TEST(test, Intent_DontMoveWhenShiftEqualIntent) {
    sf::Time elapsed = sf::seconds(2);
    uf::vec2f shiftX = {1, 0};
    uf::vec2f shiftY = {0, 1};
    float moveSpeed = 2;
    uf::vec2i moveIntentX = {1, 0};
    uf::vec2i moveIntentY = {0, 1};
    uf::vec2f constSpeed = {0, 0};
    uf::vec2f physSpeed = {0, 0};

    EXPECT_TRUE(uf::vec2f(0, 0) == uf::phys::countDeltaShift(elapsed, shiftX, moveSpeed, moveIntentX, constSpeed, physSpeed));
    EXPECT_TRUE(uf::vec2f(0, 0) == uf::phys::countDeltaShift(elapsed, shiftY, moveSpeed, moveIntentY, constSpeed, physSpeed));
}

TEST(test, Intent_SameSpeedWhenMovingDiagonally) {
    sf::Time elapsed = sf::seconds(1);
    uf::vec2f shift = {0, 0};
    float moveSpeed = 1;
    uf::vec2i moveIntent = {1, 1};
    uf::vec2f constSpeed = {0, 0};
    uf::vec2f physSpeed = {0, 0};

    // x ^ 2 = (1 * 1) ^ 2 / 2 = 0.5

    auto res = uf::phys::countDeltaShift(elapsed, shift, moveSpeed, moveIntent, constSpeed, physSpeed);
    EXPECT_TRUE(res.x * res.x - 0.5 < 0.000001);
}

