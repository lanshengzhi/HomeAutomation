#ifndef PUSH_BUTTON_h
#define PUSH_BUTTON_h
#include <Arduino.h>
#include <functional>

class PushButton {
public:
    PushButton(uint8_t pin) : m_pin(pin) {}
    void Begin();
    uint8_t GetPin() const { return m_pin; }
    void SetPressedCallback(std::function<void()> callback);
    void SetReleasedCallback(std::function<void()> callback);
    void Run();
    enum State { RELEASED = HIGH, PRESSED = LOW };
    void SetDebounceTime(uint16_t debounceTime) { m_debounceTime = debounceTime; }
    
private:
    State m_stableState = RELEASED;
    State m_prevStableState = RELEASED;
    void Debounce();
    void Callback();

private:
    uint8_t m_pin;
    uint16_t m_debounceTime = 50;
    uint16_t m_lastDebounceTime = 0;
    uint8_t m_debounceState = 0;
    std::function<void()> m_pressedCallback;
    std::function<void()> m_releasedCallback;
};

#endif