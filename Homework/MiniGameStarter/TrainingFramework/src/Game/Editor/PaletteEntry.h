#pragma once

#include "EditorGridSlot.h"

#include <vector>
#include <assert.h>

namespace Agvt
{
    class PaletteEntry
    {
    public:
        PaletteEntry(std::vector<EditorGridSlot> brushList) : m_brushList(std::move(brushList)), m_currentBrushIndex{}
        {
            assert(m_brushList.size() > 0);
        }

        PaletteEntry(const PaletteEntry&) = delete;
        PaletteEntry& operator=(const PaletteEntry&) = delete;

        PaletteEntry(PaletteEntry&&) = default;
        PaletteEntry& operator=(PaletteEntry&&) = default;

        const EditorGridSlot& GetEditorGridSlot() const
        {
            return m_brushList[m_currentBrushIndex];
        }

        void SwitchBrush(bool forward)
        {
            if (forward)
            {
                if (m_currentBrushIndex == m_brushList.size() - 1)
                {
                    m_currentBrushIndex = 0;
                }
                else
                {
                    m_currentBrushIndex++;
                }
            }
            else
            {
                if (m_currentBrushIndex == 0)
                {
                    m_currentBrushIndex = m_brushList.size() - 1;
                }
                else
                {
                    m_currentBrushIndex--;
                }
            }
        }

    private:
        std::vector<EditorGridSlot> m_brushList;
        size_t m_currentBrushIndex;
    };
}
