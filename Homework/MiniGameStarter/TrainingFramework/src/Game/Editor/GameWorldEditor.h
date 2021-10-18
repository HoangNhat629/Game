#pragma once

#include "Camera.h"
#include "Common/Vector2Int.h"
#include "ApplicationManagers/InputManager.h"

#include "PaletteEntry.h"

#include <memory>
#include <vector>
#include <array>

namespace Agvt
{
    class GameWorldEditor
    {
    public:
        GameWorldEditor();

        void Update(float deltaTime);
        void HandleTouchEvents(int x, int y, bool bIsPressed);
        void HandleMouseMoveEvents(int x, int y);

        void Draw();

        void LoadFromFile(int id);
        void SaveToFile(int id) const;

    private:
        static std::shared_ptr<Camera> CreateCamera();

        std::array<PaletteEntry, 2> LoadPalettes() const;
        std::unique_ptr<Sprite2D> LoadEraserSprite() const;

        void SwitchBrush(bool forward);
        void SwitchPalette(bool forward);
        void ToggleErasingMode();

        void PaintCurrentTile();

        Vector2 ScreenPointToWorld(int x, int y) const;
        size_t GridPos2Index(Vector2Int position) const;

    private:
        std::shared_ptr<Camera> m_worldCamera;
        Matrix m_screenPointToWorldMatrix;

        const InputManager* m_inputManager;
        Vector2Int m_hoveringGridPos;
        Vector2 m_hoveringWorldPosSnapped;

        std::vector<EditorGridSlot> m_mapLookup;

        std::array<PaletteEntry, 2> m_palettes;
        size_t m_currentPaletteIndex;

        std::unique_ptr<Sprite2D> m_eraserSpriteBrush;
        bool m_isErasing;

        Sprite2D* m_selectedSpriteBrush;

        uint8_t m_mapWidth;
        uint8_t m_mapHeight;
    };
}