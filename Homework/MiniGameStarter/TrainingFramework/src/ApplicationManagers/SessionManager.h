#pragma once

class SessionManager
{
public:
    SessionManager() : m_selectedLevel(-1)
    {
    }

    SessionManager(const SessionManager&) = delete;
    SessionManager& operator=(const SessionManager&) = delete;


    int GetSelectedLevel() const
    {
        return m_selectedLevel;
    }

    void SetSelectedLevel(int level)
    {
        m_selectedLevel = level;
    }

private:
    int m_selectedLevel;
};