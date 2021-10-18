#include "GameWorldEditor.h"

#include "../GameWorldConfig.h"
#include "../SpriteUtils/SpriteLoader.h"

#include "../Map/GameMap.h"

#include "Application.h"

namespace Agvt
{
    using namespace GameWorldConfig;

    GameWorldEditor::GameWorldEditor() : m_worldCamera(CreateCamera()), m_palettes(LoadPalettes()), m_currentPaletteIndex{},
        m_eraserSpriteBrush(LoadEraserSprite()), m_isErasing(false), m_selectedSpriteBrush{}, m_mapWidth(c_mapTileWidth), m_mapHeight(c_mapTileHeight)
    {
        m_inputManager = Application::GetInstance()->GetInputManager();
        m_screenPointToWorldMatrix = m_worldCamera->GetLookAtMatrix().Inverse();

        m_selectedSpriteBrush = m_palettes[m_currentPaletteIndex].GetEditorGridSlot().GetSprite();
    }

    void GameWorldEditor::Update(float deltaTime)
    {
        if (m_inputManager->KeyDown(KeyCode::LEFT))
        {
            SwitchBrush(false);
        }
        else if (m_inputManager->KeyDown(KeyCode::RIGHT))
        {
            SwitchBrush(true);
        }
        else if (m_inputManager->KeyDown(KeyCode::UP))
        {
            SwitchPalette(true);
        }
        else if (m_inputManager->KeyDown(KeyCode::DOWN))
        {
            SwitchPalette(false);
        }
        else if (m_inputManager->KeyDown(KeyCode::D))
        {
            ToggleErasingMode();
        }

        m_selectedSpriteBrush->Set2DPosition(m_hoveringWorldPosSnapped);

        if (m_inputManager->Touch())
        {
            PaintCurrentTile();
        }
    }

    void GameWorldEditor::HandleTouchEvents(int x, int y, bool bIsPressed)
    {
    }

    void GameWorldEditor::HandleMouseMoveEvents(int x, int y)
    {
        Vector2 worldPos = ScreenPointToWorld(x, y);
        m_hoveringGridPos = Vector2Int((int)(worldPos.x / c_tileSize), (int)(worldPos.y / c_tileSize));
        m_hoveringWorldPosSnapped = Vector2(m_hoveringGridPos.x + c_tileAlign, m_hoveringGridPos.y + c_tileAlign);
    }

    void GameWorldEditor::Draw()
    {
        for (auto& editorGridSlot : m_mapLookup)
        {
            if (editorGridSlot.GetContentType() != EditorGridSlot::ContentType::Empty)
            {
                editorGridSlot.GetSprite()->Draw();
            }
        }

        m_selectedSpriteBrush->Draw();
    }

    void GameWorldEditor::LoadFromFile(int id)
    {
        if (id == -1)
        {
            m_mapWidth = c_mapTileWidth;
            m_mapHeight = c_mapTileHeight;

            m_mapLookup = std::vector<EditorGridSlot>(m_mapWidth * m_mapHeight);

            return;
        }

        GameMap gameMap;
        if (!GameMap::LoadFromFile(id, &gameMap))
        {
            LOGI("Cannot load map %d, creating new map\n", id);

            m_mapWidth = c_mapTileWidth;
            m_mapHeight = c_mapTileHeight;

            m_mapLookup = std::vector<EditorGridSlot>(m_mapWidth * m_mapHeight);

            return;
        }

        m_mapWidth = gameMap.Width();
        m_mapHeight = gameMap.Height();

        m_mapLookup = std::vector<EditorGridSlot>(m_mapWidth * m_mapHeight);

        SpriteLoader spriteLoader(m_worldCamera);

        // Load tiles
        const std::vector<uint8_t>& tiles = gameMap.Tiles();
        for (size_t y = 0; y < m_mapHeight; y++)
        {
            for (size_t x = 0; x < m_mapWidth; x++)
            {
                std::pair<uint8_t, uint8_t> tileData = GameMap::UnpackTileData(tiles[GridPos2Index(Vector2Int(x, y))]);
                TileType tileType = (TileType)tileData.first;
                uint8_t tileDetails = tileData.second;

                if (tileType == TileType::Empty)
                {
                    continue;
                }

                EditorGridSlot& gridSlot = m_mapLookup[GridPos2Index(Vector2Int(x, y))];

                gridSlot = EditorGridSlot(
                    tileType,
                    tileDetails,
                    spriteLoader.LoadTileSprite(tileType, tileDetails, Vector2Int(x, y)));
            }
        }

        // Load entities
        const std::vector<GameMap::EntityEntry>& entityEntries = gameMap.Entities();
        for (const auto& entityEntry : entityEntries)
        {
            EditorGridSlot& gridSlot = m_mapLookup[GridPos2Index(Vector2Int(entityEntry.x, entityEntry.y))];

            EntityType entityType = (EntityType)entityEntry.entityType;

            gridSlot = EditorGridSlot(
                entityType,
                spriteLoader.LoadEntitySprite(entityType, Vector2Int(entityEntry.x, entityEntry.y)));
        }
    }

