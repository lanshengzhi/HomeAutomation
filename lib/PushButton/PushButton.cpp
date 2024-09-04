#include "PushButton.h"
#include <functional>

enum DebounceState {
    IDLE = 0,
    DEBOUNCING = 1
};

void PushButton::SetPressedCallback(std::function<void()> callback) {
    m_pressedCallback = callback;
}

void PushButton::SetReleasedCallback(std::function<void()> callback) {
    m_releasedCallback = callback;
}

void PushButton::Run() {
    Debounce();
    Callback();
}

void PushButton::Debounce() {
    uint8_t currentState = digitalRead(m_pin);      // ��ȡ������ƽ

    switch (m_debounceState) {  // ����ƴд����
        case IDLE:
            if (currentState != m_stableState) {    // �����ǰ״̬����һ��״̬��ͬ
                m_lastDebounceTime = millis();      // ����������ʱ��
                m_debounceState = DEBOUNCING;
            }
            break;

        case DEBOUNCING:                              
            if (currentState == static_cast<uint8_t>(m_stableState)) {
                m_debounceState = IDLE;
            } else if ((millis() - m_lastDebounceTime) > m_debounceTime) {
                m_stableState = static_cast<State>(currentState);
                m_debounceState = IDLE;
            }
            break;
    }
}

void PushButton::Callback() {
    if (m_prevStableState == m_stableState) {
        return;
    }

    if (m_stableState == LOW) {
        if (m_pressedCallback != nullptr) {
            m_pressedCallback();
        }
    } else {
        if (m_releasedCallback != nullptr) {
            m_releasedCallback();
        }
    }

    m_prevStableState = m_stableState;
}