    void GameWorldEditor::SaveToFile(int id) const
    {
        GameMap gameMap = GameMap(m_mapWidth, m_mapHeight);

        // Save tiles and entities
        std::vector<uint8_t>& tiles = gameMap.Tiles();
        std::vector<GameMap::EntityEntry>& entityEntries = gameMap.Entities();

        for (size_t y = 0; y < m_mapHeight; y++)
        {
            for (size_t x = 0; x < m_mapWidth; x++)
            {
                const EditorGridSlot& gridSlot = m_mapLookup[GridPos2Index(Vector2Int(x, y))];

                if (gridSlot.GetContentType() == EditorGridSlot::ContentType::Tile)
                {
                    tiles[GridPos2Index(Vector2Int(x, y))] = gridSlot.GetTileDataPacked();
                }
                else if (gridSlot.GetContentType() == EditorGridSlot::ContentType::Entity)
                {
                    entityEntries.push_back(GameMap::EntityEntry{ (uint8_t)x, (uint8_t)y, (uint8_t)gridSlot.GetEntityType() });
                }
            }
        }

        if (GameMap::SaveToFile(gameMap, id))
        {
            LOGI("Map %d saved\n", id);
        }
    }


    std::shared_ptr<Camera> GameWorldEditor::CreateCamera()
    {
        return std::make_shared<Camera>(0, 0.0f, c_mapWidth, c_mapHeight, 0.0f, -1.0f, 1.0f, 0.0f);
    }

    std::array<PaletteEntry, 2> GameWorldEditor::LoadPalettes() const
    {
        SpriteLoader spriteLoader(m_worldCamera);

        // Load tile palette
        std::vector<EditorGridSlot> tilePalette;
        tilePalette.reserve(3);
        tilePalette.emplace_back(TileType::Wall, 0, spriteLoader.LoadTileSprite(TileType::Wall, 0));
        tilePalette.emplace_back(TileType::Wall, 1, spriteLoader.LoadTileSprite(TileType::Wall, 1));
        tilePalette.emplace_back(TileType::Exit, 0, spriteLoader.LoadTileSprite(TileType::Exit, 0));

        // Load entity palette
        std::vector<EditorGridSlot> entityPalette;
        entityPalette.reserve(4);

        entityPalette.emplace_back(EntityType::Player, spriteLoader.LoadEntitySprite(EntityType::Player));
        entityPalette.emplace_back(EntityType::Box, spriteLoader.LoadEntitySprite(EntityType::Box));
        entityPalette.emplace_back(EntityType::Rock, spriteLoader.LoadEntitySprite(EntityType::Rock));
        entityPalette.emplace_back(EntityType::Key, spriteLoader.LoadEntitySprite(EntityType::Key));

        return std::array<PaletteEntry, 2>
        {
            PaletteEntry(std::move(tilePalette)),
                PaletteEntry(std::move(entityPalette))
        };
    }

    std::unique_ptr<Sprite2D> GameWorldEditor::LoadEraserSprite() const
    {
        auto resourceManager = ResourceManagers::GetInstance();

        auto spriteModel = resourceManager->GetModel("Sprite2D-cartesian.nfg");
        auto spriteShader = resourceManager->GetShader("TextureShader");

        auto spriteTexture = resourceManager->GetTexture("eraser.tga");

        std::unique_ptr<Sprite2D> eraserSprite = std::make_unique<Sprite2D>(spriteModel, spriteShader, spriteTexture);
        eraserSprite->SetCamera(m_worldCamera);
        eraserSprite->SetSize(c_tileSize, c_tileSize);

        return eraserSprite;
    }

    void GameWorldEditor::SwitchBrush(bool forward)
    {
        if (m_isErasing)
        {
            return;
        }

        m_palettes[m_currentPaletteIndex].SwitchBrush(forward);

        m_selectedSpriteBrush = m_palettes[m_currentPaletteIndex].GetEditorGridSlot().GetSprite();
    }

    void GameWorldEditor::SwitchPalette(bool forward)
    {
        if (m_isErasing)
        {
            return;
        }

        if (forward)
        {
            if (m_currentPaletteIndex == m_palettes.size() - 1)
            {
                m_currentPaletteIndex = 0;
            }
            else
            {
                m_currentPaletteIndex++;
            }
        }
        else
        {
            if (m_currentPaletteIndex == 0)
            {
                m_currentPaletteIndex = m_palettes.size() - 1;
            }
            else
            {
                m_currentPaletteIndex--;
            }
        }

        m_selectedSpriteBrush = m_palettes[m_currentPaletteIndex].GetEditorGridSlot().GetSprite();
    }

    void GameWorldEditor::ToggleErasingMode()
    {
        m_isErasing = !m_isErasing;

        if (m_isErasing)
        {
            m_selectedSpriteBrush = m_eraserSpriteBrush.get();
        }
        else
        {
            m_selectedSpriteBrush = m_palettes[m_currentPaletteIndex].GetEditorGridSlot().GetSprite();
        }
    }

    void GameWorldEditor::PaintCurrentTile()
    {
        EditorGridSlot& gridSlot = m_mapLookup[GridPos2Index(m_hoveringGridPos)];

        if (m_isErasing)
        {
            gridSlot = EditorGridSlot();
        }
        else
        {
            const EditorGridSlot& brushGridSlot = m_palettes[m_currentPaletteIndex].GetEditorGridSlot();

            if (gridSlot == brushGridSlot)
            {
                return;
            }

            gridSlot = brushGridSlot.Clone();
            gridSlot.GetSprite()->Set2DPosition(m_hoveringWorldPosSnapped);
        }
    }

    Vector2 GameWorldEditor::ScreenPointToWorld(int x, int y) const
    {
        Vector4 pointNDC(x * 2.0f / (float)Globals::screenWidth - 1.0f,
            -y * 2.0f / (float)Globals::screenHeight + 1.0f, 0.0f, 1.0f);

        Vector4 worldPos = pointNDC * m_screenPointToWorldMatrix;

        return Vector2(worldPos.x, worldPos.y);
    }

    size_t GameWorldEditor::GridPos2Index(Vector2Int position) const
    {
        return position.x + (size_t)m_mapWidth * position.y;
    }
}
